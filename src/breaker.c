//
// Created by drayr on 12/11/2021.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <crypt.h>

#include "../includes/breaker.h"

unsigned long cmp = 0;

int carry(char * tab, int i, char first_char, char last_char)
{
    if(!i)
        return 0;
    else if(tab[i - 1] < last_char)
    {
        #pragma omp atomic
            ++tab[i - 1];
        return 1;
    }

    int x;
    tab[i - 1] = first_char;
    #pragma omp parallel
    {
        #pragma omp single nowait
            #pragma omp task shared(x)
                x = carry(tab, i - 1, first_char, last_char);
    }
    return x;
}


int search_one(char * crypted, char * tab)
{
    #pragma omp atomic
        cmp++;

    if(!strcmp(crypted, crypt(tab, "pepper")))
    {
        printf("password found: %s\n", tab);
        return 1;
    }
    return 0;
}


void search_all(char * crypted, int length, char first_char, char last_char)
{
    int i;
    char * tab = (char *) malloc(length + 1);

    #pragma omp for
        for(i=0; i < length; ++i) tab[i] = first_char;

    tab[length] = '\0';
    puts("let's break it...");

    int loop = !search_one(crypted, tab);
    i = length - 1;

    while(loop)
    {
        if(tab[i] < last_char)
        {
            ++tab[i];
            loop = !search_one(crypted, tab);
        }
        else
        {
            tab[i] = first_char;
            #pragma omp parallel shared(loop) if(loop)
                #pragma omp single
                    loop = carry(tab, i, first_char, last_char);
            if (loop) loop = !search_one(crypted, tab);
        }
    }
}