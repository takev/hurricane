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

#define BOOST_TEST_MODULE FileHandle
#include <boost/test/unit_test.hpp>
#include <boost/test/execution_monitor.hpp>
#include <fcntl.h>
#include "utils.h"
#include "strings.h"
#include "FileHandle.h"

using namespace std;
using namespace takevos::hurricane;

BOOST_AUTO_TEST_CASE(filehandle_test_1)
{
    std::string     text = "Hello World";
    auto            filename = fs::path(string_format("/tmp/test-%i.txt", (int)getpid()));

    write_to_file(filename, text);

    FileHandle handle(filename);

    handle.open();

    BOOST_CHECK_EQUAL(handle.data_size, text.length());
    BOOST_CHECK_EQUAL(std::string(handle.data, handle.data_size), string(text));

    handle.close();
    
    BOOST_CHECK_EQUAL(handle.data_size, 0);

    if (unlink(filename.string().c_str()) == -1) {
        throw std::runtime_error("Could not unlink file '" + filename.string() + "'.");
    }
}

