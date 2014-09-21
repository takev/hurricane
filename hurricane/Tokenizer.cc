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

#include <iostream>
#include <future>
#include <stdarg.h>
#include <alloca.h>
#include "Tokenizer.h"

namespace takevos {
namespace hurricane {

Token::Token() :
    code(0)
{
}

Token::Token(int code, ...) :
    code(code)
{
    va_list ap;

    va_start(ap, code);
    while (true) {
        const char  *tmp;

        tmp = va_arg(ap, const char *);
        if (tmp == NULL) {
            break;
        }

        groups.push_back(std::string(tmp));
    }
    va_end(ap);
}

bool Token::operator==(const Token &other) const
{
    return !(*this != other);
}

bool Token::operator!=(const Token &other) const
{
    if (code != other.code) {
        return true;
    }

    if (groups.size() != other.groups.size()) {
        return true;
    }

    for (unsigned int i = 0; i < groups.size(); i++) {
        if (groups[i] != other.groups[i]) {
            return true;
        }
    }

    return false;
}

std::ostream &operator<<(std::ostream &os, const Token &x)
{
    os << "<Token " << x.code << ": ";
    for (unsigned int i = 0; i < x.groups.size(); i++) {
        if (i != 0) {
            os << ", ";
        }
        os << "'" << x.groups[i] << "'";
    }
    os << ">";
    return os;
}

Tokenizer::Tokenizer(int code1, char const * const pattern1, ...)
{
    va_list     ap;
    int         tmp_code        = code1;
    const char  *tmp_pattern    = pattern1;
    int         errcode;
    char        errstr[80];

    std::string combined_pattern = "(";

    va_start(ap, pattern1);
    while (true) {
        regex_t     tmp_pattern_re;

        // Compile each pattern to extract the number of sub-expressions in each pattern.
        errcode = regcomp(&tmp_pattern_re, tmp_pattern, REG_EXTENDED);
        if (errcode != 0) {
            regerror(errcode, &tmp_pattern_re, errstr, sizeof (errstr) - 1);
            fprintf(stderr, "ERROR compiling pattern '%s': %s\n", tmp_pattern, errstr);
            abort();
        }

        // Append each pattern into a large pattern.
        SubPattern sub_pattern(tmp_code, tmp_pattern_re.re_nsub);
        combined_pattern += tmp_pattern;

        sub_patterns.push_back(sub_pattern);
        regfree(&tmp_pattern_re);


        // Get the next code and pattern.
        tmp_code    = va_arg(ap, int);
        if (tmp_code == 0) {
            // Last pattern.
            combined_pattern += ")";
            break;
        } else {
            // More patterns to follow.
            combined_pattern += ")|(";
        }
        tmp_pattern = va_arg(ap, const char *);
    }
    va_end(ap);

    // Compile the large pattern.
    errcode = regcomp(&combined_pattern_re, combined_pattern.c_str(), REG_EXTENDED);
    if (errcode != 0) {
        regerror(errcode, &combined_pattern_re, errstr, sizeof (errstr) - 1);
        fprintf(stderr, "ERROR compiling combined pattern '%s': %s\n", combined_pattern.c_str(), errstr);
        abort();
    }
}

Tokenizer::~Tokenizer()
{
    regfree(&combined_pattern_re);
}

Token Tokenizer::parse(char const * const text, size_t text_size, int &offset)
{
    Token       token;
    int         errcode;
    char        errstr[80];

    regmatch_t  *match = (regmatch_t *)alloca(sizeof (regmatch_t) * (combined_pattern_re.re_nsub + 1));
    unsigned int i;
  
    switch (errcode = regnexec(&combined_pattern_re, &text[offset], text_size - offset, combined_pattern_re.re_nsub + 1, match, 0)) {
    case REG_NOMATCH:
        offset = -1;
        return token;

    case 0:
        // The first sub pattern starts at index 1.
        i = 1;
        for (auto sub_pattern: sub_patterns) {
            // Check if the main sub-expression around each sub-pattern is in use.
            if (match[i].rm_so >= 0 && match[i].rm_eo >= 0) {
                token.code = sub_pattern.code;

                // Now walk through each sub-expression of this sub-pattern.
                for (int j = 0; j < sub_pattern.nsub; j++) {
                    if (match[i+j+1].rm_so >= 0 && match[i+j+1].rm_eo >= 0) {
                        std::string group_value(&text[match[i+j+1].rm_so + offset], match[i+j+1].rm_eo - match[i+j+1].rm_so);
                        token.groups.push_back(group_value);
                    } else {
                        token.groups.push_back(std::string());
                    }
                }
            }

            // Skip to the main sub-expression of the next sub-pattern.
            i+= sub_pattern.nsub + 1;
        }

        // Update offset to behind the match.
        offset+= match[0].rm_eo;

        return token;

    default:
        regerror(errcode, &combined_pattern_re, errstr, sizeof (errstr) - 1);
        fprintf(stderr, "ERROR executing regex: %s\n", errstr);
        abort();
    }
}

TokenizerRange Tokenizer::parse(char const * const text, size_t text_size)
{
    TokenizerRange  range(this);
    int             offset = 0;

    while (true) {
        // offset is an inout argument.
        Token token = parse(text, text_size, offset);
        if (offset == -1) {
            break;
        }
        range.tokens.push_back(token);
    }
    return range;
}

TokenizerRange::TokenizerRange(Tokenizer const * const tokenizer) :
    tokenizer(tokenizer)
{
}

TokenizerIterator TokenizerRange::begin(void) const {
    return TokenizerIterator(this, 0);
}

TokenizerIterator TokenizerRange::end(void) const {
    return TokenizerIterator(this, tokens.size());
}

TokenizerIterator::TokenizerIterator(TokenizerRange const * const tokenizer_range, off_t i) :
    tokenizer_range(tokenizer_range), i(i)
{
}

const TokenizerIterator &TokenizerIterator::operator++() {
    i++;
    return *this;
}

bool TokenizerIterator::operator!=(const TokenizerIterator &other) const {
    return i != other.i;
}

Token TokenizerIterator::operator*() const {
    return tokenizer_range->tokens[i];
}

}}
