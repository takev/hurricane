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

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdarg.h>

#include "utils.h"
#include "options.h"

namespace takevos {
namespace hurricane {

using namespace std;

Options options = Options();

Options::Options(void) {
    verbose             = LOG_WARNING[0];
    application         = "<unknown>";
    working_directory   = fs::current_path();
    target              = "all";
    library_filename    = "hurricane.ini";
}

void Options::usage(void) {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "    %s -h\n", application.c_str());
    fprintf(stderr, "    %s [OPTIONS]\n", application.c_str());
    fprintf(stderr, "    %s [OPTIONS] <target>\n", application.c_str());
    fprintf(stderr, "\n");
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "    <target>                               Build target, by default: 'all'.\n");
    fprintf(stderr, "    -h, --help                             Show usage.\n");
    fprintf(stderr, "    -v, --verbose                          More verbose messages.\n");
    fprintf(stderr, "    -C, --working-directory=<directory>    Change working directory. (%s)\n", working_directory.string().c_str());
    fprintf(stderr, "    -F, --library-filename=<directory>     The name of a library filename. (%s)\n", library_filename.string().c_str());
}

void Options::post_process(void) {
    project_directory = furthest_super_directory_with_file(working_directory, library_filename);
    log(LOG_NOTICE "Found project directory at: %s", project_directory.string().c_str());

    current_library_directory = nearest_super_directory_with_file(working_directory, library_filename);
    log(LOG_NOTICE "Found current library directory at: %s", current_library_directory.string().c_str());
}

void Options::log(const char *msg, ...) {
    va_list     ap;
    char        *str;
    const char  *level_str;

    va_start(ap, msg);

    if (msg[0] > 0x10) {
        // Normal strings are always logged.
        vfprintf(stderr, msg, ap);

    } else if (msg[0] >= verbose) {
        // If the string is prefixed with a level, then log if it is better or equal than verbose.

        switch (msg[0]) {
        case LOG_DEBUG[0]:      level_str = "DEBUG";   break;
        case LOG_INFO[0]:       level_str = "INFO";    break;
        case LOG_NOTICE[0]:     level_str = "NOTICE";  break;
        case LOG_WARNING[0]:    level_str = "WARNING"; break;
        case LOG_ERROR[0]:      level_str = "ERROR";   break;
        case LOG_FATAL[0]:      level_str = "FATAL";   break;
        default:                level_str = "UNKNOWN"; break;
        }

        if (msg[1] > 0x10) {
            vasprintf(&str, &msg[1], ap);
        } else {
            vasprintf(&str, &msg[2], ap);
        }

        fprintf(stderr, "%s: %s\n", level_str, str);

        if (msg[0] == LOG_FATAL[0]) {
            abort();
        }
    }

    va_end(ap);
}

void Options::parse(int argc, char *argv[]) {
    struct option longopts[] = {
        {"help",                no_argument,        NULL, 'h'},
        {"verbose",             no_argument,        NULL, 'v'},
        {"working-directory",   required_argument,  NULL, 'C'},
        {"library-filename",    required_argument,  NULL, 'F'},
        {NULL, 0, NULL, 0}
    };

    int ch;

    application = argv[0];

    while ((ch = getopt_long(argc, argv, "hvC:F:", longopts, NULL)) != -1) {
        switch (ch) {
        case 'h':
            usage();
            exit(0);

        case 'v':
            if (verbose >= LOG_DEBUG[0]) {
                verbose--;
            }
            break;

        case 'C':
            working_directory = fs::absolute(optarg);
            break;

        case 'F':
            // Only take the filename part of a path. This allows a user to pass it a full path.
            library_filename = fs::path(optarg).filename();
            break;

        case ':':
            usage();
            exit(2);

        case '?':
            usage();
            exit(2);

        default:
            log(LOG_FATAL "Unknown result from getopt_long.");
        }
    }
    argc -= optind;
    argv += optind;

    // Parse the next non-option argument.
    if (argc) {
        target = argv[0];
        argc--;
        argv++;
    }

    // No more non-option argument allowed here.
    if (argc) {
        log(LOG_ERROR "Extra non option argument found '%s'.\n", argv[0]);
        usage();
        exit(2);
    }
}

}}
