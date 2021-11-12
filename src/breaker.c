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
    if(!i) return 0;
    else if(tab[i - 1] < last_char)
    {
        tab[i - 1] = (char) (tab[i - 1] + 1);
        return 1;
    }

    tab[i - 1] = first_char;
    return carry(tab, i - 1, first_char, last_char);
}


int search_one(char * crypted, char * tab)
{
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
    int i, loop = 1;
    char * tab = (char *) malloc(length + 1);
    for(i=0; i < length; i++) tab[i] = first_char;

    tab[length] = '\0';
    puts("let's break it...");
    loop = !search_one(crypted, tab);
    i = length - 1;
    while(loop)
    {
        if(tab[i] < last_char)
        {
            tab[i] = (char) (tab[i] + 1);
            loop = !search_one(crypted, tab);
        }
        else
        {
            tab[i] = first_char;
            if((loop = carry(tab, i, first_char, last_char)))
                loop = !search_one(crypted, tab);
        }
    }
}