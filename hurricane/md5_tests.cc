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

#define BOOST_TEST_MODULE utils
#include <boost/test/unit_test.hpp>
#include <boost/test/execution_monitor.hpp>
#include "md5.h"

using namespace takevos::hurricane;
using namespace std;

BOOST_AUTO_TEST_CASE(MD5_hash_1)
{
    __uint128_t result = MD5("");
    __uint128_t expected = 0xd41d8cd98f00b204e9800998ecf8427e_ULLL;

    BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_CASE(MD5_hash_2)
{
    __uint128_t result = MD5("a");
    __uint128_t expected = 0x0cc175b9c0f1b6a831c399e269772661_ULLL;

    BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_CASE(MD5_hash_3)
{
    __uint128_t result = MD5("abc");
    __uint128_t expected = 0x900150983cd24fb0d6963f7d28e17f72_ULLL;

    BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_CASE(MD5_hash_4)
{
    __uint128_t result = MD5("message digest");
    __uint128_t expected = 0xf96b697d7cb7938d525a2f31aaf161d0_ULLL;

    BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_CASE(MD5_hash_5)
{
    __uint128_t result = MD5("abcdefghijklmnopqrstuvwxyz");
    __uint128_t expected = 0xc3fcd3d76192e4007dfb496cca67e13b_ULLL;

    BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_CASE(MD5_hash_6)
{
    __uint128_t result = MD5("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    __uint128_t expected = 0xd174ab98d277d9f5a5611c2c9f419d9f_ULLL;

    BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_CASE(MD5_hash_7)
{
    __uint128_t result = MD5("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
    __uint128_t expected = 0x57edf4a22be3c955ac49da2e2107b67a_ULLL;

    BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_CASE(MD5_hash_8)
{
    __uint128_t result = MD5("The quick brown fox jumps over the lazy dog");
    __uint128_t expected = 0x9e107d9d372bb6826bd81d3542a419d6_ULLL;

    BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_CASE(MD5_hash_9)
{
    __uint128_t result = MD5("Test vector from febooti.com");
    __uint128_t expected = 0x500ab6613c6db7fbd30c62f5ff573d0f_ULLL;

    BOOST_CHECK_EQUAL(result, expected);
}



