/*
 * quickxor - QuickXor hash library
 *
 * © 2019 by Tekki (Rolf Stöckli)
 *
 * The original C# algorithm was published by Microsoft under the following copyright:
 *
 *   Copyright (c) 2016 Microsoft Corporation
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, andor sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 */

#include <math.h>
#include <openssl/hmac.h>
#include <string.h>
#include "quickxor.h"

QX* QX_new() {
  QX* pqx = NULL;

  pqx = calloc(1, sizeof(QX));
  if (pqx) {
    pqx->kWidthInBits  = 160;
    pqx->kShift        = 11;
    pqx->kWidthInBytes = ceil((double)pqx->kWidthInBits / 8);
    pqx->kDataLength   = ceil((double)pqx->kWidthInBits / 64);

    pqx->data = calloc(pqx->kDataLength, sizeof(uint64_t));
    if (!pqx->data) {
      free(pqx);
      pqx = NULL;
    }
  }

  return pqx;
}

void QX_add(QX* pqx, uint8_t* addData, size_t addSize) {
  // the bitvector where we'll start xoring
  size_t vectorArrayIndex = pqx->shiftSoFar / 64;

  // the position within the bit vector at which we begin xoring
  int    vectorOffset = pqx->shiftSoFar % 64;
  size_t iterations   = addSize > pqx->kWidthInBits ? pqx->kWidthInBits : addSize;

  for (size_t i = 0; i < iterations; ++i) {
    size_t nextCell         = vectorArrayIndex + 1;
    int    bitsInVectorCell = 64;
    if (nextCell == pqx->kDataLength) {
      nextCell = 0;
      if (pqx->kWidthInBits % 64 > 0) {
        bitsInVectorCell = pqx->kWidthInBits % 64;
      }
    }

    uint8_t xoredByte = 0x0;
    for (size_t j = i; j < addSize; j += pqx->kWidthInBits) {
      xoredByte ^= addData[j];
    }

    pqx->data[vectorArrayIndex] ^= (uint64_t)xoredByte << vectorOffset;

    if (vectorOffset > bitsInVectorCell - 8) {
      pqx->data[nextCell] ^= (uint64_t)xoredByte >> (bitsInVectorCell - vectorOffset);
    }

    vectorOffset += pqx->kShift;
    if (vectorOffset >= bitsInVectorCell) {
      vectorArrayIndex = nextCell;
      vectorOffset -= bitsInVectorCell;
    }
  }

  // update the starting position in a circular shift pattern
  pqx->shiftSoFar += pqx->kShift * (addSize % pqx->kWidthInBits);
  pqx->shiftSoFar %= pqx->kWidthInBits;
  pqx->lengthSoFar += addSize;
}

uint8_t* QX_digest(QX* pqx) {
  uint8_t* digest       = NULL;
  uint8_t* digestLength = NULL;
  size_t   lengthSize   = sizeof(pqx->lengthSoFar);

  // create a byte array big enough to hold all our data
  digest = calloc(1, pqx->kWidthInBytes);

  if (digest) {
    // block copy all our bitvectors to this byte array
    memcpy(digest, pqx->data, pqx->kWidthInBytes);
    digestLength = calloc(1, lengthSize);
    if (digestLength) {
      // xor the file length with the least significant bits
      memcpy(digestLength, &pqx->lengthSoFar, lengthSize);

      for (size_t i = 0; i < lengthSize; ++i) {
        digest[pqx->kWidthInBytes - lengthSize + i] ^= digestLength[i];
      }

      free(digestLength);
      digestLength = NULL;
    }
  }

  return digest;
}

char* QX_b64digest(QX* pqx) {
  BIO*     mem    = NULL;
  BIO*     b64    = NULL;
  char*    hash   = NULL;
  uint8_t* digest = NULL;

  digest = QX_digest(pqx);
  mem    = BIO_new(BIO_s_mem());
  b64    = BIO_new(BIO_f_base64());
  hash   = calloc(2, pqx->kWidthInBytes);

  if (digest && mem && b64 && hash) {
    b64 = BIO_push(b64, mem);
    BIO_write(b64, digest, pqx->kWidthInBytes);
    BIO_flush(b64);

    BIO_read(mem, hash, pqx->kWidthInBytes * 2);
    hash[strcspn(hash, "\r\n")] = '\0';
  }

  free(digest);
  digest = NULL;
  if (b64) {
    BIO_free_all(b64);
    b64 = NULL;
  } else {
    BIO_free_all(mem);
  }
  mem = NULL;

  return hash;
}

void QX_free(QX* pqx) {
  if (pqx) {
    free(pqx->data);
    free(pqx);
  }
}

void QX_reset(QX* pqx) {
  if (pqx) {
    uint64_t* old_data = pqx->data;
    pqx->data          = calloc(pqx->kDataLength, sizeof(uint64_t));
    free(old_data);
    pqx->lengthSoFar = 0;
    pqx->shiftSoFar  = 0;
  }
}

int QX_readFile(QX* pqx, char* filename) {
  int status = 1;
  if (pqx && strlen(filename)) {
    FILE* pFile = NULL;
    pFile       = fopen(filename, "rb");
    if (pFile) {
      uint8_t buf[4096];
      size_t  len;

      QX_reset(pqx);
      while ((len = fread(buf, 1, 4096, pFile)) > 0) {
        QX_add(pqx, buf, len);
      }
      fclose(pFile);

      status = 0;
    }
  }
  return status;
}