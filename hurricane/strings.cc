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
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include "strings.h"

namespace takevos {
namespace hurricane {


std::string string_format(std::string fmt, ...)
{
    va_list ap;
    char    *s;

    va_start(ap, fmt);

    if (vasprintf(&s, fmt.c_str(), ap) == -1) {
        throw std::runtime_error(std::string("Could not format string.") + strerror(errno));
    }

    std::string r(s);

    free(s);

    va_end(ap);

    return r;
}

std::vector<std::string> split_string(const std::string &haystack, const std::string &needle)
{
    auto pos = 0l;
    auto r = std::vector<std::string>();

    while (pos < haystack.length()) {
        auto i = haystack.find(needle, pos);

        if (i == std::string::npos) {
            r.push_back(haystack.substr(pos, haystack.length() - pos));
            break;
        } else {
            r.push_back(haystack.substr(pos, i - pos));
        }

        pos = i + needle.length();
    }

    return r;
}

void write_to_file(const boost::filesystem::path &filename, const std::string &text)
{
    int fd;

    if ((fd = open(filename.string().c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
        throw std::runtime_error("Could not open file '" + filename.string() + "'.");
    }

    if (write(fd, text.c_str(), text.length()) == -1) {
        throw std::runtime_error("Could not write to file '" + filename.string() + "'.");
    }

    if (close(fd) == -1) {
        throw std::runtime_error("Could not close file '" + filename.string() + "'.");
    }
}

}}
