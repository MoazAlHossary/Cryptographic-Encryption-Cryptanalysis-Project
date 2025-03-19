#include <stdio.h> //standard input/output library
#include <stdlib.h> //memory allocation
#include <string.h> //string manipulation
#include <ctype.h> //letter type


#define BLOCK_SIZE 6 //size of the blocks in crypto alg.
#define ALPHABET_SIZE 26 //number of letters in the english alphabet
#define MAX_COMBINATIONS 1000 //maximum number of word combinations or guesses the program will process
#define MAX_WORD_LENGTH 100 //allocating memory for a word from the dictionary
#define DICTIONARY_SIZE 15000 //maximum number of words the dictionary can hold


int MAX_OPTIONS;
int MAX_DISTANCE;
int SLIDING_BLOCK_SIZE;


char *dictionary[DICTIONARY_SIZE]; //array of pointers to hold words from the dictionary


typedef struct {
    char combination[4]; //holds either a bigram
    int frequency; //stores the frequency of the combination in the text
} Combination;


//function declarations
void load_dictionary(const char *filename);//reads a list of words from a file (dictionary.txt) and loads them into a global dictionary array
void generate_options(char *freq_analys_block, int start, char *options[], int distances[], int *num_options); //extracts a segment of text, finds similar words from the dictionary using the levenshtein distance, generates, displays options for the user
void calculateFrequencyPercentages(int frequency[], double percentages[], int totalChars); //calculates the percentage frequency of each initial letter in a cipher block
void sortByFrequency(double percentages[], int frequency[], char english_letters[]); //sorts letters, frequencies, and their corresponding percentages in descending order
char caesarShiftBack(char ch, int shift);//performs a backward caesar shift on a letter
void initializeCombinations(Combination combinations[ALPHABET_SIZE][MAX_COMBINATIONS], int counts[ALPHABET_SIZE]);//initializes an array of combinations for bigrams/trigrams
void analyzeBlocks(char *text, long textLength, Combination bigrams[ALPHABET_SIZE][MAX_COMBINATIONS], int bigramCounts[ALPHABET_SIZE], Combination trigrams[ALPHABET_SIZE][MAX_COMBINATIONS], int trigramCounts[ALPHABET_SIZE]);
void sortCombinationsByFrequency(Combination *combinations, int count);
void writeAnalysisToFile(Combination bigrams[ALPHABET_SIZE][MAX_COMBINATIONS], int bigramCounts[ALPHABET_SIZE], Combination trigrams[ALPHABET_SIZE][MAX_COMBINATIONS], int trigramCounts[ALPHABET_SIZE]);


//calculate the levenshtein distance between two strings
int levenshtein_distance(const char *s1, const char *s2) {
    //calculate the lengths of the two strings
    int len1 = strlen(s1); //ex. 'kitten' is 6
    int len2 = strlen(s2); //ex. 'sitting' is 7
    int dist_arr[len1 + 1][len2 + 1]; //array to store distances. ex. dist_arr[7][8]


//structure of dist_arr is:
//''  s  i  t  t  i  n  g
//''  0  1  2  3  4  5  6  7
//k   1  -  -  -  -  -  -  -
//i   2  -  -  -  -  -  -  -
//t   3  -  -  -  -  -  -  -
//t   4  -  -  -  -  -  -  -
//e   5  -  -  -  -  -  -  -
//n   6  -  -  -  -  -  -  -
//the first row and column are used to represent the cost of transforming between an empty string and each prefix of s1 and s2


    //initialize the array using nested loops (loop inside a loop)
    for (int i = 0; i <= len1; i++) { //loop over the rows, which represent letters in s1
        for (int j = 0; j <= len2; j++) { //loop over the columns, which represent letters in s2
            if (i == 0) //if we are in the first row
                dist_arr[i][j] = j; //set the cost to the column index
            else if (j == 0) //if we are in the first column
                dist_arr[i][j] = i; //set the cost to the row index
            else if (s1[i - 1] == s2[j - 1]) //if the letters in s1 and s2 match
                dist_arr[i][j] = dist_arr[i - 1][j - 1]; //no cost to align these letters, set the diagonal value
            else { //if the characters in s1 and s2 dont match
                dist_arr[i][j] = 1 //start with 1 to account for this mismatch
                    + (dist_arr[i - 1][j] < dist_arr[i][j - 1] 
                        ? //if the deletion cost is less than the insertion cost
                            (dist_arr[i - 1][j] < dist_arr[i - 1][j - 1] 
                                ? dist_arr[i - 1][j] //choose deletion if its also less than substitution
                                : dist_arr[i - 1][j - 1]) //otherwise choose substitution
                        : //if the insertion cost is less than or equal to the deletion cost
                            (dist_arr[i][j - 1] < dist_arr[i - 1][j - 1] 
                                ? dist_arr[i][j - 1] //choose insertion if its also less than substitution
                                : dist_arr[i - 1][j - 1]) //otherwise choose substitution
                    );
            }
 //the above line calculates the minimum of three possible operations:
 //1. deletion (dist_arr[i - 1][j])
 //2. insertion (dist_arr[i][j - 1])
 //3. substitution (dist_arr[i - 1][j - 1])
 //the result is incremented by 1 to account for the current operation cost
        }
    }
    return dist_arr[len1][len2]; //return the levenshtein distance which is the bottom right value in dist_arr


//resulting dist_arr:
//0  1  2  3  4  5  6  7
//0  0  1  2  3  4  5  6  7
//1  1  1  2  3  4  5  6  7
//2  2  2  1  2  3  4  5  6
//3  3  3  2  1  2  3  4  5
//4  4  4  3  2  1  2  3  4
//5  5  5  4  3  2  2  3  4
//6  6  6  5  4  3  3  2  3 <---


}


