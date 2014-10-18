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

#define BOOST_TEST_MODULE MapQuery
#include <boost/test/unit_test.hpp>
#include <boost/test/execution_monitor.hpp>
#include "MapQuery.h"

using namespace std;
using namespace takevos::hurricane;

BOOST_AUTO_TEST_CASE(mapquery_simple_1)
{
    auto r = DQ("Hello", "World");

    BOOST_CHECK_EQUAL(r.string(), "Hello:World");
}

BOOST_AUTO_TEST_CASE(mapquery_and_1)
{
    auto r = DQ("Hello", "World") & DQ("Foo", "Bar");

    BOOST_CHECK_EQUAL(r.string(), "(Hello:World&Foo:Bar)");
}

BOOST_AUTO_TEST_CASE(mapquery_and_2)
{
    auto r = DQ("Hello", "World") & DQ("Foo", "Bar") & DQ("1", "2");

    BOOST_CHECK_EQUAL(r.string(), "(Hello:World&Foo:Bar&1:2)");
}

BOOST_AUTO_TEST_CASE(mapquery_or_1)
{
    auto r = DQ("Hello", "World") | DQ("Foo", "Bar");

    BOOST_CHECK_EQUAL(r.string(), "(Hello:World|Foo:Bar)");
}

BOOST_AUTO_TEST_CASE(mapquery_complex_1)
{
    auto r = (DQ("Hello", "World") | DQ("Foo", "Bar")) & DQ("1", "2") & DQ("3", "4");

    BOOST_CHECK_EQUAL(r.string(), "((Hello:World|Foo:Bar)&1:2&3:4)");
}


BOOST_AUTO_TEST_CASE(mapquery_complex_2)
{
    auto r = (DQ("Hello", "World") & DQ("Foo", "Bar")) | (DQ("1", "2") & DQ("3", "4"));

    BOOST_CHECK_EQUAL(r.string(), "((Hello:World&Foo:Bar)|(1:2&3:4))");
}

BOOST_AUTO_TEST_CASE(mapquery_compare_1)
{
    auto                                r = (DQ("Hello", "World") & DQ("Foo", "Bar")) | (DQ("1", "2") & DQ("3", "4"));
    std::map<std::string,std::string>   e;

    e["Hello"] = "World";
    e["Foo"] = "Bar";
    BOOST_CHECK(r == e);
}

BOOST_AUTO_TEST_CASE(mapquery_compare_2)
{
    auto                                r = (DQ("Hello", "World") & DQ("Foo", "Bar")) | (DQ("1", "2") & DQ("3", "4"));
    std::map<std::string,std::string>   e;

    e["Hello"] = "World";
    e["1"] = "2";
    BOOST_CHECK(!(r == e));
}

BOOST_AUTO_TEST_CASE(mapquery_compare_with_different_sizes_1)
{
    auto                                r = (DQ("Hello", "World") & DQ("Foo", "Bar")) | (DQ("1", "2") & DQ("3", "4")) | DQ("1", "2");
    std::map<std::string,std::string>   e1;
    std::map<std::string,std::string>   e2;

    e1["1"] = "2";
    e1["3"] = "4";
    BOOST_CHECK(r == e1);

    e2["1"] = "2";
    BOOST_CHECK(r == e2);
}


