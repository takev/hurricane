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

#define BOOST_TEST_MODULE VHDLSourceFile
#include <boost/test/unit_test.hpp>
#include <boost/test/execution_monitor.hpp>
#include <boost/filesystem.hpp>
#include "VHDLSourceFile.h"

using namespace std;
using namespace boost::filesystem;
using namespace takevos::hurricane;

BOOST_AUTO_TEST_CASE(tokenizer_simple_1)
{
    const char *text =
        "--\n"
        "-- hurricane_library testlib;\n"
        "--\n"
        "\n"
        "use work.use1;\n"
        "\n"
        "package testpkg is\n"
        "\n"
        "end package;\n"
        "\n"
        "-- entity foo is\n"
        "\n"
        "entity testentity is\n"
        "   port (\n"
        "   );\n"
        "end;\n"
        "\n"
        "architecture rtl of testentity is\n"
        "begin\n"
        "   i1: entity work.instance1 port map(clk => clk);\n"
        "\n"
        "   i2: entity instance2 port map(clk => clk);\n"
        "\n"
        "   i3: entity work.instance3(rtl)\n"
        "       port map(clk => clk);\n"
        "\n"
        "   i4: instance4 port map(clk => clk);\n"
        "\n"
        "end;\n";
    size_t text_size = strlen(text);

    VHDLSourceFile      source_file(absolute("testcases/parsers/test.vhd"));
    std::vector<Token>  result = source_file.vhdl_tokenizer.tokenize(text, text_size);
    std::vector<Token>  expected;
    
    expected.push_back(Token(VHDLSourceFile::library_pragma,            "testlib", NULL));
    expected.push_back(Token(VHDLSourceFile::use_statement,             "work.use1", NULL));
    expected.push_back(Token(VHDLSourceFile::package_declaration,       "testpkg", NULL));
    expected.push_back(Token(VHDLSourceFile::entity_declaration,        "testentity", NULL));
    expected.push_back(Token(VHDLSourceFile::architecture_declaration,  "rtl", "testentity", NULL));
    expected.push_back(Token(VHDLSourceFile::entity_instantiation,      "work.instance1", NULL));
    expected.push_back(Token(VHDLSourceFile::entity_instantiation,      "instance2", NULL));
    expected.push_back(Token(VHDLSourceFile::entity_instantiation,      "work.instance3(rtl)", NULL));
    expected.push_back(Token(VHDLSourceFile::entity_instantiation,      "instance4", NULL));
    BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected.begin(), expected.end());
}


