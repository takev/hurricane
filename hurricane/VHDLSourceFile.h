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
#ifndef TAKEVOS_HURRICANE_VHDLSOURCEFILE_H
#define TAKEVOS_HURRICANE_VHDLSOURCEFILE_H
#include "SourceFile.h"

namespace takevos {
namespace hurricane {

class VHDLSourceFile : SourceFile {
public:
    static const int library_pragma             = 1;
    static const int translate_pragma           = 2;
    static const int library_statement          = 3;
    static const int use_statement              = 4;
    static const int entity_instantiation       = 5;
    static const int package_declaration        = 6;
    static const int entity_declaration         = 7;
    static const int architecture_declaration   = 8;
    static const Tokenizer vhdl_tokenizer;

    VHDLSourceFile(fs::path const &filename);
    
private:
    bool                        translating;
    std::string                 destination_library;
    std::vector<std::string>    imported_libraries;

    void handle_library_pragma(std::string name);
    void handle_translate_pragma(std::string value);
    void handle_library_statement(std::string name);
    void handle_use_statement(std::string name);
    void handle_entity_instantiation(std::string library_name, std::string entity_name, std::string architecture_name);
    void handle_package_declaration(std::string name);
    void handle_entity_declaration(std::string name);
    void handle_architecture_declaration(std::string name, std::string entity_name);
    void parse(char const * const text, size_t text_size);
};

}}
#endif
