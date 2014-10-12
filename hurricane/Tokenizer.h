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

/** Information about a pattern that was loaded into the Tokenizer.
 */
struct SubPattern {
    int code;   ///< Code to return when the pattern is found in the text.
    int nsub;   ///< Number of capturing sub expressions for this pattern.

    /** Initialize a SubPattern, with information about a pattern.
     * @param code  The token code to return when a pattern was found in the text.
     *              0 when the token should not be returned.
     *              -1 is used as a sentinal.
     * @param nsub  Number of capturing sub expressions for this pattern.
     */
    SubPattern(int code, int nsub) : code(code), nsub(nsub) { }
};

/** Token found during parsing.
 */
struct Token {
    static const int            sentinal = -1;
    static const int            suppress = 0;

    int                         code;       ///< Code matching the pattern.
    std::vector<std::string>    groups;     ///< Captured sub expressions.

    /** Non-initialized token.
     */
    Token();

    /** Initialize token.
     * This constructor is used for test benches.
     * @param code  The code belonging with the pattern.
     * @param ...   Captures sub expressions of type (char const * cost), terminated with NULL.
     */
    Token(int code, ...);

    /** Compare if not equal.
     * Used in unit test to compare the result with expected.
     */
    bool operator!=(const Token &other) const;

    /** Compare if equal.
     * Used in unit test to compare the result with expected.
     */
    bool operator==(const Token &other) const;
};

/** Output information about token.
 * Used in unit test to visually compare result with expected.
 */
std::ostream &operator<<(std::ostream &os, const Token &x);

/** A sparse tokenizer for source code.
 * This tokenizer is designed to return just pieces of information from a source file at high speed.
 */
class Tokenizer {
public:
    regex_t                     combined_pattern_re;    ///< All patterns are combined in a single regular expression.
    std::vector<SubPattern>     sub_patterns;           ///< Information about each pattern in stored here.

    /** Initialize the tokenizer with a set of patterns.
     * @param code1     The code to return when this pattern is found in the text.
     * @param str1      The pattern to find in the text. In extended regular expression format.
     * @param ...       More code,str pairs, followd by a single -1 as sentinal.
     */
    Tokenizer(int code1, char const * const str1, ...);

    /** Destructor.
     * Free the regular expression.
     */
    ~Tokenizer();

    /** Find a single token in the text starting at offset.
     * Source code will be mapped into memory and there will not be a trailing zero.
     * Therefor the size of the text neesds to be given.
     *
     * @param text          The text to parse.
     * @param text_size     The size of the text.
     * @param offset        The offset to start parsing.
     *                      Returns the offset after the found pattern, or -1 when pattern is not found.
     * @return The token found with the captured sub expressions.
     */
    Token tokenize(char const * const text, size_t text_size, int &offset) const;

    /** Find all tokens in the text.
     * Source code will be mapped into memory and there will not be a trailing zero.
     * Therefor the size of the text neesds to be given.
     *
     * @param text          The text to parse.
     * @param text_size     The size of the text.
     * @return The tokens found with the captured sub expressions.
     */
    std::vector<Token> tokenize(char const * const text, size_t text_size) const;
};

}}
#endif
