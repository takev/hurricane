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
#ifndef TAKEVOS_HURRICANE_OPTIONS_H
#define TAKEVOS_HURRICANE_OPTIONS_H
#include <stdbool.h>
#include <string>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace takevos {
namespace hurricane {

#define LOG_DEBUG     "\x01"
#define LOG_INFO      "\x02"
#define LOG_NOTICE    "\x03"
#define LOG_WARNING   "\x04"
#define LOG_ERROR     "\x05"
#define LOG_FATAL     "\x06"

class Options {
public:
    static const int        simulation  = 1;
    static const int        synthesis   = 2;

    std::string             application;
    char                    verbose;
    std::string             target;
    fs::path                working_directory;
    fs::path                project_directory;
    fs::path                current_library_directory;
    fs::path                library_filename;
    int                     compilation_mode;

    Options(void);

    /** Post process the options.
     * Secondary information is gathered.
     */
    void post_process(void);

    /** Print a message of a certain level to stderr.
     * Only print the message when the log level is lower or equal to the verbose level.
     */
    void log(const char *msg, ...);

    /** Print the usage information to stderr, then exit with the exit_code.
     */
    void usage(void);

    /** Parse command line options.
     * @param argc  Number of command line arguments.
     * @param argv  List of command line arguments.
     */
    void parse(int argc, char *argv[]);
};

extern Options options;

}}
#endif