//load the dictionary
void load_dictionary(const char *filename) {
    FILE *file = fopen(filename, "r"); //open the dictionary file in read mode
    if (!file) { //check if the file was successfully opened
        perror("Failed to open dictionary file"); //print an error message if the file couldnt be opened
    }


    char word[MAX_WORD_LENGTH]; //declare a letter array to store each word read from the file
    int index = 0; //initialize an index to keep track of the number of words loaded into the dictionary


    //read each line from the file until the end of the file is reached or the dictionary is full
    while (fgets(word, sizeof(word), file) && index < DICTIONARY_SIZE) {
        //remove the newline letter at the end of the word
        word[strcspn(word, "\n")] = '\0';
        dictionary[index] = strdup(word); //duplicate the word and store it in the dictionary array
        index++; //increment the index to move to the next position in the dictionary
    }


    fclose(file); //close the file after all words have been read and stored
}


//generate and display options for the user, sorted by distance using the levenshtein distance function
void generate_options(char *freq_analys_block, int start, char *options[], int distances[], int *num_options) {
    char segment[SLIDING_BLOCK_SIZE + 1]; //letter array to hold a segment of text from freq_analys_block
    *num_options = 0; //initialize the number of options to 0


    //extract the current segment from the freq_analys_block
    strncpy(segment, freq_analys_block + start, SLIDING_BLOCK_SIZE); //copy a segment of size SLIDING_BLOCK_SIZE from freq_analys_block starting at 'start' position.
    segment[SLIDING_BLOCK_SIZE] = '\0'; //null-terminate the segment to ensure it is a valid string


    //print the current segment
    printf("\nCurrent segment: %s\n", segment);
    printf("Options for the next segment of freq_analys_block:\n");


    //use levenshtein distance to find similar words
    for (int i = 0; i < DICTIONARY_SIZE && dictionary[i] != NULL; i++) {//iterate over each word in the dictionary
        int distance = levenshtein_distance(segment, dictionary[i]);//calculate the levenshtein distance between the segment and the dictionary word
        if (distance <= MAX_DISTANCE) {//check if the distance is acceptable (MAX_DISTANCE)
            options[*num_options] = dictionary[i];//store the matching word in the options array
            distances[*num_options] = distance;//store the distance in the distances array
            (*num_options)++;//increment the number of options
            if (*num_options >= MAX_OPTIONS) {//check if the maximum number of options has been reached
                break; //stop adding more options
            }
        }
    }


   //sort the options and distances in ascending order of distances using nested loops
    for (int i = 0; i < *num_options - 1; i++) {//pass through the entire array
        for (int j = 0; j < *num_options - i - 1; j++) {//compare each pair of adjacent elements
            if (distances[j] > distances[j + 1]) {//compare adjacent elements in the distances array
                //swap distances if they are out of order
                int temp_distance = distances[j];//store the current distance in a temporary variable
                distances[j] = distances[j + 1];//move the next distance into the current position
                distances[j + 1] = temp_distance;//move the temporary distance into the next position


                //swap the corresponding options to keep the arrays in sync
                char *temp_option = options[j];//store the current option in a temporary variable
                options[j] = options[j + 1];//move the next option into the current position
                options[j + 1] = temp_option;//move the temporary option into the next position
            }
        }
    }


    //check if there are no valid options
    if (*num_options == 0) {
        printf("No valid words found. Would you like to:\n"); 
        printf("1. Reassess MAX_OPTIONS\n"); //option 1: allow the user to change the maximum number of options displayed
        printf("2. Reassess MAX_DISTANCE\n"); //option 2: allow the user to change the maximum allowable levenshtein distance
        printf("3. Reassess SLIDING_BLOCK_SIZE\n"); //option 3: allow the user to change the size of the sliding block
        printf("4. Go to main menu (write word manually)\n"); //option 4: give the user an option to return to the main menu
        printf("Enter your choice (1-4): "); //prompt the user to make a choice


        int user_choice; // Variable to store the user's choice
        scanf("%d", &user_choice); // Read the user's input


        switch (user_choice) {
            case 1: //if the user chose to reassess MAX_OPTIONS
                printf("Enter new value for MAX_OPTIONS: ");
                scanf("%d", &MAX_OPTIONS); 
                break;
            case 2: //if the user chose to reassess MAX_DISTANCE
                printf("Enter new value for MAX_DISTANCE: ");
                scanf("%d", &MAX_DISTANCE);
                break;
            case 3: //if the user chose to reassess SLIDING_BLOCK_SIZE
                printf("Enter new value for SLIDING_BLOCK_SIZE: ");
                scanf("%d", &SLIDING_BLOCK_SIZE);
                break;
            default: //do nothing
                break;
        }
    } else { //if there are valid word options available
        //loop through and display each word option with its levenshtein distance
        for (int i = 0; i < *num_options; i++) {
            printf("[%d] %s (distance: %d)\n", i + 1, options[i], distances[i]); //print the option number, word, and distance
        }
    }
}


