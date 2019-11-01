#ifndef QUICKXOR_H
#define QUICKXOR_H

#include <stdint.h>
#include <stdlib.h>

typedef struct {
  size_t    kDataLength;
  size_t    kShift;
  size_t    kWidthInBits;
  size_t    kWidthInBytes;
  size_t    lengthSoFar;
  size_t    shiftSoFar;
  uint64_t* data;
} QX;

QX*  QX_new();

void QX_add(QX* pqx, uint8_t* addData, size_t addSize);

char* QX_b64digest(QX* pqx);

uint8_t* QX_digest(QX* pqx);

void QX_free(QX* pqx);

int QX_readFile(QX* pqx, char* filename);

void QX_reset(QX* pqx);

#endif
