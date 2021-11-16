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
    tab[length]='\0';

    for(int i = 0; i < length; ++i) tab[i] = first_char;


    struct crypt_data data;

    int ret = -1;
    printf("max_iter = %lu \n", (unsigned long) max_iter);

    #pragma omp parallel for shared(tab, data, ret) \
                private(max_iter, crypted, last_char, first_char, length) default(none)
    for(int i = 0; i < max_iter; ++i)
    {
        if(!strcmp(crypted, crypt_r(tab, "salt", &data)))
        {
            printf("password found: %s\n", tab);
            ret = i;
        }

        #pragma omp atomic update
            ++tab[0];
        for(int j = 0; j < length - 1; ++j)
        {
            if(last_char == tab[j])
            {
                tab[j] = first_char;
                #pragma omp atomic update
                    ++tab[j + 1];
            }
        }
    }
    return 0;
}