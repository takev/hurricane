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

#define BOOST_TEST_MODULE Tokenizer
#include <boost/test/unit_test.hpp>
#include <boost/test/execution_monitor.hpp>
#include "Tokenizer.h"

using namespace takevos::hurricane;

BOOST_AUTO_TEST_CASE(tokenizer_simple_1)
{
    Tokenizer           p(1, "a", 0);
    const char          *test = "abac";
    std::vector<Token>  expected;

    auto result = p.parse(test, strlen(test));
    expected.push_back(Token(1, NULL));
    expected.push_back(Token(1, NULL));
    BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(tokenizer_groups_1)
{
    Tokenizer           p(2, "([0-9]+).([0-9]+).([0-9]+).([0-9]+)", 0);
    const char          *test = "foo\nbaz 127.0.0.1 -> 10.1.2.3 bar";
    std::vector<Token>  expected;

    auto result = p.parse(test, strlen(test));
    expected.push_back(Token(2, "127", "0", "0", "1", NULL));
    expected.push_back(Token(2, "10", "1", "2", "3", NULL));
    BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(tokenizer_groups_2)
{
    Tokenizer           p(
        1, R"||(use\s+([a-zA-Z0-9_.]+)\s*;)||",
        2, R"||(library\s+([a-zA-Z0-9_.]+)\s*;)||",
        0
    );

    const char          *test =
        "begin\n"
        "library foo;\n"
        "use foo.bar;\n"
        "end;\n";

    std::vector<Token>  expected;

    auto result = p.parse(test, strlen(test));
    expected.push_back(Token(2, "foo", NULL));
    expected.push_back(Token(1, "foo.bar", NULL));
    BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected.begin(), expected.end());
}




