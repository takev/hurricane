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
#ifndef TAKEVOS_HURRICANE_PATTERN_H
#define TAKEVOS_HURRICANE_PATTERN_H
#include <string>
#include <vector>
#include <iostream>
#include <regex.h>
#include "Options.h"

namespace takevos {
namespace hurricane {


/** A regex patern.
 */
struct Pattern {
    int                 code;
    regex_t             pattern;

    /** Initialize a pattern.
     *
     * @param code          to return in a match.
     * @param pattern_text  A regex-string pattern.
     */
    Pattern(int code, const std::string &pattern_text);

    /** Free the regex being created by the constructor.
     */
    ~Pattern();

    /** Search the pattern in the text.
     * @param token     The returned token, the value of the token will be overwritten.
     * @param text      The text to search.
     * @param text_size The size of the text.
     * @param offset    The offset in the text to start the search.
     * @return          The offset behind the match, or -1 if no match is found.
     */
    off_t search(Token &token, char const * const text, size_t text_size, off_t offset) const;

    /** Search for all tokens in the text.
     * @param text      The text to search.
     * @param text_size The size of the text.
     * @return          A list of tokens, sorted by the offset in the text.
     */
    std::vector<Token> search(char const * const text, size_t text_size) const;
};

}}
#endif
