#ifndef MAIN

#define MAIN
#define range(x,y) for(int x=0;x<y;x++)

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <stdint.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#ifdef DEBUG
//#define opcodeDEBUG
#define debug printf
#else
#define debug //
#endif // DEBUG

uint16_t stackMem[16], delay, sound, pc, I, sc, BCD, newX, newY;
SDL_Event keyEvent;
bool screen[32][64], gfx_flag, unpressed, bit;
uint8_t memory[4096], reg[16], *keyState, hexChars[80];
const char* mapToKeys;
char* key;

#endif // MAIN