//sort combinations by frequency in descending order using a nested loop
void sortCombinationsByFrequency(Combination *combinations, int count) {
    //outer loop to iterate over each combination
    for (int i = 0; i < count - 1; i++) {
        //inner loop to compare the current combination with the subsequent ones
        for (int j = i + 1; j < count; j++) {
            //if the frequency of the current combination is less than the frequency of the next combination
            if (combinations[i].frequency < combinations[j].frequency) {
                //swap the two combinations to sort them in descending order
                Combination temp = combinations[i];//store the current combination in a temporary variable
                combinations[i] = combinations[j];//move the next combination into the current position
                combinations[j] = temp;//move the temporary combination into the next position
            }
        }
    }
}


//initialize combination arrays
void initializeCombinations(Combination combinations[ALPHABET_SIZE][MAX_COMBINATIONS], int counts[ALPHABET_SIZE]) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        counts[i] = 0;
        for (int j = 0; j < MAX_COMBINATIONS; j++) {
            combinations[i][j].combination[0] = '\0';
            combinations[i][j].frequency = 0;
        }
    }
}


//analyze bigrams and trigrams for each initial letter
void analyzeBlocks(char *text, long textLength, Combination bigrams[ALPHABET_SIZE][MAX_COMBINATIONS], int bigramCounts[ALPHABET_SIZE], Combination trigrams[ALPHABET_SIZE][MAX_COMBINATIONS], int trigramCounts[ALPHABET_SIZE]) {
    for (long i = 0; i < textLength; i += BLOCK_SIZE) { //iterate through the text in size of BLOCK_SIZE letters
        char initial = text[i]; //get the initial letter of the current block


        int index = initial - 'a'; //calculate the index for the initial letter (0 for 'a', 25 for 'z')


        //analyze bigrams within the block
        for (int j = 0; j < BLOCK_SIZE - 1 && i + j + 1 < textLength; j++) { //this loop ensures that you dont go past the length of the text without going out of bounds
            
                char bigram[3] = {text[i + j], text[i + j + 1], '\0'}; //create a bigram from two consecutive letters and convert to lowercase
                int found = 0; //flag to indicate if the bigram was found


                //check if the bigram is already in the list for this initial letter
                for (int k = 0; k < bigramCounts[index]; k++) {
                    if (strcmp(bigrams[index][k].combination, bigram) == 0) { //compare the current bigram with existing ones
                        bigrams[index][k].frequency++; //increment the frequency if found
                        found = 1; //set the flag to indicate that the bigram was found
                        break; //exit the loop as the bigram has been processed
                    }
                }


                //if the bigram was not found, add it to the list
                if (!found && bigramCounts[index] < MAX_COMBINATIONS) {
                    strcpy(bigrams[index][bigramCounts[index]].combination, bigram); //copy the bigram into the array
                    bigrams[index][bigramCounts[index]].frequency = 1; //set the initial frequency to 1
                    bigramCounts[index]++; //increment the count of bigrams for this letter
                }
            }


        //analyze trigrams within the block
        for (int j = 0; j < BLOCK_SIZE - 2 && i + j + 2 < textLength; j++) {
                //create a trigram from three consecutive characters and convert to lowercase
                char trigram[4] = {text[i + j], text[i + j + 1], text[i + j + 2], '\0'};
                int found = 0; //flag to indicate if the trigram was found


                //check if the trigram is already in the list for this initial letter
                for (int k = 0; k < trigramCounts[index]; k++) {
                    if (strcmp(trigrams[index][k].combination, trigram) == 0) { //compare the current trigram with existing ones
                        trigrams[index][k].frequency++; //increment the frequency if found
                        found = 1; //set the flag to indicate that the trigram was found
                        break; //exit the loop as the trigram has been processed
                    }
                }


                //if the trigram was not found, add it to the list
                if (!found && trigramCounts[index] < MAX_COMBINATIONS) {
                    strcpy(trigrams[index][trigramCounts[index]].combination, trigram); //copy the trigram into the array
                    trigrams[index][trigramCounts[index]].frequency = 1; //set the initial frequency to 1
                    trigramCounts[index]++; //increment the count of trigrams for this letter
                }
            }
        }
    }


