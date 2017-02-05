#ifndef MAIN
#define MAIN

#define range(x,y) for(int x=0;x<y;x++)

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

//#define opcodeDEBUG

void debug(const char *, ...), opc(const char *, ...), runOpcode(uint16_t);
uint16_t stackMem[16], delay, sound, pc, I, sc, BCD, newX, newY, opcode, OPS;
time_t start, check;
uint8_t memory[4096], reg[16], hexChars[80];
const char* mapToKeys;
char* key;

#endif // MAIN
