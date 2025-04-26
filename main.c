#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include "hangman.h"

int main() {

    srand(time(NULL));

    char secret[11];

    int result = get_word(secret);
    if (result) {
        return 1;
    }

    // printf("%s\n", secret);

    // printf("%d\n", is_word_guessed("secret", "aeiou"));
    // // prints: 0
    // printf("%d\n", is_word_guessed("hello", "aeihoul"));
    // // prints: 1

    char resultTwo[30];
    get_guessed_word("container", "arpstxgoieyu", resultTwo);
    // printf("%s\n", resultTwo);
    // // result = "_o_tai_er"

    char resultThree[30];
    get_available_letters("arpstxgoieyu", resultThree);
    // printf("%s\n", resultThree);
    // // result = "bcdfhjklmnqvwz"

    hangman(secret);

    return 0;
}