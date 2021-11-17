//
// Created by drayr on 12/11/2021.
//

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <crypt.h>

#include "../includes/breakerFor.h"

int search_all(char * crypted, int length, char first_char, char last_char)
{
    int loop_size = last_char - first_char;
    int cryptlen = strlen(crypted);
    int max_iter = (int) powl(loop_size, length);
    char tab[length];
    tab[length] = '\0';

    for(int i = 0; i < length; ++i) tab[i] = first_char;


    struct crypt_data data;

    int ret = -1;
    printf("max_iter = %lu \n", (unsigned long) max_iter);

    #pragma omp parallel private(data) shared(ret, tab, crypted, cryptlen) \
                firstprivate(max_iter, last_char, first_char, length) default(none)
    {
        #pragma omp for
        for(int i = 0; i < max_iter; ++i)
        {
            /*if (ret != -1)
            {
                #pragma omp cancel for
            }*/

            /*#pragma omp parallel shared(ret) \
                        firstprivate(tab, cryptlen, crypted, i) \
                        private(data) default(none)
            #pragma omp single*/
            //char * encrypted = crypt_r(tab, "salt", &data);
            if(!strncmp(crypted, crypt_r(tab, "salt", &data), cryptlen))
            {
                printf("password found: %s\n", tab);
                ret = i;
                #pragma omp cancel for
            }

            #pragma omp cancellation point for
                #pragma omp atomic
                    ++tab[0];
                for(int j = 0; j < length - 1; ++j)
                {
                    if(last_char == tab[j])
                    {
                        tab[j] = first_char;
                        #pragma omp atomic
                            ++tab[j + 1];
                    }
                }
            #pragma omp cancellation point for
        }
        #pragma omp cancel parallel
    }
    return ret;
}