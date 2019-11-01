# NAME

libquickxor - C implementation of the QuickXor hash

# SYNOPSIS

    # command line
    $ quickxorhash FILE[s]

    /* library */
    #include <quickxor.h>
    pqx = QX_new();

    QX_add(pData, size);
    QX_readFile(filename);

    pDigest = QX_digest(pqx);
    pBase64Digest = QX_b64digest(pqx);

    QX_reset(pqx);
    QX_free(pqx);

# DESCRIPTION

The C implementation of the QuickXor hash.

The QuickXor hash is the digest used by Microsoft on Office 365 OneDrive for Business and Sharepoint.
It was published by Microsoft in 2016 in form of a C\# script. The explanation describes it as a
"quick, simple non-cryptographic hash algorithm that works by XORing the bytes in a circular-shifting fashion".

# INSTALLATION

    wget https://github.com/Tekki/quickxor-c/archive/v0.01.tar.gz
    tar xzf quickxor-0.01.tar.gz
    cd quickxor-c-0.01
    autoreconf --install
    ./configure
    make
    make check  # optional
    sudo make install
    sudo ldconfig

# BINARY

## quickxorhash

    quickxorhash FILE[s]

Calculates the QuickXor hash for one or multiple files.

# LIBRARY FUNCTIONS

## QX\_new

    QX* pqx = QX_new();

Returns a pointer to a new QX structure.

## QX\_add

    /* void QX_add(QX* pqx, uint8_t* addData, size_t addSize) */

    uint8_t* addData = ...;
    size_t addSize = ...;
    QX_add(pqx, addData, addSize);

Adds a new block of data.

## QX\_b64digest

    /* char* QX_b64digest(QX* pqx) */

    char* b64digest = QX_b64digest(pqx);

Returns the Base64 encoded digest for the data.
This operation does not affect the data and can be repeated.

## QX\_digest

    /* uint8_t* QX_digest(QX* pqx) */

    uint8_t* digest = QX_digest(pqx);

Returns the binary array of the digest for the data.
This operation does not affect the data and can be repeated.

## QX\_free

    /* void QX_free(QX* pqx) */

    QX_free(pqx);

Frees the memory reserved for the specified QX.

## QX\_readFile

    /* int QX_readFile(QX* pqx, char* filename) */

    char filename[] = "a file";
    int status = QX_readFile(pqx, filename);

Resets the QX, reads the file and adds its content.
Returns 0 on success and 1 if the file could not be read.

## QX\_reset

    /* void QX_reset(QX* pqx) */

    QX_reset(pqx);

Resets the QX to its orignal state so it can be used to calculate a new digest.

# AUTHOR & COPYRIGHT

© 2019 by Tekki (Rolf Stöckli).

© for the original algorithm 2016 by Microsoft.

This program is free software, you can redistribute it and/or modify it under
the terms of the Artistic License version 2.0.

# SEE ALSO

[QuickXorHash Algorithm](https://docs.microsoft.com/en-us/onedrive/developer/code-snippets/quickxorhash?view=odsp-graph-online)
in the OneDrive Dev Center.

[QuickXorHash.cs](https://gist.github.com/rgregg/c07a91964300315c6c3e77f7b5b861e4)
by Ryan Gregg.