//write bigram and trigram analysis to a file
void writeAnalysisToFile(Combination bigrams[ALPHABET_SIZE][MAX_COMBINATIONS], int bigramCounts[ALPHABET_SIZE], Combination trigrams[ALPHABET_SIZE][MAX_COMBINATIONS], int trigramCounts[ALPHABET_SIZE]) {
    //arrays containing bigrams/trigrams we want to include in the search
    const char *bigramMap[] = {"th", "he", "in", "er", "an", "re", "nd", "on", "en", "at",
                               "ou", "ed", "ha", "to", "or", "it", "is", "hi", "es", "ng"};
    const char *trigramMap[] = {"the", "and", "ing", "her", "hat", "his", "tha", "ere", "for", "ent",
                                "ion", "ter", "was", "you", "ith", "ver", "all", "wit", "thi", "tio"};


    //open a file for writing the analysis results
    FILE *file = fopen("bigrams_and_trigrams.txt", "w");
    if (file == NULL) { //check if the file was successfully created
        perror("Error creating analysis file"); //print an error message if the file cannot be opened
        return; //exit the function early
    }


    //iterate over each letter of the alphabet
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        //only process letters that have bigram or trigram counts
        if (bigramCounts[i] > 0 || trigramCounts[i] > 0) {
            fprintf(file, "Analysis for blocks starting with '%c':\n", 'a' + i); //write the initial letter
            fprintf(file, "-------------------------------------------------\n");


            //sort and write bigrams with a frequency of at least 5
            if (bigramCounts[i] > 0) {
                sortCombinationsByFrequency(bigrams[i], bigramCounts[i]); //sort the bigrams by frequency
                fprintf(file, "  Bigrams:\n");
                fprintf(file, "  %-10s | %-10s | %-10s\n", "Bigram", "Frequency", "Mapped To");
                fprintf(file, "  -------------------------------------------\n");


                int printedBigrams = 0; //counter to track how many bigrams are printed
                for (int j = 0; j < bigramCounts[i] && j < 20; j++) { //limit to 20 bigrams
                    if (bigrams[i][j].frequency >= 5) { //only print bigrams with frequency >= 5
                        fprintf(file, "  %-10s | %-10d | %-10s\n", bigrams[i][j].combination, bigrams[i][j].frequency, bigramMap[j]); //print the bigram and its frequency
                        printedBigrams++;
                    }
                }
                if (printedBigrams == 0) { //if no bigrams with sufficient frequency are found
                    fprintf(file, "  None found with frequency >= 5\n");
                }
            }


            //sort and write trigrams with a frequency of at least 5
            if (trigramCounts[i] > 0) {
                sortCombinationsByFrequency(trigrams[i], trigramCounts[i]); //sort the trigrams by frequency
                fprintf(file, "\n  Trigrams:\n");
                fprintf(file, "  %-10s | %-10s | %-10s\n", "Trigram", "Frequency", "Mapped To");
                fprintf(file, "  -------------------------------------------\n");


                int printedTrigrams = 0; //counter to track how many trigrams are printed
                for (int j = 0; j < trigramCounts[i] && j < 20; j++) { //limit to 20 trigrams
                    if (trigrams[i][j].frequency >= 5) { //only print trigrams with frequency >= 5
                        fprintf(file, "  %-10s | %-10d | %-10s\n", trigrams[i][j].combination, trigrams[i][j].frequency, trigramMap[j]); //print the trigram and its frequency
                        printedTrigrams++;
                    }
                }
                if (printedTrigrams == 0) { //if no trigrams with sufficient frequency are found
                    fprintf(file, "  None found with frequency >= 5\n");
                }
            }


            fprintf(file, "\n-------------------------------------------------\n\n"); //separator between sections
        }
    }


    fclose(file); //close the file after writing
}


