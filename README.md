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
  
