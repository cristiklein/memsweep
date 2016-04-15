#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void print_help(int argc, char **argv) {
        fprintf(stderr, "Usage: %s -s SIZE [-i ITERATIONS]\n", argv[0]);
        fprintf(stderr, "Sweap an memory area of size SIZE and report performance\n");
}

// parse an integer, optionally followed by suffixes k, m, g, t, p, e
size_t memparse(const char *s) {
        char *endptr;
        size_t result = strtoul(s, &endptr, 10);

        switch (*endptr) {
                case 'e':
                case 'E':
                        result *= 1000;
                case 'p':
                case 'P':
                        result *= 1000;
                case 't':
                case 'T':
                        result *= 1000;
                case 'g':
                case 'G':
                        result *= 1000;
                case 'm':
                case 'M':
                        result *= 1000;
                case 'k':
                case 'K':
                        result *= 1000;
        }

        return result;
}

int main(int argc, char **argv) {
        // parse command-line
        size_t iterations = -1; // i.e., maximum
        size_t size = 0;
        int opt;
        while ((opt = getopt(argc, argv, "hs:i:")) != -1) {
                switch (opt) {
                        case 'h':
                                print_help(argc, argv);
                                return 1;
                        case 's':
                                size = memparse(optarg);
                                break;
                        case 'i':
                                iterations = atoi(optarg);
                                break;
                        default:
                                print_help(argc, argv);
                                exit(EXIT_FAILURE);
                }
        }
        if (size == 0) {
                print_help(argc, argv);
                exit(EXIT_FAILURE);
        }

        volatile char *area_to_sweep = malloc(size);
        if (!area_to_sweep) {
                fprintf(stderr, "Could not allocate memory of size %lu\n", size);
                exit(EXIT_FAILURE);
        }

        size_t page_size = sysconf(_SC_PAGESIZE);
        printf("Sweaping %lu bytes, in %lu bytes increment, for %lu iterations\n", size, page_size, iterations);

        for (size_t iteration = 0; iteration < iterations; iteration++) {
                struct timespec start, end;
                clock_gettime(CLOCK_MONOTONIC, &start);
                for (volatile char *p = area_to_sweep; p < area_to_sweep + size; p += page_size)
                        *p = *p + iteration;
                clock_gettime(CLOCK_MONOTONIC, &end);

                time_t duration_in_ns = (end.tv_nsec - start.tv_nsec) + (end.tv_sec - start.tv_sec) * 1000000000;
                uint64_t pages = size / page_size;
                printf("[%lu.%06lu] Iteration %lu, average latency per page access %luns\n", end.tv_sec, end.tv_nsec / 1000, iteration, duration_in_ns / pages);
        }

        return EXIT_SUCCESS;
}