//calculate frequency percentages
void calculateFrequencyPercentages(int *frequency, double *percentages, int totalChars) {
    //loop over each letter of the alphabet
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        //calculate the frequency percentage for each letter
        percentages[i] = (frequency[i] / (double)totalChars) * 100.0;
        //frequency[i] is the count of the letter
        //totalChars is the total number of characters in the text
        //percentages[i] is the frequency of the letter as a percentage of the total characters
    }
}


//sort characters by their frequency percentages in descending order
void sortByFrequency(double *percentages, int *frequency, char *english_letters) {
    //use a simple Bubble Sort to sort the percentages, frequencies, and letters in descending order
    for (int i = 0; i < ALPHABET_SIZE - 1; i++) { //outer loop iterates over each letter (except the last one)
        for (int j = i + 1; j < ALPHABET_SIZE; j++) { //inner loop compares the current letter with subsequent letters
            //check if the current percentage is less than the next percentage
            if (percentages[i] < percentages[j]) {
                //swap percentages to arrange them in descending order
                double tempPercent = percentages[i]; //store the current percentage in a temporary variable
                percentages[i] = percentages[j]; //move the larger percentage to the current position
                percentages[j] = tempPercent; //move the stored percentage to the next position


                //swap frequencies to keep the frequency array synchronized with the percentage array
                int tempFreq = frequency[i]; //store the current frequency count in a temporary variable
                frequency[i] = frequency[j]; //move the higher frequency count to the current position
                frequency[j] = tempFreq; //move the stored frequency count to the next position


                //swap letters to keep the letter array synchronized with the percentages and frequencies
                char tempChar = english_letters[i]; //store the current letter in a temporary variable
                english_letters[i] = english_letters[j]; //move the letter corresponding to the higher percentage to the current position
                english_letters[j] = tempChar; //move the stored letter to the next position
            }
        }
    }
}


//perform a caesar shift backward on a character
char caesarShiftBack(char ch, int shift) {
    if (ch >='a' && ch <= 'z') { //check if the character is alphabetic
        //perform the backward caesar shift and wrap around using modulo
        return (ch - 'a' - shift + ALPHABET_SIZE) % ALPHABET_SIZE + 'a';
    }
    return ch; //if the character is not alphabetic, return it unchanged
}


