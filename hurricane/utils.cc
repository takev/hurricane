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
#include "numbers.h"

namespace takevos {
namespace hurricane {

std::tuple<boost::filesystem::path,boost::filesystem::path> path_split(boost::filesystem::path const & path)
{
    auto                                _path = path.string();
    size_t                              i;
    std::pair<std::string,std::string>  r;

    i = _path.find_last_of('/');
    if (i == std::string::npos) {
        // No '/' in path, so it is just a file.
        return make_tuple(
            "",
            _path
        );

    } else if (i == 0) {
        // Leaf the root slash when it is there.
        return make_tuple(
            "/",
            _path.substr(i + 1, std::string::npos)
        );

    } else {
        // Simply split the path.
        return make_tuple(
            _path.substr(0, i),
            _path.substr(i + 1, std::string::npos)
        );
    }
}

std::vector<boost::filesystem::path> super_directories_with_file(boost::filesystem::path const & directory, boost::filesystem::path const & filename)
{
    if (directory.empty()) {
        // We have completely finished.
        return std::vector<boost::filesystem::path>();
    }

    if (canonical(directory) != directory) {
        throw std::invalid_argument("Directory is not canonical.");
    }

    // Recurse toward root.
    auto r = super_directories_with_file(directory.parent_path(), filename);

    if (exists(directory / filename)) {
        // The file is found, add the directory where the file was found.
        r.push_back(directory);
    }

    return r;
}

boost::filesystem::path furthest_super_directory_with_file(boost::filesystem::path const & directory, boost::filesystem::path const & filename)
{
    auto directories = super_directories_with_file(directory, filename);
    return directories.front();
}

boost::filesystem::path nearest_super_directory_with_file(boost::filesystem::path const & directory, boost::filesystem::path const & filename)
{
    auto directories = super_directories_with_file(directory, filename);
    return directories.back();
}


std::vector<boost::filesystem::path> search_file_in_subdirectories(boost::filesystem::path const & directory, std::vector<std::string> extensions)
{
    std::vector<boost::filesystem::path>   tmp;
    //recursive_directory_iterator i = directory;
    //for (auto
    return tmp;
}


}}
