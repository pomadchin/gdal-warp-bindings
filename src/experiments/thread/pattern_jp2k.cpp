/*
 * Copyright 2019 Azavea
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <random>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#include "../../bindings.h"
#include "../../locked_dataset.hpp"

// Strings
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"
const char *xres = "5";
const char *yres = "7";
char const *bad_uri = "HOPEFULLY_THERE_IS_NO_FILE_WITH_THIS_NAME";
char const *options[] = { };
#pragma GCC diagnostic pop

// Constants
constexpr int N = 1024;
constexpr int DIM = 1 << 8;
constexpr int BUFFERSIZE = DIM * DIM;
constexpr int ATTEMPTS = 1 << 20;
constexpr int COPIES = -4;

// Threads
int lg_steps = 12;
pthread_t threads[N];

// Cache size
int cache_size = 1 << 8;

// ANSI
// Reference: https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c
// Reference: https://en.wikipedia.org/wiki/ANSI_escape_code#Colors
#define ANSI_COLOR_RED "\x1b[31;1m"
#define ANSI_COLOR_GREEN "\x1b[32;1m"
#define ANSI_COLOR_YELLOW "\x1b[33;1m"
#define ANSI_COLOR_BLUE "\x1b[34;1m"
#define ANSI_COLOR_MAGENTA "\x1b[35;1m"
#define ANSI_COLOR_CYAN "\x1b[36;1m"
#define ANSI_COLOR_RESET "\x1b[0m"

void *reader(void *argv1)
{
    for (int k = 0; k < (1 << lg_steps); ++k)
    {
        uint64_t token;

        auto g = std::mt19937(std::random_device{}());
        auto dist = std::uniform_int_distribution<int>(0, 999);

        switch (dist(g))
        {
        case 0:
            token = 1; // Hopefully this is a bad token
            break;
        case 1:
            token = get_token(bad_uri, options); // Hopefully this is a bad filename
            break;
        default:
            token = get_token(static_cast<const char *>(argv1), options);
        }

        // fprintf(stdout, "argv1 = %s", static_cast<const char *>(argv1));

        int width = -1;
        int height = -1;
        get_width_height(token, locked_dataset::SOURCE, ATTEMPTS, COPIES, &width, &height);
        // fprintf(stdout, ANSI_COLOR_BLUE "witdth, height = %d, %d\n" ANSI_COLOR_RESET, width, height);

        constexpr int N = 1 << 10;
        char actual_chars[N];

        get_crs_wkt(token, locked_dataset::SOURCE, ATTEMPTS, COPIES, actual_chars, N);

        // fprintf(stdout, ANSI_COLOR_BLUE "src = %s\n" ANSI_COLOR_RESET, actual_chars);

        // { 0,0,255,255 }
        /** { 256,0,511,255 }
        { 512,0,767,255 }
        { 768,0,1023,255 }
        { 1024,0,1279,255 }
        { 1280,0,1535,255 }
        { 1536,0,1791,255 }
        { 1792,0,1829,255 }
        { 0,256,255,511 }
        { 256,256,511,511 }
        { 512,256,767,511 }
        { 768,256,1023,511 }
        { 1024,256,1279,511 }
        { 1280,256,1535,511 }
        { 1536,256,1791,511 }
        { 1792,256,1829,511 }
        { 0,512,255,767 }
        { 256,512,511,767 }
        { 512,512,767,767 }
        { 768,512,1023,767 }
        { 1024,512,1279,767 }
        { 1280,512,1535,767 }
        { 1536,512,1791,767 }
        { 1792,512,1829,767 }
        { 0,768,255,1023 }
        { 256,768,511,1023 }
        { 512,768,767,1023 }
        { 768,768,1023,1023 }
        { 1024,768,1279,1023 }
        { 1280,768,1535,1023 }
        { 1536,768,1791,1023 }
        { 1792,768,1829,1023 }
        { 0,1024,255,1279 }
        { 256,1024,511,1279 }
        { 512,1024,767,1279 }
        { 768,1024,1023,1279 }
        { 1024,1024,1279,1279 }
        { 1280,1024,1535,1279 }
        { 1536,1024,1791,1279 }
        { 1792,1024,1829,1279 }
        { 0,1280,255,1535 }
        { 256,1280,511,1535 }
        { 512,1280,767,1535 }
        { 768,1280,1023,1535 }
        { 1024,1280,1279,1535 }
        { 1280,1280,1535,1535 }
        { 1536,1280,1791,1535 }
        { 1792,1280,1829,1535 }
        { 0,1536,255,1791 }
        { 256,1536,511,1791 }
        { 512,1536,767,1791 }
        { 768,1536,1023,1791 }
        { 1024,1536,1279,1791 }
        { 1280,1536,1535,1791 }
        { 1536,1536,1791,1791 }
        { 1792,1536,1829,1791 }
        { 0,1792,255,1829 }
        { 256,1792,511,1829 }
        { 512,1792,767,1829 }
        { 768,1792,1023,1829 }
        { 1024,1792,1279,1829 }
        { 1280,1792,1535,1829 }
        { 1536,1792,1791,1829 }
        { 1792,1792,1829,1829 }*/

        int src_window[4] = {0, 0, 255, 255};
        int dst_window[2] = {255, 255};

        uint8_t *buf = new uint8_t[width * height];
    
        get_data(token, token % 2, ATTEMPTS, 0, COPIES, src_window, dst_window, 1, 1 /* GDT_Byte */, buf);
    }

    return nullptr;
}

bool keep_going = true;

int main(int argc, char **argv)
{
    int n = N;

    if (argc < 2)
    {
        exit(-1);
    }
    if (argc >= 3)
    {
        sscanf(argv[2], "%d", &lg_steps);
        fprintf(stderr, ANSI_COLOR_BLUE "lg_steps = %d\n" ANSI_COLOR_RESET, lg_steps);
    }
    if (argc >= 4)
    {
        sscanf(argv[3], "%d", &n);
        if (n > N || n < 0)
        {
            n = N;
        }
        fprintf(stderr, ANSI_COLOR_BLUE "n = %d\n" ANSI_COLOR_RESET, n);
    }
    if (argc >= 5)
    {
        sscanf(argv[4], "%d", &cache_size);
        if (cache_size < 0)
        {
            cache_size = 1 << 8;
        }
        fprintf(stderr, ANSI_COLOR_BLUE "cache_size = %d\n" ANSI_COLOR_RESET, cache_size);
    }

    // Setup

    init(cache_size);
    dup2(open("/dev/null", O_WRONLY), 2);

    for (int i = 0; i < n; ++i)
    {
        assert(pthread_create(&threads[i], nullptr, reader, argv[1]) == 0);
    }
    for (int i = 0; i < n; ++i)
    {
        assert(pthread_join(threads[i], nullptr) == 0);
        fprintf(stdout, ANSI_COLOR_MAGENTA "." ANSI_COLOR_RESET);
    }
    fprintf(stdout, "\n");

    deinit();

    return 0;
}
