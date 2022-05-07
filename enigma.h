int main(int argc, char* argv[]);

void encrypt(char* c, char plugboard[13][2]);

char handlePlugBoard(char c, char plugboard[13][2]);

char handleRoters(char c);

void handleChar(char c, char plugboard[13][2]);

char useRotors(char c);

void rotate();

char grabNextChar(char c, char* rotorString, char* nextRotor);

char* rotateString(char* s);

void setup();

void selectRotorOrder();

void selectRotorStartingPoint();

void cleanMemory();
