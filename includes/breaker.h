//
// Created by drayr on 12/11/2021.
//

#ifndef PANDREAKER_BREAKER_H
#define PANDREAKER_BREAKER_H

extern unsigned long cmp;

int carry(char * tab, int i, char first_char, char last_char);
int search_one(char * crypted, char * tab);
void search_all(char * crypted, int length, char first_char, char last_char);

#endif //PANDREAKER_BREAKER_H
