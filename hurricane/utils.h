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
#ifndef TAKEVOS_HURRICANE_UTILS_H
#define TAKEVOS_HURRICANE_UTILS_H

#include <string>
#include <vector>
#include <tuple>
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;

namespace takevos {
namespace hurricane {

/** Split a path into a directory and filename part.
 *
 * @param path  Path the split.
 * @return (directory, filename)
 */
tuple<path,path> path_split(path const & path) __attribute__((pure));

/** Find a file by traversing each directory in the path.
 *
 * @param directory The path in which to search the file
 * @param filename  The name of the file to find in the path.
 * @return A list of directories where filename is found. Ordered from root first.
 */
vector<path> super_directories_with_file(path const & directory, path const & filename);

/** Find the furthest super directory containing a file.
 *
 * @param directory The path to start the search from. The path itself may be returned.
 * @param filename  The filename to search.
 * @return The super directory furthest toward the root that contains the filename.
 */
path furthest_super_directory_with_file(path const & directory, path const & filename);

/** Find the nearest super directory containing a file.
 *
 * @param directory The path to start the search from. The path itself may be returned.
 * @param filename  The filename to search.
 * @return The super directory nearest to the search-start that contains the filename.
 */
path nearest_super_directory_with_file(path const & directory, path const & filename);


/** Find all files with a certain extension by tranversing into subdirectories.
 */
vector<path> search_file_in_subdirectories(path const & directory, vector<string> extensions);

}}

#endif
