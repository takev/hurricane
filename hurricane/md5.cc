/* Copyright (c) 2014-2014, Take Vos
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice, 
 *   this list of conditions and the following disclaimer in the documentation 
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "utils.h"
#include "md5.h"

namespace takevos {
namespace hurricane {


const unsigned int MD5_chunk_size = 64;

void MD5_round(int i, uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t &f, int &g, uint32_t chunk[])
{
    uint32_t tmp;
    const int shift_table[64] = {
        7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
        5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
        4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
        6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
    };
    const uint32_t random_table[64] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };

    tmp = d;
    d = c;
    c = b;
    b = b + rol32(a + f + random_table[i] + chunk[g], shift_table[i]);
    a = tmp;
}

void MD5_sign_chunk(uint32_t hash[4], const uint32_t *_chunk)
{
    uint32_t    chunk[16];
    uint32_t    a = hash[0];
    uint32_t    b = hash[1];
    uint32_t    c = hash[2];
    uint32_t    d = hash[3];
    uint32_t    f;
    int         g;
    int         i;

    for (i = 0; i < 16; i++) {
        chunk[i] = le32toh(_chunk[i]);
    }

    for (i = 0; i < 16; i++) {
        f = (b & c) | ((~b) & d);
        g = i;
        MD5_round(i, a, b, c, d, f, g, chunk);
    }
    for (i = 16; i < 32; i++) {
        f = (d & b) | ((~d) & c);
        g = (5 * i + 1) & 0xf;
        MD5_round(i, a, b, c, d, f, g, chunk);
    }
    for (i = 32; i < 48; i++) {
        f = b ^ c ^ d;
        g = (3 * i + 5) & 0xf;
        MD5_round(i, a, b, c, d, f, g, chunk);
    }
    for (i = 48; i < 64; i++) {
        f = c ^ (b | (~d));
        g = (7 * i) & 0xf;
        MD5_round(i, a, b, c, d, f, g, chunk);
    }

    hash[0] += a;
    hash[1] += b;
    hash[2] += c;
    hash[3] += d;
}

/**
 * The last fragment is one or two 64 byte chunks containing:
 * - The last piece of data.
 * - A terminating 0x80 byte.
 * - Padding 0x00 bytes
 * - 64 bit big endian length of data, aligned to the end-edge of a chunk.
 */
void MD5_sign_last_fragment(uint32_t hash[4], const char *fragment, size_t fragment_size, size_t data_size)
{
    char    last_chunks[2 * MD5_chunk_size];
    size_t  last_chunks_size; // The size of the last_chunks is unknown until padding is calculated.
    off_t   offset = 0;

    memcpy(last_chunks, fragment, fragment_size);
    offset += fragment_size;

    // Add a '1' bit right after the data.
    last_chunks[offset++] = 0x80;

    // The last 64 bit of the last chunk is the length.
    if (offset < (MD5_chunk_size - sizeof (uint64_t))) {
        // There enough room in the current chunk.
        last_chunks_size = MD5_chunk_size;
    } else {
        // We have to use an extra chunk.
        last_chunks_size = (2 * MD5_chunk_size);
    }

    size_t padding_size = last_chunks_size - sizeof (uint64_t) - offset;
    memset(&last_chunks[offset], 0, padding_size);
    offset += padding_size;

    // Save the size as big endian.
    *((uint64_t *)&last_chunks[offset]) = htole64(data_size);

    MD5_sign_chunk(hash, (uint32_t *)&last_chunks[0]);
    if (last_chunks_size > MD5_chunk_size) {
        MD5_sign_chunk(hash, (uint32_t *)&last_chunks[MD5_chunk_size]);
    }
}

uint128_t MD5(const char *data, size_t data_size)
{
    unsigned int offset;
    uint32_t hash[4] = {
        0x67452301,
        0xefcdab89,
        0x98badcfe,
        0x10325476
    };

    for (offset = 0; offset + MD5_chunk_size - 1 < data_size ; offset += MD5_chunk_size) {
        MD5_sign_chunk(hash, (uint32_t *)&data[offset]);
    }
    MD5_sign_last_fragment(hash, &data[offset], data_size - offset, data_size * 8);

    uint32_t result_hash[4];

    result_hash[0] = htole32(hash[0]);
    result_hash[1] = htole32(hash[1]);
    result_hash[2] = htole32(hash[2]);
    result_hash[3] = htole32(hash[3]);

    return be128toh(*(uint128_t *)result_hash);
}

}}
