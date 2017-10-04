#include "display.h"

uint8_t mapFromKeys(const char * keyInput) {
    switch ((char)keyInput) {
        case 'X': return 0x0;
        case '1': return 0x1;
        case '2': return 0x2;
        case '3': return 0x3;
        case 'Q': return 0x4;
        case 'W': return 0x5;
        case 'E': return 0x6;
        case 'A': return 0x7;
        case 'S': return 0x8;
        case 'D': return 0x9;
        case 'Z': return 0xa;
        case 'C': return 0xb;
        case '4': return 0xc;
        case 'R': return 0xd;
        case 'F': return 0xe;
        case 'V': return 0xf;
        default:
            printf("\nERROR: Failed input: %s\n",keyInput);
            return -1;
    }
}

void initializeWindow(uint64_t pixelSize) {
    SDL_Init(SDL_INIT_EVERYTHING);
    atexit(SDL_Quit);
    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);
    disp = SDL_CreateWindow("Stardust - Chip-8 Emulator",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            64*pixelSize, 32*pixelSize, SDL_WINDOW_RESIZABLE );
    renderer = SDL_CreateRenderer(disp, -1, SDL_RENDERER_ACCELERATED);
    SDL_ShowCursor(0);
}

void initializeAudio() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024)==-1) {
        printf("%s",Mix_GetError());
    }
    beep = Mix_LoadWAV("beep.wav");
    Mix_AllocateChannels(16);
    Mix_VolumeChunk(beep,3);
    Mix_PlayChannel(1,beep,0);
}

void playBeep() {
    Mix_PlayChannel(1,beep,0);
}

bool handleEvents() {
    while(SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return 0;
            case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    return 0;
                case SDLK_SPACE:
                    pause ^= 1;
                    printf("The game has been %spaused.\n",pause?"":"un");
                    break;
                case SDLK_l:
                    fullscreen ^= 1;
                    SDL_SetWindowFullscreen(disp,fullscreen?SDL_WINDOW_FULLSCREEN_DESKTOP:0);
                    printf("The game has been set to %s.\n",fullscreen?"fullscreen":"windowed mode");
                    gfx_flag = 1;
                    break;
                case SDLK_k:
                    pixelSpace ^= 1;
                    gfx_flag = 1;
                    break;
            }
        }
        switch (event.window.event)
        {
            case SDL_WINDOWEVENT_MAXIMIZED:
            case SDL_WINDOWEVENT_RESIZED:
            {
                int w = 64*round(event.window.data1/64);
                SDL_SetWindowSize(disp,w,w/2);
                gfx_flag = 1;
                break;
            }
        }
    }
    return 1;
}

void drawGraphics() {
    int x, y, w, h;
    SDL_GetWindowSize(disp,&w,&h);
    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    SDL_RenderClear(renderer);
    box.h = h/32-pixelSpace, box.w = w/64-pixelSpace;
    SDL_SetRenderDrawColor(renderer, 255,255,255,0);
    for(y=0; y<32; y++) for(x=0; x<64; x++) {
        box.x = w/64 * x;
        box.y = h/32 * y;
        if (screen[y][x]) SDL_RenderFillRect(renderer,&box);
    }
    SDL_RenderPresent(renderer);
    gfx_flag = 0;
}

uint8_t waitForKeypress() {
    SDL_WaitEvent(&keyEvent);
    while(SDL_PollEvent(&keyEvent)) {
        if (keyEvent.type == SDL_KEYDOWN) {
            return mapFromKeys(SDL_GetKeyName(keyEvent.key.keysym.sym));
        }
    }
    return -1;
}

void refreshKeyboard() {
    SDL_PumpEvents();
}

bool keyDown(char* key) {
    return SDL_GetKeyboardState(NULL)[SDL_GetScancodeFromName(&key)];
}

void delayTime(int ms) {
    SDL_Delay(ms);
}

