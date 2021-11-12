#include <stdio.h>
#include <stdlib.h>
#include <crypt.h>
#include <string.h>
#include <time.h>

#include "../includes/breaker.h"

int main(int argc, char ** argv) {
    char * password;
    char first_char, last_char;
    clock_t t1, t2;

    if(argc == 1)
    {
        password = "A$4c";
        first_char = 32;
        last_char = 126;
        /* ---ASCII values---
         * special characters: 	32 to 47
         * numbers: 		48 to 57
         * special characters: 	58 to 64
         * letters uppercase: 	65 to 90
         * special characters: 	91 to 96
         * letters lowercase: 	97 to 122
         * special characters: 	123 to 126
         * */
    }
    else if(argc == 4)
    {
        password = argv[1];
        first_char = (char) atoi(argv[2]);
        last_char = (char) atoi(argv[3]);
    }
    else
    {
        puts("usage: breaker <password> <first_ch> <last_ch>");
        puts("default: breaker A$4c 32 126");
        puts("exemple to break the binary password 1101000:");
        puts("breaker 1101000 48 49");
        exit(0);
    }
    char * crypted0 = crypt(password, "pepper");
    char * crypted = (char*) malloc(strlen(crypted0) + 1);
    strcpy(crypted, crypted0);

    puts("*running parameters*");
    printf(" -password length:\t%lu digits\n", strlen(password));
    printf(" -digits:\t\tfrom -%c- to -%c-\n", first_char, last_char);
    printf(" -crypted to break:\t%s\n", crypted);

    t1 = clock();
    search_all(crypted, (int) strlen(password), first_char, last_char);
    t2 = clock();

    float period = (float) (t2-t1)/CLOCKS_PER_SEC;
    if(period < 60)
        printf("time: %.1fs \n", period);
    else
        printf("time: %.1fmin \n", period/60);
    printf("#tries: %lu\n", cmp);
    printf("=> efficiency: %.f tries/s\n", (float) cmp/period);

    return EXIT_SUCCESS;
}
