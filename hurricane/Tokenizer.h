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
#ifndef TAKEVOS_HURRICANE_TOKENIZER_H
#define TAKEVOS_HURRICANE_TOKENIZER_H
#include <stdbool.h>
#include <string>
#include <vector>
#include <regex.h>

namespace takevos {
namespace hurricane {

class TokenizerRange;
class TokenizerIterator;

struct SubPattern {
    int                 code;
    int                 nsub;

    SubPattern(int code, int nsub) : code(code), nsub(nsub) { }
};

struct Token {
    int                         code;
    std::vector<std::string>    groups;

    Token();
    Token(int code, ...);

    bool operator!=(const Token &other) const;
    bool operator==(const Token &other) const;
};

std::ostream &operator<<(std::ostream &os, const Token &x);

class Tokenizer {
public:
    regex_t                     combined_pattern_re;
    std::vector<SubPattern>     sub_patterns;

    Tokenizer(int code1, char const * const str1, ...);
    Token parse(char const * const text, size_t text_size, int &offset);
    TokenizerRange parse(char const * const text, size_t text_size);
    ~Tokenizer();
};

class TokenizerRange {
public:
    const Tokenizer     *tokenizer;
    std::vector<Token>  tokens;

    TokenizerRange(Tokenizer const * const tokenizer);

    TokenizerIterator begin(void) const;
    TokenizerIterator end(void) const;
};

class TokenizerIterator {
public:
    const TokenizerRange    *tokenizer_range;
    off_t                   i;

    TokenizerIterator(TokenizerRange const * const tokenizer_range, off_t i);

    const TokenizerIterator &operator++();

    bool operator!=(const TokenizerIterator &other) const;

    Token operator*() const;
};

}}
#endif
