#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>


//function which takes a letter and returns its index in the alphabet. (ex. a = 0, b = 1)
int index_finder(char letter) {
    return tolower(letter) - 'a';  //makes a letter lowercase and then subtracts from the ASCII value of the letter 'a' to find the index of the specific letter
}


//replaces a letter with its index equal letter from a table (sub_table) - Ceasar Cipher
char substitution(char letter, char *sub_table) {
    if (isalpha(letter)) {  //only substitutes if the letter is an alphabet. (Avoids spaces, numbers, etc)
        int index = index_finder(letter); //calls the index_finder function to get the index of the letter and assigns it to integer 'index' 
        return sub_table[index]; //then returns the corresponding letter from the substitution table. 
    }
    return letter;  //if the character is not a letter, it returns it as is. 
}


//this function is a loop which shifts the next 5 alphabetic characters
void shift_char(char *text, int start, int shift) {
    for (int i = start; i < start + 5 && text[i] != '\0'; i++) { //this is essentially saying, start at the letter located at 'start', move through each letters checking 5 characters from the start, but stop early if we reach the end of the text, and increment by one at each iteration of the loop.
        if (isalpha(text[i])) {  //this ensures that the loop skips non alphabetic characters
            text[i] = 'a' + (text[i] - 'a' + shift) % 26; //this shifts the letter while staying within the bounds of the alphabet by subtracting whatever letter were on from the ASCII value of 'a', and then performing a modulo 26 operation. 
        }
    }
}


void explain_encryption(char *plaintext, char *ciphertext, char *sub_table, int currentIndex, int shift_value) {
    char plainChar = plaintext[currentIndex]; //the character from the plaintext being processed
    char substitutedChar = ciphertext[currentIndex]; //the character from the ciphertext after substitution


    printf("\n'%c' (from plaintext) corresponds to '%c' in the substitution table with an index of %d\n", plainChar, substitutedChar, index_finder(plainChar));
    printf("\nShifting the next 5 characters by %d (mod 26):\n", shift_value);


    //the below handles the shifting
    for (int i = currentIndex + 1; i < currentIndex + 6 && ciphertext[i] != '\0'; i++) { //loops through the next 5 characters in the plaintext starting from currentIndex + 1. Stops if completes 5 char or null term. 
        if (isalpha(plaintext[i])) { //checks if the character is an alphabetic character
            char original_letter = plaintext[i]; //stores the plaintext character
            char shifted_letter = ciphertext[i]; //stores the shifted character
            int original_index = index_finder(original_letter); //finds the index of the original character
            printf("Character '%c' (index %d) shifted by %d results in '%c'\n", original_letter, original_index, shift_value, shifted_letter);
        }
    }
}


//this is the main encryption function which applies the ceasar cipher and the shifting algorithm. 
void encryption(char *plaintext, char *sub_table, char *ciphertext) {
    int length = strlen(plaintext); //stores the length of the plaintext in an integer varaible 'length'
    strcpy(ciphertext, plaintext);  //this copies the contents of plaintext into ciphertext so that later on, changes can be made to ciphertext and plaintext remains the same. 


    int i = 0;
    while (i < length) { //we loop through the plaintext and:
        if (isalpha(plaintext[i])) {  //ensure only alphabetic characters are encrypted
            char lower_character = tolower(plaintext[i]); //all letters are lowercased


            ciphertext[i] = substitution(lower_character, sub_table); //we then substitute the current letter using the ceasar cipher table


            int shift_value = index_finder(lower_character); //find the index of the current letter by calling the index_finder function


            shift_char(ciphertext, i + 1, shift_value);  //shift the next 5 letters based on the value of the current letter


            explain_encryption(plaintext, ciphertext, sub_table, i, shift_value); //calls the encryption explainer function with the plaintext, the ciphertext, the substitution table being used, the current letter, and the shift value


            i += 6; //move to the next character after shifting 5 characters
        }
    }
}


void generate_random_key(char *sub_table) {
    char alphabet[] = "abcdefghijklmnopqrstuvwxyz"; //given the english alphabet
    int len = strlen(alphabet); //creates a varaible of 26 characters


    srand(time(0)); //by using the time in seconds, we are feeding a different seed into the algorithm each time the code is run. This seed will never be the same because the time is calculated in seconds since January 1st, 1970!
    //below is a fisher yates shuffle algorithm.
    for (int i = len - 1; i > 0; i--) { //starts from the last letter in the alphabet and moves backwards to ensure the character has not been swapped yet.
        int j = rand() % (i + 1); //rand() generates a random number based off the previous seed. '% (i + 1) limits the random number to a value between 0 and i.
        char temp = alphabet[i]; //the letter just chosed by the previous function is stored temporarily
        alphabet[i] = alphabet[j]; //we will move the character from its original place (i), to its new assigned place (j).
        alphabet[j] = temp; //and then move the character to the position of j
    }
    
    strcpy(sub_table, alphabet);


    FILE *key_file = fopen("encryption_key.txt", "w"); //creates/opens a new file encryption_key.txt with write permissions 
    fprintf(key_file, "%s", sub_table); //writes the randomly generated key
    fclose(key_file); //closes the file
}


int main() {
    char sub_table[27];  //substitution table for the ceasar cipher
    char plaintext[1000];  //this buffer holds the plaintext from plaintext.txt
    char ciphertext[1000];  //this buffer holds the processed ciphertext


    int choice;
    printf("1. Default key\n");
    printf("2. Randomly generated key (saved to encryption_key.txt)\n");
    printf("3. User defined key\n");
    printf("Select encryption key method: ");
    scanf("%d", &choice);


    //depending on the users choices, the following function:
    if (choice == 1) { 
        strcpy(sub_table, "hilwmkbdpcvazusjgrynqxofte"); //1. copies the predefined sub table into sub_table
    } else if (choice == 2) {
        generate_random_key(sub_table); //2. or, generates a randomly generated key
    } else if (choice == 3) {
        printf("enter a custom key (26 characters): "); //3. or, allows the user to enter their own key. 
        scanf("%s", sub_table);
        if (strlen(sub_table) != 26) { //checks if the user defined key is 26 characters. raises an error if not. 
            printf("key must be 26 characters!\n");
            return 1;
        }
    } else {
        printf("invalid choice.\n");
        return 1;
    }


    FILE *plaintext_file = fopen("plaintext.txt", "r"); //this function opens plaintext.txt with read persmissions


    fgets(plaintext, sizeof(plaintext), plaintext_file);  //this reads from plaintext.txt
    fclose(plaintext_file);  //close plaintext.txt


    char filtered_plaintext[1000];
    int y = 0;
    
    for (int i = 0; i < strlen(plaintext); i++) { //loops through the entire plaintext
        if (isalpha(plaintext[i])) { //checks if letter i is an alphabet
            filtered_plaintext[y++] = tolower(plaintext[i]);  //stores plaintext as lowercase ALPHABETS only to filtered_plaintext
        }
    }
    filtered_plaintext[y] = '\0'; //terminating the string 


    encryption(filtered_plaintext, sub_table, ciphertext); //providing the 3 parameters for the encryption function. 1. what needs to be encrypted, 2. how letters should be substituted, and 3. where to store the result. 


    ciphertext[strlen(plaintext)] = '\0'; //assigns a null at the last position of the text, calculated using the length of the plaintext.


    FILE *ciphertext_file = fopen("ciphertext.txt", "w"); //open/create a ciphertext.txt file with write permissions
    
    fprintf(ciphertext_file, "%s", ciphertext); //write the encrypted text to the file
    fclose(ciphertext_file); //close the  file


    printf("Success.\n");


    return 0;
}
