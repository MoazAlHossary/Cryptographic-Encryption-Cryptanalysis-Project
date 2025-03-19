## 1. Scope
The code provides several key functionalities:
  - Frequency Analysis: Analyzes the frequency of initial characters in each block of the ciphertext and uses this information to create a mapping of the most common letters and compares them to expected English letter frequencies.
  - Decryption: Implements a Caesar cipher backward shift on the remaining 5 characters of a block utilizing the mapping of the initial character.
  - Word Based Validation: Uses a dictionary of known words to validate and refine the decrypted text which allows for user interaction to ensure relative accuracy of the output, with an option allowing for manual input when automated options are insufficient.
  - Bigrams and Trigrams Analysis: Collects and sorts common character pairs and triples within the text to gain insights into language patterns.

## 2. Possible Attacks on Polyalphabetic Ciphers
Polyalphabetic techniques present a more complex challenge than a simple monoalphabetic cipher due to their shifting and substitution mechanisms, which in our case, we combined monoalphabetic substitutions with Caesar shifts which resulted in a dual encryption which should increase the algorithm’s resistance to simple attacks, however, this mechanism is still very susceptible to cryptanalysis.   

1. **Frequency Analysis:** Although the monoalphabetic substitution at first hides the frequency patterns of the Caesar cipher, the monoalphabetic cipher itself remains vulnerable to frequency analysis because it produces ciphertext which statistical characteristics that can be exploited due to the open design nature of the mechanism, therefor a cryptanalyst can leverage the frequency of the monoalphabetic substitutions to map them back to their original characters, using known letter frequency distributions of the English language and then reverse Caesar shift the remaining characters of the blocks which will allow the cryptanalyst to reconstruct the plaintext.

2. **Known-Plaintext and Chosen-Plaintext Attacks:** If an attacker gains access to a large sample size of plaintext-ciphertext pairs, they can deduce parts of the substitution table with statistical analysis until it is partially or fully reconstructed, from there, an attack only needs to perform the Caesar shifts to retrieve the key.  
3. **Attack via Block Repetition Analysis:** Since the mechanism utilizes consistent blocks of 6 and a deterministic Caesar shift tied to the index of the initial character, a cryptanalyst can analyze repeated block patterns in the ciphertext to infer clues about the shifts and help map back to the original characters.  
4. **Brute Force via Statistical Analysis:** A cryptanalyst can try each possible initial character, perform the inverse Caesar shift, and validate the resulting words against a dictionary.   


## 3.Overview of the Code
1.**Reading and Decrypting the Ciphertext**
   **Reading the Ciphertext:** The program reads the encrypted text in one go using fread() from ciphertext.txt and dynamically allocates buffer using malloc(), using fseek() and ftell() to determine the size of the file and then checks for errors in file handling, and finally the string is null terminated to ensure that it is a properly formatted C string.  
3.**Frequency Analysis**
   - **Single Letter Frequency Analysis:** The code counts the frequency of each initial character of a block in the text, updating a frequency array where each index corresponds to a letter of the alphabet at each iteration, calculates frequency percentages, and sorts the letters in descending order of frequency, printing the single letter frequency analysis to the terminal in an organized manner. The program then uses these frequencies to begin the decryption process, comparing these frequencies to standard English letter frequencies and using this information to create mappings of each character. Using this mapping, the remaining 5 letters of the block are backwards Caesar shifted accordingly, assuming all characters are lowercased English letters.
   - **Bigrams and Trigrams Analysis:** To gain deeper insights into the text, the program analyzes common bigrams and trigrams which helps in identifying patterns and structures within the text on a letter-to-letter basis, calculating the bigrams and trigrams for each initial letter of a block in the ciphertext separately. These bigrams and trigrams are then sorted and written to a file for further review.  
5. **Word Validation**
**Dictionary Validation:** The program uses a preloaded dictionary consisting of 10,000 common English words to validate the decrypted words, dividing the text into segments of the user’s choice, where each segment is checked against a dictionary to determine if it forms a valid English words using the Levenshtein distance metric to measure the similarity between the current segment and words in the dictionary by calculating the minimum number of edits (insertions, deletions, or substitutions) needed to transform a word into another. The user is then given a list of the found words, sorted by similarity (distance) to the decrypted segment, along with their relevant distance, helping the user identify the most likely word, or input their own word if none of the recommendations are satisfactory. The user may also edit certain parameters during the decryption process to yield results in the case that none could be found using the initial configuration.  


## 4. User Interaction
- User-Configurable Parameters:
    - MAX_DISTANCE: The user can specify the maximum allowable Levenshtein distance for a word to be considered a close match which allows for tighter or looser matching.
    - MAX_OPTIONS: The user can define how many recommended word options are presented for each segment, where the user is presented with up to MAX_OPTIONS words that are similar to the current text segment.
    - SLIDING_BLOCK_SIZE: The user can set the number of letters in every segment, which allows for manipulating how much of the text is tested at once.
    - Custom English Character Frequency: The user can choose from predefined character frequency mappings or input their own custom frequency order, allowing the user to test for most the most successful combination.
    - User Choice: The user can select one of the recommended options by entering the corresponding number or choose to manually input a word.

## 5. Conclusion and Summary of Work
- Decryption Using Frequency Analysis: Implemented a system that analyzes character frequency and maps the most common characters in the ciphertext to their expected English equivalents, using a backward Caesar cipher shift to decipher the text the remaining 5 characters in the block.
- Bigrams and Trigrams Analysis: Added functionality to analyze common bigrams and trigrams.
- Word Validation: Integrated a dictionary-based validation mechanism that allows the user to select or manually input words.
- User Interaction: Allowed the user to insert custom English letter frequency, customizable through user input, or to choose from a set of 2 predefined English letter frequency tables, along with key parameters, such as MAX_OPTIONS, MAX_DISTANCE, SLIDING_BLOCK_SIZE, and, and provided a functionality allowing the user to alter said parameters midst deciphering.





