# Enigma
A C implementation of the Enigma cipher machine with no memory leaks.

# Usage: 

>make
>./x mystringtoencrypt
From here, the program will instruct you to select the initial rotor orientation and deliver your encryption.
# Notes and Limitations
- Currently only accepts lowercase inputs with no spaces
- The plugboard is current fixed and may not be changed
- There are only three rotors and users may not supply their own
- The gcc compiler is required to use the suplied Makefile
- The application is memory safe with Valgrind reporting all blocks free'd
- There are only 26^3 = 17,576 possible initial configurations; cryptanalysis is ongoing
