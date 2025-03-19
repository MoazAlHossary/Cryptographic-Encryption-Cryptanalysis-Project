#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


//finds the index of a character in the substitution table
int index_finder(char letter, char *sub_table) {
    for (int i = 0; i < 26; i++) { //loops through all 26 letters of the alphabet
        if (sub_table[i] == letter) { //finds the index of the letter from the substitution table
            return i;
        }
    }
    return -1;
}


//replaces a letter with its index equal letter from a table (sub_table) - Ceasar Cipher
char substitution(char letter, char *sub_table) {
    if (isalpha(letter)) { //only substitutes if the letter is an alphabet. (Avoids spaces, numbers, etc)
        int index = index_finder(letter, sub_table); //calls the index_finder function to get the index of the letter and assigns it to integer 'index' 
        return 'a' + index;  //converts the letter back to the original by adding its ASCII to that of the letter 'a'
    }
    return letter;  //if the character is not a letter, it returns it as is. 
}


//this function is a loop which shifts the next 5 alphabetic characters
void shift_char(char *text, int start, int shift) {
    int count = 0; //keeps count of how many characters have been shifted
    for (int i = start; text[i] != '\0' && count < 5; i++) { //the loop starts at the start index and continues until the string ends at the null terminator or having shifted 5 characters. 
        if (isalpha(text[i])) {  //this ensures that the loop skips non alphabetic characters
            text[i] = 'a' + (text[i] - 'a' - shift + 26) % 26; //this function gives the position of the letter relative to 'a' using subtraction, then shifts the character backwards, and adds 26 to ensure the result is always positive. then performs a modulo 26 to wrap around the alphabet, and adds the ASCII of 'a' to convert the result into a character. 
            count++;
        }
    }
}


void explain_decryption(char *ciphertext, char *plaintext, char *sub_table, int currentIndex, int shift_value) {
    char plainChar = plaintext[currentIndex]; //the character from the plaintext being reversed
    char substitutedChar = ciphertext[currentIndex]; //the character from the ciphertext


    printf("\n'%c' (from ciphertext) corresponds to '%c' in the substitution table with an index of %d\n", substitutedChar, plainChar, index_finder(substitutedChar, sub_table));
    printf("Shifting the next 5 characters back by %d positions (mod 26):\n", shift_value);


    //the below handles the shifting
    for (int i = currentIndex + 1; i < currentIndex + 6 && ciphertext[i] != '\0'; i++) { //loops through the next 5 characters in the ciphertext starting from currentIndex + 1. Stops if completes 5 char or null term. 
        if (isalpha(ciphertext[i])) { //checks if the character is an alphabetic character
            char original_letterr = ciphertext[i]; //stores the ciphertext character
            char shifted_letter = plaintext[i]; //stores the plaintext character
            int original_index = index_finder(original_letterr, sub_table); //finds the index of the original character
            printf("Character %c (index %d) shifted back by %d results in %c\n", original_letterr, original_index, shift_value, shifted_letter);
        }
    }
}


//this is the main decryption function which applies the ceasar cipher and the shifting algorithm. 
void decryption(char *ciphertext, char *sub_table, char *plaintext) {
    int length = strlen(ciphertext); //stores the length of the ciphertext in an integer varaible 'length'
    strcpy(plaintext, ciphertext);  //this copies the contents of ciphertext into plaintext so that later on, changes can be made to plaintext and ciphertext remains the same. 


    int i = 0;
    while (i < length) { //we loop through the ciphertext and:
        if (isalpha(ciphertext[i])) {  //ensure only alphabetic characters are decrypted


            plaintext[i] = substitution(ciphertext[i], sub_table); //we then substitute the current letter using the ceasar cipher table


            int shift_value = index_finder(ciphertext[i], sub_table); //find the index of the current letter by calling the index_finder function
            shift_char(plaintext, i + 1, shift_value); //shift the next 5 letters based on the value of the current letter


            // Explain each decryption step (including both substitution and shifts)
            explain_decryption(ciphertext, plaintext, sub_table, i, shift_value);


            i += 6;  //move to the next character after shifting 5 characters
        }
    }
}


// Function to load the key from the file "encryption_key.txt"
void load_key_from_file(char *sub_table) {
    FILE *key_file = fopen("encryption_key.txt", "r");
    if (key_file == NULL) {
        printf("Could not open encryption_key.txt for reading.\n");
        exit(1);
    }
    fgets(sub_table, 27, key_file);  // Read the substitution key from file
    fclose(key_file);
    printf("Key loaded from encryption_key.txt: %s\n", sub_table);
}


int main() {
    char sub_table[27]; //substitution table for the ceasar cipher
    char ciphertext[1000]; //this buffer holds the ciphertext from ciphertext.txt
    char plaintext[1000];  //this buffer holds the processed plaintext


    int choice;
    printf("Select decryption key method:\n");
    printf("1. Default key\n");
    printf("2. Random key (loaded from encryption_key.txt)\n");
    printf("3. User defined key\n");
    printf("Select encryption key method which was used for the encryption: ");
    scanf("%d", &choice);


    //depending on the users choices, the following function:
    if (choice == 1) {
        strcpy(sub_table, "hilwmkbdpcvazusjgrynqxofte"); //1. decrypts the ciphertext using the predefined sub table
    } else if (choice == 2) {
        load_key_from_file(sub_table); //decrypts from a randomly generated key stored in encryption_key.txt
    } else if (choice == 3) {
        printf("Enter the key used for encryption (26 unique letters): "); //allows the user to input the key which was used for encryption.
        scanf("%s", sub_table);
        if (strlen(sub_table) != 26) {
            printf("Error: The key must have exactly 26 letters!\n");
            return 1;
        }
    } else {
        printf("Invalid choice.\n");
        return 1;
    }


    FILE *ciphertext_file = fopen("ciphertext.txt", "r"); //this function opens ciphertext.txt with read persmissions
    fgets(ciphertext, sizeof(ciphertext), ciphertext_file);  //this reads from ciphertext.txt
    fclose(ciphertext_file);  //close ciphertext.txt


    char filtered_ciphertext[1000];
    int y = 0;
    for (int i = 0; i < strlen(ciphertext); i++) { //loops through the entire ciphertext
        if (isalpha(ciphertext[i])) { //checks if letter i is an alphabet
            filtered_ciphertext[y++] = ciphertext[i];  //stores ciphertext as lowercase ALPHABETS only to filtered_plaintext
        }
    }
    filtered_ciphertext[y] = '\0'; //terminating the string 


    decryption(filtered_ciphertext, sub_table, plaintext); //providing the 3 parameters for the encryption function. 1. what needs to be encrypted, 2. how letters should be substituted, and 3. where to store the result. 


    plaintext[strlen(ciphertext)] = '\0'; //assigns a null at the last position of the text, calculated using the length of the ciphertext.


    FILE *plaintext_file = fopen("plaintext_decrypted.txt", "w"); //open/create a plaintext_decrypted.txt file with write permissions
    fprintf(plaintext_file, "%s", plaintext);  //write the decrypted text to the file
    fclose(plaintext_file);  //close the  file


    printf("Success\n");


    return 0;
}
