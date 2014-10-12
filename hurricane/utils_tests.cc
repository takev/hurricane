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
#include "utils.h"

using namespace takevos::hurricane;
using namespace std;
using namespace boost::filesystem;

namespace fs = boost::filesystem;

struct F {
    fs::path base_path;

    F() {
        base_path = string_format("/tmp/utils-tests-%i", (int)getpid());
        create_directory(base_path);

        // Just in case symbolic links are pointing from /tmp
        base_path = canonical(base_path);

        create_directory(base_path / "foo");
        write_to_file(base_path / "foo" / "hurricane.ini", string("Hello World"));
        create_directory(base_path / "foo" / "bar");
        write_to_file(base_path / "foo" / "bar" / "hurricane.ini", string("Hello World"));
        create_directory(base_path / "foo" / "bar" / "baz");
        create_directory(base_path / "foo" / "baz");
        write_to_file(base_path / "foo" / "baz" / "test.vhd", string("Hello World"));
    }

    ~F() {
        remove_all(base_path);
    }
};

BOOST_FIXTURE_TEST_SUITE(utils, F)

BOOST_AUTO_TEST_CASE(path_split_1)
{
    auto p = path_split("/foo/bar/baz.txt");
    BOOST_CHECK_EQUAL(get<0>(p), "/foo/bar");
    BOOST_CHECK_EQUAL(get<1>(p), "baz.txt");
}

BOOST_AUTO_TEST_CASE(path_split_2)
{
    auto p = path_split("/foo/bar/baz");
    BOOST_CHECK_EQUAL(get<0>(p), "/foo/bar");
    BOOST_CHECK_EQUAL(get<1>(p), "baz");
}

BOOST_AUTO_TEST_CASE(path_split_3)
{
    auto p = path_split("/foo/bar/baz/");
    BOOST_CHECK_EQUAL(get<0>(p), "/foo/bar/baz");
    BOOST_CHECK_EQUAL(get<1>(p), "");
}

BOOST_AUTO_TEST_CASE(path_split_4)
{
    auto p = path_split("/foo");
    BOOST_CHECK_EQUAL(get<0>(p), "/");
    BOOST_CHECK_EQUAL(get<1>(p), "foo");
}

BOOST_AUTO_TEST_CASE(path_split_5)
{
    auto p = path_split("/foo/");
    BOOST_CHECK_EQUAL(get<0>(p), "/foo");
    BOOST_CHECK_EQUAL(get<1>(p), "");
}

BOOST_AUTO_TEST_CASE(path_split_6)
{
    auto p = path_split("/");
    BOOST_CHECK_EQUAL(get<0>(p), "/");
    BOOST_CHECK_EQUAL(get<1>(p), "");
}

BOOST_AUTO_TEST_CASE(path_split_7)
{
    auto p = path_split("foo");
    BOOST_CHECK_EQUAL(get<0>(p), "");
    BOOST_CHECK_EQUAL(get<1>(p), "foo");
}

BOOST_AUTO_TEST_CASE(path_split_8)
{
    auto p = path_split("foo/bar");
    BOOST_CHECK_EQUAL(get<0>(p), "foo");
    BOOST_CHECK_EQUAL(get<1>(p), "bar");
}

BOOST_AUTO_TEST_CASE(path_split_9)
{
    auto p = path_split("");
    BOOST_CHECK_EQUAL(get<0>(p), "");
    BOOST_CHECK_EQUAL(get<1>(p), "");
}

BOOST_AUTO_TEST_CASE(super_directories_with_file_1)
{
    vector<path>    result = super_directories_with_file(base_path, "hurricane.ini");
    vector<path>    expected = {};

    BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(super_directories_with_file_2)
{
    vector<path>    result = super_directories_with_file(base_path / "foo", "hurricane.ini");
    vector<path>    expected = {base_path / "foo"};

    BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(super_directories_with_file_3)
{
    BOOST_CHECK_THROW(super_directories_with_file(base_path / "foo/", "hurricane.ini"), invalid_argument);
}

BOOST_AUTO_TEST_CASE(super_directories_with_file_4)
{
    vector<path>    result = super_directories_with_file(base_path / "foo" / "bar", "hurricane.ini");
    vector<path>    expected = {base_path / "foo", base_path / "foo" / "bar"};

    BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(super_directories_with_file_5)
{
    vector<path>    result = super_directories_with_file(base_path / "foo" / "bar" / "baz", "hurricane.ini");
    vector<path>    expected = {base_path / "foo", base_path / "foo" / "bar"};

    BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(super_directories_with_file_6)
{
    BOOST_CHECK_THROW(super_directories_with_file(base_path / "wrong" / "bar" / "baz", "hurricane.ini"), runtime_error);
}

BOOST_AUTO_TEST_CASE(super_directories_with_file_7)
{
    BOOST_CHECK_THROW(super_directories_with_file(base_path / "foo" / "bar" / "wrong", "hurricane.ini"), runtime_error);
}

BOOST_AUTO_TEST_CASE(nearest_super_directory_with_file_4)
{
    auto result = nearest_super_directory_with_file(base_path / "foo" / "bar", "hurricane.ini");
    auto expected = base_path / "foo" / "bar";

    BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_CASE(furthest_super_directory_with_file_4)
{
    auto result = furthest_super_directory_with_file(base_path / "foo" / "bar", "hurricane.ini");
    auto expected = base_path / "foo";

    BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_CASE(string_format_1)
{
    BOOST_CHECK_EQUAL(string_format("Hello %i", 15), string("Hello 15"));
}

BOOST_AUTO_TEST_CASE(string_split_1)
{
    auto result = split("Hello World", " ");
    auto expected = std::vector<std::string>{"Hello", "World"};

    BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_SUITE_END()