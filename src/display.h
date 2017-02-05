#ifndef DISPLAY
#define DISPLAY

#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

SDL_Rect box;
SDL_Event event, keyEvent;
SDL_version compiled, linked;
SDL_DisplayMode current;
SDL_Window* disp;
SDL_Renderer* renderer;

Mix_Chunk *beep;

uint8_t waitForKeypress();
bool keyDown(char *), screen[32][64], gfx_flag, pause, fullscreen, pixelSpace, handleEvents(), bit;
void initializeWindow(), initializeAudio(), playBeep(), delayTime(), drawGraphics(), refreshKeyboard();

#endif // DISPLAY
