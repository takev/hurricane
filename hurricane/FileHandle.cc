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
#include <exception>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "FileHandle.h"

namespace takevos {
namespace hurricane {

FileHandle::FileHandle(const fs::path &filename) : filename(filename), fd(-1), data(NULL), data_size(0)
{
}

FileHandle::~FileHandle()
{
    assert(fd == -1 && data == NULL && data_size == 0);
}

void FileHandle::open(void)
{
    struct stat         stats;

    if ((fd = ::open(filename.string().c_str(), O_RDONLY, 0)) == -1) {
        throw std::runtime_error("Cannot open file '" + filename.string() + "'.");
    }

    if (::fstat(fd, &stats) == -1) {
        close();
        throw std::runtime_error("Cannot stat file '" + filename.string() + "'.");
    }
    data_size = stats.st_size;

    if ((data = (const char *)::mmap(NULL, data_size, PROT_READ, MAP_FILE | MAP_SHARED, fd, 0)) == MAP_FAILED) {
        data = NULL; // This is different from MAP_FAILED.
        close();
        throw std::runtime_error("Cannot map file '" + filename.string() + "'.");
    }
}

void FileHandle::close(void)
{
    if (data != NULL) {
        if (::munmap((void *)data, data_size) != 0) {
            throw std::runtime_error("Cannot unmap file '" + filename.string() + "'.");
        }
        data = NULL;
        data_size = 0;
    }

    if (fd != -1) {
        if (::close(fd) == -1) {
            throw std::runtime_error("Cannot close file '" + filename.string() + "'.");
        }
        fd = -1;
    }
}


}}