int main() {
    //open the file containing the ciphertext in read mode
    FILE *file = fopen("ciphertext.txt", "r");
    if (file == NULL) { //check if the file was successfully opened
        perror("Error opening file"); //print an error message if the file couldn't be opened
        return 1; //exit the program with an error code
    }


    //read the entire content of the file into a buffer
    fseek(file, 0, SEEK_END); //move the file pointer to the end of the file to get its size
    long fileSize = ftell(file); //get the size of the file
    fseek(file, 0, SEEK_SET); //move the file pointer back to the beginning to read the content


    //allocate memory for the ciphertext with an extra byte for the null terminator
    char *ciphertext = (char *)malloc(fileSize + 1);
    if (ciphertext == NULL) { //check if memory allocation was successful
        perror("Memory allocation failed"); //print an error message if allocation failed
        fclose(file); //close the file
        return 1; //exit the program with an error code
    }


    //read the entire file content into the buffer
    fread(ciphertext, 1, fileSize, file); //read 'fileSize' bytes from the file into 'ciphertext'
    ciphertext[fileSize] = '\0'; //add a null terminator
    fclose(file); //close the file 


    //frequency analysis
    int frequency[ALPHABET_SIZE] = {0}; //array to store the frequency of each letter
    int totalChars = 0; //variable to keep track of the total number of alphabetic characters processed


    // Count the frequency of only the first character of each block
    for (int i = 0; i < fileSize; i += BLOCK_SIZE) { // Iterate over the file in steps of BLOCK_SIZE
        if (ciphertext[i] >= 'a' && ciphertext[i] <= 'z') { // Check if the character is lowercase alphabetic
            frequency[ciphertext[i] - 'a']++; // Update the frequency using the character's index in the alphabet
            totalChars++; // Increment the total number of lowercase characters processed
        }
    }


    //calculate frequency percentages
    double percentages[ALPHABET_SIZE] = {0}; //array to store frequency percentages (initialized to zero)
    calculateFrequencyPercentages(frequency, percentages, totalChars); //calculate percentages using the frequency data


    //prepare an array of english_letters for sorting
    char english_letters[ALPHABET_SIZE] = "abcdefghijklmnopqrstuvwxyz"; //array of lowercase english letters
    sortByFrequency(percentages, frequency, english_letters); //sort the letters, frequencies, and percentages in descending order


    // Predefined frequency letter mappings
    char option1[ALPHABET_SIZE + 1] = "etaionsrhldcumpfgywbvkxzjq";
    char option2[ALPHABET_SIZE + 1] = "etaoinshrdlucmfywgpbvkxqjz";
    char freq_letters[ALPHABET_SIZE + 1]; //user selected or custom variety


    //prompt the user to select an option
    int choice;
    printf("Choose a frequency letter mapping:\n");
    printf("1: Common English letter frequency (etaionsrhldcumpfgywbvkxzjq), most reliable\n");
    printf("2: Common English letter frequency (etaoinshrdlucmfywgpbvkxqjz), somewhat less reliable\n");
    printf("3: Enter your own custom variety\n");
    printf("Enter your choice (1, 2, or 3): ");
    scanf("%d", &choice);
    printf("\n");


    //handle the users choice
    if (choice == 1) {
        strcpy(freq_letters, option1);
    } else if (choice == 2) {
        strcpy(freq_letters, option2);
    } else if (choice == 3) {
        printf("Enter your own custom variety of 26 unique lowercase letters: ");
        scanf("%s", freq_letters); //get the custom input from the user
        //ensure the input has exactly 26 characters
        if (strlen(freq_letters) != ALPHABET_SIZE) {
            printf("Error: Your input must be exactly 26 unique lowercase letters.\n");
            return 1; // Exit the program if the input is invalid
        }
    } else {
        printf("Invalid choice!\n");
        return 1; //exit the program if the user makes an invalid choice
    }


    //create a mapping from the original letters to the expected english letters
    char mapping[ALPHABET_SIZE];
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        //map each letter to the corresponding letter in 'freq_letters'
        //'english_letters[i] - 'a'' gives the index of the current letter in the alphabet
        mapping[english_letters[i] - 'a'] = freq_letters[i];
    }


    //output the frequency analysis to the console
    printf("Letter --> Frequency --> Percentage --> Mapped To\n");
    printf("------------------------------------------\n");
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        //print each letter, its frequency, its percentage of the total, and the letter its mapped to
        printf("   %c   -->    %d     -->   %.2f%%   -->     %c\n", english_letters[i], frequency[i], percentages[i], freq_letters[i]);
    }


    //create a buffer for the plaintext
    char *plaintext = (char *)malloc(fileSize + 1); //allocate memory for the plaintext
    if (plaintext == NULL) { //check if memory allocation was successful
        perror("Memory allocation failed"); //print an error message if allocation failed
        free(ciphertext); //free the memory allocated for the ciphertext
        return 1; //exit the program with an error code
    }
    strcpy(plaintext, ciphertext); //copy the contents of 'ciphertext' into 'plaintext' for modification


    //decrypt the ciphertext
    for (int i = 0; i < fileSize; i += BLOCK_SIZE) { //iterate over the ciphertext in blocks of size BLOCK_SIZE
    // if (isalpha(ciphertext[i])) { //check if the first character of the block is alphabetic
        // char originalChar = ciphertext[i]; //convert the character to lowercase to ensure case insensitivity
        char newChar = mapping[ciphertext[i] - 'a']; //use the mapping to get the letter based on frequency analysis
        int shift = newChar - 'a'; //calculate the shift value for the caesar cipher


        //replace the first character in the plaintext with the mapped character
        plaintext[i] = newChar;


        //apply the caesar shift backward to the next 5 characters in the block
        for (int j = 1; j < BLOCK_SIZE && (i + j) < fileSize; j++) { //iterate over the next characters in the block
            plaintext[i + j] = caesarShiftBack(ciphertext[i + j], shift); //shift each character backward using the calculated shift
        }
    // }
    }


    //write the modified text (plaintext) to "single_letter_frequency.txt"
    FILE *outputFile = fopen("single_letter_frequency.txt", "w"); //open the output file in write mode
    if (outputFile == NULL) { //check if the file was successfully opened
        perror("Error creating output file"); //print an error message if the file cannot be created
        return 1; //exit the program with an error code
    }
    fprintf(outputFile, "%s", plaintext); //write the decrypted plaintext to the file
    fclose(outputFile); //close the output file
    free(ciphertext); //free the memory allocated for the ciphertext
    free(plaintext); //free the memory allocated for the plaintext
    
    printf("\nSingle Letter Frequency analysis has been written to 'single_letter_frequency.txt'");


    //reopen the file to perform bigram and trigram analysis
    file = fopen("single_letter_frequency.txt", "r"); //open the plaintext file in read mode
    if (file == NULL) { //check if the file was successfully opened
        perror("Error opening file for analysis"); //print an error message if the file cannot be opened
        return 1; //exit the program with an error code
    }


    //read the entire content of the file into a buffer for further analysis
    fseek(file, 0, SEEK_END); //move the file pointer to the end of the file to determine its size
    fileSize = ftell(file); //get the size of the file
    fseek(file, 0, SEEK_SET); //move the file pointer back to the beginning to read the content


    //allocate memory for the text buffer
    char *text = (char *)malloc(fileSize + 1); //allocate memory for the file content with an extra byte for the null terminator
    if (text == NULL) { //check if memory allocation was successful
        perror("Memory allocation failed"); //print an error message if allocation failed
        fclose(file); //close the file 
        return 1; //exit the program with an error code
    }


    fread(text, 1, fileSize, file); //read the entire file content into the buffer
    text[fileSize] = '\0'; //null-terminate the string to make it a valid C string
    fclose(file); //close the file 


    Combination bigrams[ALPHABET_SIZE][MAX_COMBINATIONS]; //array to store bigram combinations for each letter of the alphabet
    Combination trigrams[ALPHABET_SIZE][MAX_COMBINATIONS]; //array to store trigram combinations for each letter of the alphabet
    int bigramCounts[ALPHABET_SIZE]; //array to store the count of bigram combinations for each letter
    int trigramCounts[ALPHABET_SIZE]; //array to store the count of trigram combinations for each letter


    initializeCombinations(bigrams, bigramCounts); //initialize the bigram structures and set the counts to 0
    initializeCombinations(trigrams, trigramCounts); //initialize the trigram structures and set the counts to 0


    analyzeBlocks(text, fileSize, bigrams, bigramCounts, trigrams, trigramCounts); //analyze the text for bigrams and trigrams
    writeAnalysisToFile(bigrams, bigramCounts, trigrams, trigramCounts); //write the analyzed bigram and trigram data to "bigrams_and_trigrams.txt"
    printf("\nBigram and trigram analysis written to 'bigrams_and_trigrams.txt'\n"); //inform the user that the analysis has been written
    free(text); //free the memory used to store the text


    printf("\nEnter the maximum number of word options to display (recommended: 10): ");
    scanf("%d", &MAX_OPTIONS);


    printf("Enter the maximum allowable Levenshtein distance (recommended: 7): ");
    scanf("%d", &MAX_DISTANCE);


    printf("Enter the number of letters to show at a time in the sliding window (recommended: 14): ");
    scanf("%d", &SLIDING_BLOCK_SIZE);


    load_dictionary("dictionary.txt"); //load words from the dictionary file into a data structure


    //open the output file "completed_frequency_analysis.txt" in write mode
    FILE *output_file = fopen("completed_frequency_analysis.txt", "w");
    if (output_file == NULL) { //check if the file was successfully opened
        perror("Failed to open output file"); //print an error message if the file couldn't be opened
        return 1; //exit the program with an error code
    }


    //read the text from the file which has undergone individual letter frequency analysis replacements from the input file
    FILE *input_file = fopen("single_letter_frequency.txt", "r");
    if (input_file == NULL) { //check if the file was successfully opened
        perror("Failed to open input file"); //print an error message if the file couldn't be opened
        return EXIT_FAILURE; //exit the program with an error code
    }
    char freq_analys_block[200000]; //buffer to hold the unspaced text from the file (adjust size as needed)
    fscanf(input_file, "%s", freq_analys_block); //read the entire content of the file into the buffer as a single string
    fclose(input_file); //close the input file


    int start = 0; //initialize the starting position for processing the dictionary text
    while (start < strlen(freq_analys_block)) { //loop until the entire text has been processed
        //determine the end of the current block of characters
        int block_end = start + SLIDING_BLOCK_SIZE; //calculate the end position for the current block
        if (block_end > strlen(freq_analys_block)) { //if the end position exceeds the text length
            block_end = strlen(freq_analys_block); //adjust the end position to the end of the text
        }


        char block[SLIDING_BLOCK_SIZE + 1]; //buffer to store the current block of characters
        strncpy(block, freq_analys_block + start, block_end - start); //copy the block from the text
        block[block_end - start] = '\0';


        char *options[MAX_OPTIONS]; //array to store the word options
        int distances[MAX_OPTIONS]; //array to store the levenshtein distances of the options
        int num_options; //number of options generated
        generate_options(freq_analys_block, start, options, distances, &num_options); //generate options based on the current text block


        int choice; //users choice
        printf("Enter the number of your choice (or 0 to enter manually, or -1 to reassess parameters): ");
        scanf("%d", &choice);


        char chosen_word[MAX_WORD_LENGTH];
        if (choice == 0) {
            //manual input
            printf("Enter the word manually: ");
            scanf(" %[^\n]%*c", chosen_word); //reads an entire line of text including spaces and stores it in chosen_word
        } else if (choice == -1) {
            //reassess parameters and reiterate the same segment
            printf("\nWould you like to:\n");
            printf("1. Reassess MAX_OPTIONS\n");
            printf("2. Reassess MAX_DISTANCE\n");
            printf("3. Reassess SLIDING_BLOCK_SIZE\n");
            printf("Enter your choice (1-3): ");
            int reassess_choice;
            scanf("%d", &reassess_choice);


            switch (reassess_choice) {
                case 1:
                    printf("Enter new value for MAX_OPTIONS: ");
                    scanf("%d", &MAX_OPTIONS);
                    break;
                case 2:
                    printf("Enter new value for MAX_DISTANCE: ");
                    scanf("%d", &MAX_DISTANCE);
                    break;
                case 3:
                    printf("Enter new value for SLIDING_BLOCK_SIZE: ");
                    scanf("%d", &SLIDING_BLOCK_SIZE);
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
                    break;
            }
            continue; //reiterate the loop to process the same segment
        } else if (choice > 0 && choice <= num_options) {
            //copy the chosen word from options
            strcpy(chosen_word, options[choice - 1]);
        } else {
            printf("Invalid choice. Please try again.\n");
            continue;
        }


        fprintf(output_file, "%s ", chosen_word); //write the chosen word to the output file
        fflush(output_file); //ensure the word is written to the file immediately


        //output the chosen word and update the start position
        printf("You selected: %s\n", chosen_word);
        start += strlen(chosen_word); //correctly update the start position
    }


    //free allocated memory for the dictionary
    for (int i = 0; i < DICTIONARY_SIZE && dictionary[i] != NULL; i++) {
        free(dictionary[i]);
    }
    
    fclose(output_file);


    return 0;
}
