#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "hangman.h"


int get_word(char secret[]){
    // check if file exists first and is readable
    FILE *fp = fopen(WORDLIST_FILENAME, "rb");
    if( fp == NULL ){
        fprintf(stderr, "No such file or directory: %s\n", WORDLIST_FILENAME);
        return 1;
    }

    // get the filesize first
    struct stat st;
    stat(WORDLIST_FILENAME, &st);
    long int size = st.st_size;

    do{
        // generate random number between 0 and filesize
        long int random = (rand() % size) + 1;
        // seek to the random position of file
        fseek(fp, random, SEEK_SET);
        // get next word in row ;)
        int result = fscanf(fp, "%*s %20s", secret);
        if( result != EOF )
            break;
    }while(1);

    fclose(fp);

    return 0;
}

int is_word_guessed(const char secret[], const char letters_guessed[]) {
    int secretLen = strlen(secret);
    int lettersLen = strlen(letters_guessed);
    int notFoundFlag;
    
    for (int i = 0; i < secretLen; i++) {
        notFoundFlag = 1;
        for (int j = 0; j < lettersLen; j++) {
            if (letters_guessed[j] == secret[i]) {
                notFoundFlag = 0;
                break;
            }
        }
        if (notFoundFlag) {
            return 0;
        }
    }
    
    return 1;
}

void get_guessed_word(const char secret[], const char letters_guessed[], char guessed_word[]) {
    int i, j;
    int len = strlen(secret);
    int guessed;

    for (i = 0; i < len; i++) {
        guessed = 0;
        for (j = 0; letters_guessed[j] != '\0'; j++) {
            if (secret[i] == letters_guessed[j]) {
                guessed = 1;
                break;
            }
        }
        if (guessed) {
            guessed_word[i] = secret[i];
        } else {
            guessed_word[i] = '_';
        }
    }
    guessed_word[len] = '\0';
}


void get_available_letters(const char letters_guessed[], char available_letters[]) {
    
    char alphabet[27] = "abcdefghijklmnopqrstuvwxyz";
    int lgLen = strlen(letters_guessed);
    int foundFlag;
    int x = 0;

    for (int i = 0; i < 26; i++) {
        foundFlag = 0;
        for (int j = 0; j < lgLen; j++) {
            if (letters_guessed[j] == alphabet[i]) {
                foundFlag = 1;
                break;
            }
        }
        if (!foundFlag) {
            available_letters[x] = alphabet[i];
            x++;
        }
    }
    available_letters[x] = '\0';
}

int letter_in_word (const char string[], const char symbol) {
    for (int i = 0; i < strlen(string); i++) {
        if (symbol == string[i]) {
            return 1;
        }
    }
    return 0;
}

int string_compare(const char str1[], const char str2[], int strLen) {
    for (int i = 0; i < strLen; i++) {
        if (str1[i] != str2[i]) {
            return 0;
        }
    }
    return 1;
}


void hangman(const char secret[]) {

    int secretLen = strlen(secret);
    int attempts = 8;
    char letters_guessed[27] = "";
    int letters_guessed_index = 0;
    char available_letters[27];
    char input[255] = "";
    char guessed_word[secretLen];

    printf("Welcome to the game, Hangman!\n");
    printf("I am thinking of a word that is %d letters long.\n", secretLen);

    get_guessed_word(secret, letters_guessed, guessed_word);

    while (attempts > 0 && !is_word_guessed(secret, letters_guessed)) {
        printf("-------------\n");
        printf("You have %d guesses left.\n", attempts);

        get_available_letters(letters_guessed, available_letters);
        printf("Available letters: %s\n", available_letters);
        
        printf("Please guess a letter: ");
        fgets(input, 255, stdin);
        
        int inputLen = strlen(input) - 1;

        if (inputLen > 1) {
            // string
            if (inputLen != secretLen) {
                printf("Sorry, bad guess. The word was goal.\n");
                attempts = 0;
                break;
            }
            if (string_compare(secret, input, secretLen)) {
                printf("Congratulations, you won!\n");
                break;
            } else {
                printf("Sorry, bad guess. The word was goal.\n");
                attempts = 0;
                break;
            }
        } else {
            // char
            char symbol = input[0];
            if (symbol >= 65 && symbol <= 90) {
                symbol += 32;
            }
            if (symbol < 97 || symbol > 122) {
                printf("Wrong symbol!\n");
                continue;
            }
 
            if (letter_in_word(letters_guessed, symbol)) {
                printf("Oops! You've already guessed that letter: ");
            } else {
                letters_guessed[letters_guessed_index++] = symbol;
                if (letter_in_word(secret, symbol)) {
                    printf("Good guess: ");
                    get_guessed_word(secret, letters_guessed, guessed_word);
                } else {
                    printf("Oops! That letter is not in my word: ");
                    attempts--;
            }
            printf("%s\n", guessed_word);
            }
        }

        if (attempts == 0) {
            printf("-------------\n");
            printf("Sorry, you ran out of guesses. The word was %s.\n", secret);
        }
        if (is_word_guessed(secret, letters_guessed)) {
            printf("-------------\n");
            printf("%s, %s\n", secret, letters_guessed);
            printf("Congratulations, you won!\n");
        }

    printf("-------------\n");
    }
}