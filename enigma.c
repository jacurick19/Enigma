/*
*	Jacob Urick
*	5/27/2022
*	This is a sample enigma machine with a fixed plugboard.
*	The configuration and initial roation of the disks is variable.
*	It operates on each lowercase input character as follows:
*       input -> plugboard -> disks -> plugboard -> disks in reverse order -> plugboard.
*	The disks rotate as in the original machine after each character.
*	Decryption is by entering the ciphertext with identical initial configuration.
*
*/
#include <stdio.h>
#include <stdlib.h>
#include "enigma.h"

char g_rotor1[] = "ekmflgdqvzntowyhxuspaibrcj";
char g_rotor2[] = "ajdksiruxblhwtmcqgznpyfvoe";
char g_rotor3[] = "bdfhjlcprtxvznyeiwgakmusqo";
char** g_rotors;
char plugboard[13][2] = {{'a', 'q'}, {'w', 's'}, {'d', 'e'}, {'r', 'f'}, {'g', 't'}, {'y', 'h'}, {'j', 'u'}, {'i', 'k'}, {'m', 'n'}, {'b', 'v'}, {'c', 'x'}, {'l', 'o'}, {'p', 'z'}};
int g_rotor1_count = 0;
int g_rotor2_count = 0;
int flag = 0;

/*
*	Encrypts the argument supplied to the program
*/
int main(int argc, char *argv[]){
	setup();
	encrypt(argv[1], plugboard);
	cleanMemory();
	printf("\n");
	return 0;
}

/*
*	Does a final free on the rotor array
*/
void cleanMemory(){
	for(int i = 0; i < 3; i++){
		free(g_rotors[i]);
	}
	free(g_rotors);
}

/*
*	Initializes the rotor array with the three alphabets
*/
void initializeAr(){
        g_rotors = calloc(3, sizeof(char*));
        g_rotors[0] = g_rotor1;
        g_rotors[1] = g_rotor2;
        g_rotors[2] = g_rotor3;
}

/*
*	Sets up the device by initializing the rotors.
*/
void setup(){
	initializeAr();
	selectRotorOrder();
	selectRotorStartingPoint();
}

/*
*	Walks the user through selecting the initial rotation of each rotor.
*/
void selectRotorStartingPoint(){
	int offset = 0;
	for(int i = 0; i < 3; i++){
		printf("What should the offset be?\n");
		scanf("%d", &offset);
		for(int j = 0; j < offset; j++){
			char* toFree = g_rotors[i];
			g_rotors[i] = rotateString(g_rotors[i]);
			if(flag){
				free(toFree);
			}else{
				flag = 1;
			}
		}
		flag = 0;
	}
}

/*
*	Walks the user through selecting the desired orientation of rotor disks.
*/
void selectRotorOrder(){
	char choice1, choice2, choice3;
	choice1 = 0;
	choice2 = 0;
	choice3 = 0;
	printf("We're going to select the rotor order.\n");
	printf("Which alphabet should be first(1,2, or 3)?\n");
	choice1 = getchar();
	choice2 = choice1;
	getchar();
	while(choice1 == choice2){
		printf("Which aphabet should be second?\n");
		choice2 = getchar();
		getchar();
	}
	choice3 = choice2;
	while(choice3 == choice2 || choice3 == choice1){
                printf("Which aphabet should be third?\n");
                choice3 = getchar();
        	getchar();
	}
	char* temp = 0;
	temp = g_rotors[choice1 - '0' - 1];
	g_rotors[0] = temp;
        temp = g_rotors[choice2 - '0' - 1];
        g_rotors[1] = temp;
        temp = g_rotors[choice3 - '0' - 1];
        g_rotors[2] = temp;
}

/*
*	Encrypts the string *c.
*/
void encrypt(char* c, char plugboard[13][2]){
	while(*c != 0){
		handleChar(*c, plugboard);
		c = c + 1;
	}
}

/*
*	Encrypts and prints char $c using the specified rotor order and $plugboard.
*/
void handleChar(char c, char plugboard[13][2]){
	char current = c;
	current = handlePlugBoard(c, plugboard);
	current = useRotors(current);
	current = handlePlugBoard(current, plugboard);
	printf("%c", current);
}

/*
*	Takes the input char through the rotors, reflects through the plugboard,
*	and back. Also rotates the rotors.
*/
char useRotors(char c){
	for(int i = 0; i < 2; i ++){
		c = grabNextChar(c, g_rotors[i], g_rotors[i+1]);
	}
	c = handlePlugBoard(c, plugboard);
	for(int i = 2; i > 0; i --){
		c = grabNextChar(c, g_rotors[i], g_rotors[i-1]);
	}
	rotate();
	return c;
}

/*
*	Rotates all alphabets as in the original device, with 26 turns of the
*	first disk to result in one turn of the second disk, with the third disk
*	behaving analogously.
*/
void rotate(){
	g_rotor1_count++;
	char* toFree = g_rotors[0];
	g_rotors[0] = rotateString(g_rotors[0]);
	free(toFree);
	if(g_rotor1_count > 26){
		g_rotor2_count++;
		toFree = g_rotors[1];
		g_rotors[1] = rotateString(g_rotors[1]);
		free(toFree);
		g_rotor1_count = 0;
		if(g_rotor2_count > 26){
			toFree = g_rotors[2];
			g_rotors[2] = rotateString(g_rotors[2]);
			free(toFree);
			g_rotor2_count = 0;
		}
	}
}

/*
*	Rotates the alphabet forward one letter
*/
char* rotateString(char* s){
	char* nextStr = calloc(27, sizeof(char));
	char last = *(s + 25);
	for(int i = 1; i < 26; i ++){
		*(nextStr + i) = *(s + (i - 1));
	}
	*nextStr = last;
	*(nextStr + 26) = '\n';
	return nextStr;
}

/*
*	Returns the char in $nextRotorString with the index
*	coresponding to $c's position on $rotorString.
*/
char grabNextChar(char c, char* rotorString, char* nextRotorString){
	for(int i = 0; i < 26; i++){
		if(c == rotorString[i]){
			return nextRotorString[i];
		}
	}
}
/*
*	Replaces $c with the plugboard specified char.
*/
char handlePlugBoard(char c, char plugboard[13][2]){
	for(int i = 0; i < 13; i++){
		if(plugboard[i][0] == c){
			return plugboard[i][1];
		}
		if(plugboard[i][1] == c){
			return plugboard[i][0];
		}
	}
	return c;
}

