# Cryptographic-Encryption-Cryptanalysis-Project

## Overview
This Project focuses on developing and analyzing a combined encryption system which integrates substitution ciphers and Caesar shifts. It consists of two phases:

1. **Encryption & Decryption Implementation** - Developing an encryption system with multiple key selection methods.
2. **Cryptanalysis & Security Evaluation** - Tests of encryption security through frequency analysis examine the encryption system’s vulnerabilities.

## Objectives
### Phase 1
1. Develop a substitution and Ceasar-based encryption algorithm which take a user’s provided plaintext file and encrypts it into a ciphertext. 
2. Develop a decryption algorithm which can restore the original plaintext from the ciphertext by reversing the substitutions and shifts.
3. Write detailed documentation and explanation for each step of the algorithms. 
4. Allow the user to choose from a default key, a truly random generated key, or a user defined key for the encryption process, and ensure that the decryption can handle all three options. 
5. Develop code which explains each step of the algorithm process in detail, making it easier for a user to understand how their plaintext has been encrypted and decrypted.
  
### Phase 2
The objective of this Phase is to decrypt text that was encrypted using a block-based cryptographic algorithm, where the plaintext is split into 6-character blocks, and each block's initial character is substituted using a substitution table, and the remaining 5 characters are Caesar shifted based on the index of the original initial character. This phase of the project leverages frequency analysis, pattern recognition, and statistical properties of the english language to recover the plaintext from the encrypted text, all the while conducting an in depth analysis of bigrams and trigrams within the blocks to understand letter and word frequencies more effectively, thus enhancing the decryption process and ensuring that the output is linguistically precise and relevant in the presence of the substitution and shifting schemes.     

# Phase 1 Encryption & Decryption
## Encryption Algorithm:  
The encryption algorithm uses substitution and shifting to transform the plaintext into ciphertext. Below is an explanation of the algorithm:  

#### 1. Each letter of the plaintext is replaced with a corresponding letter from the substitution table. It is worth noting that the substitution table in this cryptographic algorithm is the key used for encryption and decryption, and can be one of three types:     

  - Default Key, or a predetermined arrangement of the alphabet (hilwmkbdpcvazusjgrynqxofte). 
  
  - A randomly generated key using the Fisher-Yates shuffle method, ensuring each randomly generated key produces a different substitution key. The Fisher-Yates shuffle method randomly shuffles a 
list of items fairly, meaning every possible ordering is equally likely[2].  

  - A user defined key, which essentially allows the user to assign any substitution table they desire.      

  2. After the character is substituted, the next 5 alphabetic characters are shifted by a value equal to that of the index of the substituted character. For example, if the character which has been substituted in step 1 is ‘d’, then the next 5 characters will be shifted forward by 3 positions in the alphabet (‘d’ has an index of 3).
  3. The code features an algorithm explainer which allows the user to go step by step throughout the encryption algorithm.  


## Decryption Algorithm:     
The decryption algorithm is essentially the encryption algorithm, reversed. It works as follows:  

1. It first finds each character in the substitution table to find its original character, reversing the substitution operation applied during encryption.
2. It then shifts the next 5 characters backwards an ‘n’ number of positions, depending on the index of the first and every sixth character.
3. The decryption algorithm, like the encryption algorithm, also features an algorithm explainer to assist the user in understanding the decryption process. 

## Implementation Specifications:
This project was implemented using C programming language, requires GCC to run, and the encryption and decryption algorithms were developed with the following specifications:

---

## Key Generation/Selection:
The user may choose from the following Key schemes: 
1. A default substitution key (hilwmkbdpcvazusjgrynqxofte).
2. A randomly generated key which uses the Fisher-Yates shuffle algorithm and the ‘srand(time(0))’ function to seed the random number generator with an ever changing number (the number of seconds since January 1, 1970)[3]. The key is saved in a file called ‘encryption_key.txt’ for future use.
3. A user defined key, where the user can input a custom 26 letter key.
![image](https://github.com/user-attachments/assets/55b2d9ce-b60d-41e2-8e42-b7a3bf0a9b5f)

---

## File Handling:
1. The plaintext is read from a file named ‘plaintext.txt’.
2. The ciphertext is saved to ‘ciphertext.txt’.
3. During decryption, the ciphertext is read from ‘ciphertext.txt’ and the output is saved to ‘plaintext_decrypted.txt’.
4. If the random key is used, the key is saved to a file called ‘encryption_key.txt’ for consistency in decryption. 

 ---

## Encryption and Decryption Functions:
1. The main functions involved in encryption and decryption are:
‘index_finder()’, which finds the index of a letter in the alphabet or the substitution table.  
![image](https://github.com/user-attachments/assets/c5c89cae-6f0c-4071-b198-0b1131888a75)
    
2. ‘substitution()’, which performs the substitution based on a key.
![image](https://github.com/user-attachments/assets/6ddbf0bf-ced8-45d9-bd4a-0bec93d13903)
  
3. ‘shift_char()’, which shifts the letters to either encrypt or decrypt a message. 
  ![image](https://github.com/user-attachments/assets/381206d8-ee8e-458e-a269-9457e648cb49)

4. ‘explain_encryption()’ and ‘explain_decryption()’, which provide detailed information for each substitution and shift performed, aiding in the user’s understanding of how each character of the message has been transformed.
![image](https://github.com/user-attachments/assets/65facf27-4d1a-461f-b9b9-cf25c3151925)


---

## 1. Encryption Output
When the encryption code is run, the user is prompted to choose the Key they would like to perform the encryption with. This is the key that will be used for the encryption and decryption. After choosing the key type, an explanation of the process is shown to the user.
![image](https://github.com/user-attachments/assets/055a5737-f157-47e0-9dd4-36a41d14cea2)
  
## 2. Decryption Output
When the decryption code is run, the user is prompted to choose the Key which was used to perform the encryption. After choosing the key type, an explanation of the process is shown to the user.

![image](https://github.com/user-attachments/assets/b081601b-1230-485e-88b7-abf8d682c9cd)

## 3. Folder Contents Post-Decryption
Before starting the encryption, a plaintext.txt needs to be present in the directory. After encryption, ciphertext.txt is written into the directory; this file is important for the decryption algorithm which outputs plaintext_decrypted.txt
![image](https://github.com/user-attachments/assets/7996bfbd-aa85-42c0-bed6-c2026eae5113)

## 4. Output Post-Encryption and Decryption
As seen in the below screenshot, the encryption algorithm successfully encrypts, and the decryption algorithm successfully decrypts. 
![image](https://github.com/user-attachments/assets/0170659e-2887-4b72-96c1-0dbdda2ad79d)


## 5. References
GeeksforGeeks. (2023, November 23). How to write a pseudo code? https://www.geeksforgeeks.org/how-to-write-a-pseudo-code/ 
Shuffle a given array using Fisher–Yates shuffle algorithm. GeeksforGeeks. (2022, December 19). https://www.geeksforgeeks.org/shuffle-a-given-array-using-fisher-yates-shuffle-algorithm/ 
Ssand(time(0)) and random number generation. Stack Overflow. (2016, May 1). https://stackoverflow.com/questions/4736485/srandtime0-and-random-number-generation 
