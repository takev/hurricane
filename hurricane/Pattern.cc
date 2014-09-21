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
#include "Pattern.h"

namespace takevos {
namespace hurricane {

Pattern::Pattern(int code, const std::string &pattern_text)
    : code(code)
{
    int     errcode;
    char    errstr[80];
    
    errcode  = regcomp(&pattern, pattern_text.c_str(), REG_EXTENDED);
    if (errcode != 0) {
        regerror(errcode, &pattern, errstr, sizeof (errstr) - 1);
        fprintf(stderr, "ERROR compiling regex: %s\n", errstr);
        abort();
    }
}

Pattern::~Pattern()
{
    regfree(&pattern);
}

off_t Pattern::search(Token &token, char const * const text, size_t text_size, off_t offset) const
{
    int         errcode;
    char        errstr[80];
    regmatch_t  match[10];
  
    // Erase the token value. 
    token.offset = -1;
    token.code = 0; 
    token.groups.erase(token.groups.begin(), token.groups.end());

    switch (errcode = regnexec(&pattern, &text[offset], text_size - offset, sizeof (match) / sizeof (match[0]), match, 0)) {
    case REG_NOMATCH:
        return -1;

    case 0:
        // Populate the token.
        token.offset = match[0].rm_so + offset;
        token.code = code;
        for (unsigned int i = 0; i < pattern.re_nsub; i++) {
            if (match[i+1].rm_so >= 0 && match[i+1].rm_eo >= 0) {
                std::string group_value(&text[match[i+1].rm_so + offset], match[i+1].rm_eo - match[i+1].rm_so);
                token.groups.push_back(group_value);
            } else {
                token.groups.push_back(std::string());
            }
        }

        return match[0].rm_eo + offset;

    default:
        regerror(errcode, &pattern, errstr, sizeof (errstr) - 1);
        fprintf(stderr, "ERROR executing regex: %s\n", errstr);
        abort();
    }
}

std::vector<Token> Pattern::search(char const * const text, size_t text_size) const
{
    std::vector<Token>  tokens;
    Token               token;
    off_t               offset = 0;

    while ((offset = search(token, text, text_size, offset)) >= 0) {
        tokens.push_back(token);
    }

    return tokens;
}

}}
