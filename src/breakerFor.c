//
// Created by drayr on 12/11/2021.
//

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <crypt.h>
#include <omp.h>

#include "../includes/breakerFor.h"

int search_all(char * crypted, int length, char first_char, char last_char)
{
    int loop_size = last_char - first_char;
    int cryptlen = strlen(crypted);
    int max_iter = (int) powl(loop_size, length);
    char tab[length];
    tab[length] = '\0';

    //for(int i = 0; i < length; ++i) tab[i] = first_char;

    struct crypt_data data;

    int ret = -1;
    printf("max_iter = %lu \n", (unsigned long) max_iter);

    /**
     * Parallel region
     * Private:
     *         - data: struct crypt_data, uninitialized before the parallel region and only set by crypt_r
     *         - tab: char[length], uninitialized before the parallel region and set first to divide range of characters
     * Shared:
     *         - ret: int, -1 is considered as no value, modified by threads in case of the password has been found
     *         - crypted: char *, used by strncmp function to compare the expected and the actual crypted
     *         - cryptlen: int, used by strncmp function to compare the expect and the actual crypted
     * FirstPrivate:
     *         - max_iter: int, ensure the max iteration keeps its value and stay unmodified after the region
     *         - last_char: char, ensure the last char keeps its value and stay unmodified after the region
     *         - first_char: char, ensure the first char keeps its value and stay unmodified after the region
     *         - length: int, ensure the length keeps its value and stay unmodified after the region
     *         - loop_size: int, ensure the loop size keeps its value and stay unmodified after the region
     */
    #pragma omp parallel private(data, tab) shared(ret, crypted, cryptlen) \
                firstprivate(max_iter, last_char, first_char, length, loop_size) default(none)
    {
        // Fill tab by dividing the range of characters in function of total of threads
        for (int i = 0; i < length; ++i)
            tab[i] = (char) (first_char + (loop_size / omp_get_num_threads()) * omp_get_thread_num());

        // For parallelization, looping until the max iteration has been reached
        #pragma omp for
        for(int i = 0; i < max_iter; ++i)
        {
            // Usage of strncmp instead of strcmp since in some cases
            // strcmp can be not thread-safe
            // Usage of crypt_r(3) since crypt(3) is not thread-safe
            if(!strncmp(crypted, crypt_r(tab, "salt", &data), cryptlen))
            {
                printf("password found: %s\n", tab);
                // As we cannot return inside a region we set the return value
                ret = i;
                // Signal to all threads to cancel their task
                #pragma omp cancel for
            }

            // Force synchronization of the threads, check if a Cancel Signal has been sent (Optimization)
            #pragma omp cancellation point for
            ++tab[0];
            for(int j = 0; j < length - 1; ++j)
            {
                if(last_char == tab[j])
                {
                    tab[j] = first_char;
                    ++tab[j + 1];
                }
            }
            // Force synchronization of the threads, check if a Cancel Signal has been sent (Optimization)
            #pragma omp cancellation point for
        }
        // Cancel the parallel region by exiting the for region (Optimization)
        #pragma omp cancel parallel
    }
    return ret;
}