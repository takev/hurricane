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
#ifndef TAKEVOS_HURRICANE_NUMBERS_H
#define TAKEVOS_HURRICANE_NUMBERS_H

#include <string>
#include <vector>
#include <tuple>
#include <CoreServices/CoreServices.h>
#include "strings.h"
#include "numbers.h"

typedef __uint128_t uint128_t;

static inline uint64_t htobe64(uint64_t x) {
    return CFSwapInt64HostToBig(x);
}

static inline uint64_t htole64(uint64_t x) {
    return CFSwapInt64HostToLittle(x);
}

static inline uint32_t le32toh(uint32_t x) {
    return CFSwapInt32LittleToHost(x);
}

static inline uint32_t htole32(uint32_t x) {
    return CFSwapInt32HostToLittle(x);
}

static inline uint128_t be128toh(uint128_t x) {
#ifdef __BIG_ENDIAN__
    return x;
#elif __LITTLE_ENDIAN__
    return (((uint128_t)CFSwapInt64BigToHost(x & 0xffffffffffffffff)) << 64) | CFSwapInt64BigToHost(x >> 64);
#else
#error "Unknown byte order"
#endif
}

static inline uint32_t rol32(uint32_t x, int count) {
    return (x << count) | (x >> (32 - count));
}

constexpr uint128_t operator"" _ULLL(const char *s) {
    int         offset = 0;
    int         base = 0;
    int         digit = 0;
    char        c = 0;
    uint128_t   r = 0;

    switch (s[0]) {
    case '\0':
        return 0;
    case '0':
        switch (s[1]) {
        case '\0':  return 0;
        case 'b': case 'B': base =  2; offset = 2; break;
        case 'o': case 'O': base =  8; offset = 2; break;
        case 'd': case 'D': base = 10; offset = 2; break;
        case 'x': case 'X': base = 16; offset = 2; break;
        default:            base =  8; offset = 1;
        }
        break;
    default:
        base = 10;
        offset = 0;
    }

    while ((c = s[offset]) != '\0') {
        if (c >= '0' && c <= '9') {
            digit = c - '0';
        } else if (c >= 'a' && c <= 'f') {
            digit = c - 'a' + 10;
        } else if (c >= 'A' && c <= 'F') {
            digit = c - 'A' + 10;
        } else {
            return 0;
        }

        r *= base;
        r += digit;
        offset++;
    }

    return r;
}

namespace std {

static inline std::ostream &operator<<(std::ostream &os, const uint128_t &x) {
    os << takevos::hurricane::string_format("0x%08llx%08llx", (unsigned long long)(x >> 64), (unsigned long long)(x & 0xffffffffffffffff));
    return os;
}

}

#endif
