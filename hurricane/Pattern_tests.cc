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

#define BOOST_TEST_MODULE Pattern
#include <boost/test/unit_test.hpp>
#include <boost/test/execution_monitor.hpp>
#include "Pattern.h"

using namespace takevos::hurricane;

BOOST_AUTO_TEST_CASE(pattern_simple_1)
{
    Pattern     p(1, "a");
    const char  *test = "abac";
    Token       expected;
    Token       result;
    off_t       o = 0;

    o = p.search(result, test, sizeof (test), o);
    expected = Token(1, 0, NULL);
    BOOST_CHECK_EQUAL(o, 1);
    BOOST_CHECK_EQUAL(result, expected);

    o = p.search(result, test, sizeof (test), o);
    expected = Token(1, 2, NULL);
    BOOST_CHECK_EQUAL(o, 3);
    BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_CASE(pattern_groups_1)
{
    Pattern     p(2, "([0-9]+).([0-9]+).([0-9]+).([0-9]+)");
    const char  test[] = "foo\nbaz 127.0.0.1 -> 10.1.2.3 bar";
    Token       expected;
    Token       result;
    off_t       o = 0;

    o = p.search(result, test, sizeof (test), o);
    expected = Token(2, 8, "127", "0", "0", "1", NULL);
    BOOST_CHECK_EQUAL(o, 17);
    BOOST_CHECK_EQUAL(result, expected);

    o = p.search(result, test, sizeof (test), o);
    expected = Token(2, 21, "10", "1", "2", "3", NULL);
    BOOST_CHECK_EQUAL(o, 29);
    BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_CASE(pattern_groups_2)
{
    Pattern     p(2, "(([0-9]+).([0-9]+)).([0-9]+).([0-9]+)");
    const char  test[] = "foo\nbaz 127.0.0.1 -> 10.1.2.3 bar";
    std::vector<Token>  expected;
    std::vector<Token>  result;

    result = p.search(test, sizeof (test));
    expected.push_back(Token(2, 8, "127", "0", "0", "1", NULL));
    expected.push_back(Token(2, 21, "10", "1", "2", "3", NULL));

    BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected.begin(), expected.end());
}





