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
#ifndef TAKEVOS_HURRICANE_SOURCEFILE_H
#define TAKEVOS_HURRICANE_SOURCEFILE_H
#include <string>
#include <sys/stat.h>
#include <boost/filesystem.hpp>
#include <memory.h>
#include <string>
#include <map>
#include "MapQuery.h"
#include "Tokenizer.h"
#include "numbers.h"

namespace fs = boost::filesystem;

namespace takevos {
namespace hurricane {

using DQ = MapQuery<std::string,std::string>;
using DQMap = std::map<std::string,std::string>;



/** A source file.
 */
class SourceFile {
public:
    fs::path            filename;   ///< filename of the file
    uint128_t           md5hash;
    std::vector<DQ>     needs;      ///< Required objects.
    std::vector<DQMap>  provides;   ///< Objects that this file creates.

    /** Open a source file.
     * @param filename  A path the a file.
     */
    SourceFile(fs::path const &filename);

    /** Close and unmap a file.
     */
    virtual ~SourceFile();

    virtual void process_file(void);

    inline void add_need(const DQ &q) {
        needs.push_back(q);
    }

    inline void add_provide(const DQ &q) {
        provides.push_back(q.map());
    }

    virtual void parse(char const * const text, size_t text_size) = 0;
};

}}
#endif
