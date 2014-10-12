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

#include "VHDLSourceFile.h"
#include "Tokenizer.h"
#include "Options.h"

namespace takevos {
namespace hurricane {

const Tokenizer VHDLSourceFile::vhdl_tokenizer(
    library_pragma,             R"__(--\s+(?:pragma|synthesis|synopsys|exemplar)\s+library\s+([_.[:alnum:]]+))__",
    translate_pragma,           R"__(--\s+(?:pragma|synthesis|synopsys|exemplar)\s+translate_(off|on))__",
    translate_pragma,           R"__(--\s+(?:pragma|synthesis|synopsys|exemplar)\s+synthesis_(off|on))__",
    translate_pragma,           R"__(--\s+rtl_synthesis\s+(off|on))__",
    Token::suppress,            R"__(--.*?$)__",
    library_statement,          R"__(library\s+([_[:alnum:]]+)\s*;)__",
    use_statement,              R"__(use\s+([_.[:alnum:]]+)\s*;)__",
    entity_instantiation,       R"__(\w+\s*:\s*(?:entity\s+)?(?:([_[:alnum:]]+)\.)?([_[:alnum:]]+)(?:\([_[:alnum:]]+)\))?\s+(?:generic|port)\s+map\s*\()__",
    package_declaration,        R"__(package\s+(\w+)\s+is\s+)__",
    entity_declaration,         R"__(entity\s+(\w+)\s+is\s+)__",
    architecture_declaration,   R"__(architecture\s+(\w+)\s+of\s+(\w+)\s+is\s+)__",
    Token::sentinal
);

VHDLSourceFile::VHDLSourceFile(fs::path const &filename) :
    SourceFile(filename), destination_library("workd"), translating(true)
{
}

void VHDLSourceFile::handle_library_pragma(std::string name)
{
    destination_library = name;
}

void VHDLSourceFile::handle_translate_pragma(std::string value)
{
    // The translate_on and translate_off pragmas only function during synthesis.
    if (options.compilation_mode == Options::synthesis) {
        if (value == "on") {
            // All statements will be processed.
            translating = true;

        } else if (value == "off") {
            // Only pragmas will be processed.
            translating = false;
        }
    }
}

void VHDLSourceFile::handle_library_statement(std::string name)
{
    if (!translating) {
        return;
    }

    imported_libraries.push_back(name);
}

void VHDLSourceFile::handle_use_statement(std::string path)
{
    if (!translating) {
        return;
    }

    auto parts = split(name, ".");

    // The first part is a library if it was imported, otherwise it is a package/entity
    // in the work library.
    i = 0;
    if (contains(parts[i], imported_libraries)) {
        library = parts[i++];
    } else {
        library = "work";
    }

    // The second part is a package/entity.
    name = parts[i++];

    // We don't care about the third part of the use statement.
    add_need(DQ("lib", library) & (DQ("pkg", name) | DQ("ent", name)));
}


void VHDLSourceFile::handle_entity_instantiation(std::string library_name, std::string entity_name, std::string architecture_name)
{
    if (!translating) {
        return;
    }

    DQ library_q;
    if (library_name != "") {
        library_q = DQ("lib", library_name);
    } else {
        for (auto &x: imported_libraries) {
            library_q |= DQ("lib", x);
        }
    }

    DQ architecture_q;
    if (architecture_name != "") {
        architecture_q = DQ("arch", architecture_name);
    } else {
        architecture_q = DQ("arch");
    }

    add_need(library_q & DQ("ent", entity_name) & architecture_q);
}

void VHDLSourceFile::handle_package_declaration(std::string name)
{
    if (!translating) {
        return;
    }

    add_provide(DQ("lib", destination_library) & DQ("pkg", name));
}

void VHDLSourceFile::handle_entity_declaration(std::string name)
{
    if (!translating) {
        return;
    }

    add_provide(DQ("lib", destination_library) & DQ("ent", name));
}

void VHDLSourceFile::handle_architecture_declaration(std::string name, std::string entity_name)
{
    if (!translating) {
        return;
    }

    DQ library_q;
    for (auto &x: imported_libraries) {
        library_q |= DQ("lib", x);
    }

    add_need(library_q & DQ("ent", entity_name));
    add_provide(DQ("lib", destination_library) & DQ("ent", entity_name) & DQ("arch", name));
}


void VHDLSourceFile::parse(char const * const text, size_t text_size)
{
    for (auto &token: vhdl_tokenizer.tokenize(text, text_size)) {
        switch (token.code) {
        case library_pragma:
            handle_library_pragma(token.groups[0]);
            break;
        case translate_pragma:
            handle_translate_pragma(token.groups[0]);
            break;
        case library_statement:
            handle_library_statement(token.groups[0]);
            break;
        case use_statement:
            handle_use_statement(token.groups[0]);
            break;
        case entity_instantiation:
            handle_entity_instantiation(token.groups[0]);
            break;
        case package_declaration:
            handle_package_declaration(token.groups[0]);
            break;
        case entity_declaration:
            handle_entity_declaration(token.groups[0]);
            break;
        case architecture_declaration:
            handle_architecture_declaration(token.groups[0], token.groups[1]);
            break;
        }
    }
}


}}
