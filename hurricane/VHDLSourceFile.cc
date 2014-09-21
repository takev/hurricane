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

#include <boost/regex.hpp>
#include "Tokenizer.h"
#include "VHDLSourceFile.h"
#include "Options.h"

namespace takevos {
namespace hurricane {

const std::string   hurricane_library_re_str        = "--\s+hurricane_library\s+(\w+)";
const std::string   import_re_str                   = "use\s+([_.[:alnum:]]+)\s*;";
const std::string   entity_instance_re_str          = "\w+\s*:\s*(?:entity\s+)?([_.()[:alnum:]]+)\s(?:generic|port)\s+map\s+\(";
const std::string   package_re_str                  = "package\s+(\w+)\s*is\s+";
const std::string   entity_declaration_re_str       = "entity\s+(\w+)\s+is\s+";
const std::string   architecture_declaration_re_str = "architecture\s+(\w+)\s+of\s+(\w+)\s+is\s+";

const Tokenizer     tokenizer(
    'L', "--\s+hurricane_library\s+(\w+)",
    'U', "use\s+([_.[:alnum:]]+)\s*;",
    'I', "\w+\s*:\s*(?:entity\s+)?([_.()[:alnum:]]+)\s(?:generic|port)\s+map\s+\(",
    'P', "package\s+(\w+)\s*is\s+",
    'E', "entity\s+(\w+)\s+is\s+",
    'A', "architecture\s+(\w+)\s+of\s+(\w+)\s+is\s+",
    0, ""
);

VHDLSourceFile::VHDLSourceFile(ProjectConfiguration const &project_configuration, LibraryConfiguration const &library_configuration, fs::path const &filename) : SourceFile(project_configuration, library_configuration, filename)
{
}

VHDLSourceFile::parse(void)
{

}


}}
