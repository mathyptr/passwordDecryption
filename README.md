# Decrypt passwords encrypted using crypt (DES)
OpenMP implementation of an algorithm implementing a brute-force attack to decrypt an eight-character passwords encrypted with DES. 
The aim of this work is to compare the computational costs of a sequentail and a parallel version of a brute-force attack on password, evaluating the speedup achieved in several test situations through parallelization.

## Overview
This project is a mid term project for the course of Parallel Computing, held by professor Marco Bertini at University of Florence.

This study focuses on decrypting an eight-character password from the [a-zA-Z0-9./] character set. 

The methodology employs a dictionary based attack that iteratively hashes entries from a common password database using the Data Encryption Standard (DES). These generated hashes are then compared against the target hash to find a match.
This implementation relies on the POSIX crypt() function, a standard for password hashing in Unix-based system.

## Proposed solutions
To address the password decryption problem, this project develops a comparative study between sequential and parallel computational models. All implementations are written in the C++ programming language. The proposed methodologies consist of the following distinct versions:

1. Sequential: a single-threaded baseline implementation.

2. Parallel OMP: a multi-threaded solution utilizing OpenMP directives for concurrent execution.

## Dataset
Given that our project targets an eight-character password space within the specific [a-zA-Z0-9./] character set, we implemented a dedicated preprocessing pipeline for the RockYou dataset.

RockYou.txt:
Originating from a 2009 security breach, the RockYou.txt wordlist was born when the RockYou social application suffered a cyberattack exposing over 32 million credentials. The gravity of the leak was intensified by the fact that passwords were stored in plaintext—a fundamental security failure. Today, this dataset is an industry-standard resource for penetration testers and IT administrators. By
employing this list within tools like Hashcat or John the Ripper, security experts can simulate real-world attacks to identify authentication.

## Requirements
In order to run the code are required:
- OpenMP
- C++17 or newer 
