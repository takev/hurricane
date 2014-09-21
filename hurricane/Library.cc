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
#include "Library.h"
#include "Options.h"

namespace takevos {
namespace hurricane {

Library::Library(fs::path const &library_directory) :
    library_directory(library_directory),
    configuration_path(library_directory / options.library_filename)
{
    pt::ini_parser::read_ini(configuration_path.string(), configuration);
}

void Library::walk(fs::path const &directory)
{
    /*
    fs::directory_iterator end;

    for (auto i = fs::directory_iterator(directory); i != end; i++) {
        auto x = *i;
        if (is_directory(x.path())) {
            if (exists(x.path() / options.library_filename)) {
                options.log(LOG_NOTICE "Found library: %s", x.path().string().c_str());
            } else {
                walk(x.path());
            }

        } else {
            options.log(LOG_NOTICE "entry: %s", x.path().string().c_str());
            auto ext = x.path().extension();

            auto j = options.source_file_handlers.find(ext);
            if (j != options.source_file_handlers.end()) {
                auto source_file_handler = *j;

                SourceFile(this, source_file_handler, x.path());
            }
        }
    }
    */
}

void Library::walk(void)
{
    walk(library_directory);
}

}}
