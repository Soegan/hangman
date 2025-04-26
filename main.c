#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include "hangman.h"

int main() {

    srand(time(NULL));

    char secret[11];

    int resultWord = get_word(secret);
    if (resultWord) {
        perror("Cannot get secret word for some reason");
        return 1;
    }

    hangman(secret);

    return 0;
}