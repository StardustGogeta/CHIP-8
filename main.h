#ifndef MAIN

#define MAIN
#define range(x,y) for(int x=0;x<y;x++)

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdint.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

//#define opcodeDEBUG

void debug(const char *, ...), opc(const char *, ...), runOpcode(uint16_t);
uint16_t stackMem[16], delay, sound, pc, I, sc, BCD, newX, newY, opcode, w;
SDL_Event keyEvent;
SDL_version compiled, linked;
SDL_DisplayMode current;
bool screen[32][64], gfx_flag, unpressed, bit, pause, fullscreen;
uint8_t memory[4096], reg[16], *keyState, hexChars[80], mapFromKeys(const char *);
const char* mapToKeys;
char* key;

#endif // MAIN
