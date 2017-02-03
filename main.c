#include "main.h"

// Select options here:
#define FRAMETIME 2
#define BOXSIZE 12
#define ROM_FILEPATH "c8games/PONG2"

void debug(const char *str, ...) {
    #ifdef DEBUG
    va_list l;
    va_start(l, str);
    vprintf(str, l);
    #else
    return;
    #endif // DEBUG
}

uint16_t pc=0x200, sc=15;
const char* mapToKeys = "X123QWEASDZC4RFV";
uint8_t hexChars[80] = {0xF0, 0x90, 0x90, 0x90, 0xF0,
                0x20, 0x60, 0x20, 0x20, 0x70,
                0xF0, 0x10, 0xF0, 0x80, 0xF0,
                0xF0, 0x10, 0xF0, 0x10, 0xF0,
                0x90, 0x90, 0xF0, 0x10, 0x10,
                0xF0, 0x80, 0xF0, 0x10, 0xF0,
                0xF0, 0x80, 0xF0, 0x90, 0xF0,
                0xF0, 0x10, 0x20, 0x40, 0x40,
                0xF0, 0x90, 0xF0, 0x90, 0xF0,
                0xF0, 0x90, 0xF0, 0x10, 0xF0,
                0xF0, 0x90, 0xF0, 0x90, 0x90,
                0xE0, 0x90, 0xE0, 0x90, 0xE0,
                0xF0, 0x80, 0x80, 0x80, 0xF0,
                0xE0, 0x90, 0x90, 0x90, 0xE0,
                0xF0, 0x80, 0xF0, 0x80, 0xF0,
                0xF0, 0x80, 0xF0, 0x80, 0x80};


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
            debug("\nFailed input: %c\n",keyInput);
            break;
    }
}

void loadRom(const char* path) {
	FILE* ROM = fopen(path,"rb");
	fseek(ROM, 0, SEEK_END);
	int ROMsize = ftell(ROM);
	debug("The file size is %d bytes.\n", ROMsize);
	rewind(ROM);
	for (int x=0; x<ROMsize; x++)
		fscanf (ROM, "%c", &memory[0x200+x]);
	fclose(ROM);
	/* Examples for printing hex values.
	printf("The first byte is %x\n",memory[512]);
	printf("The second byte is %x\n",memory[513]);
	*/
}

int main(int argc, char* argv[]) {
    /* Test keyboard to input translation, and back again:
    for (uint8_t x=0x0;x<0xf;x++) {
        printf("Original: %x, Chip-8: %c, Revert: %x\n",
                x, mapToKeys[x], mapFromKeys(mapToKeys[x]));
    }
    */
	debug("Program start.\n");
	SDL_Init(SDL_INIT_EVERYTHING);
	atexit(SDL_Quit);
	uint8_t pixVal, pixelSpace = 1;
	const int pixelSize = BOXSIZE;
	SDL_VERSION(&compiled);
    debug("We compiled against SDL version %d.%d.%d ...\n",
       compiled.major, compiled.minor, compiled.patch);
	SDL_GetVersion(&linked);
	debug("But we are linking against SDL version %d.%d.%d.\n",
       linked.major, linked.minor, linked.patch);
	SDL_Window* disp = SDL_CreateWindow("Stardust - Chip-8 Emulator",
										SDL_WINDOWPOS_UNDEFINED,
										SDL_WINDOWPOS_UNDEFINED,
										64*pixelSize, 32*pixelSize,
										SDL_WINDOW_RESIZABLE
										);
	SDL_Renderer* renderer = SDL_CreateRenderer(disp, -1, SDL_RENDERER_ACCELERATED);
    SDL_Rect box; SDL_Event event;
    keyState = SDL_GetKeyboardState(NULL);
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024)==-1) {
        printf("%s",Mix_GetError());
    }
    Mix_Chunk *beep = Mix_LoadWAV("beep.wav");
    Mix_AllocateChannels(16);
    Mix_VolumeChunk(beep,3);
    Mix_PlayChannel(1,beep,0);
    debug("The sound has been initialized.\n");
    /* Key Testing Example
    char* test = mapToKeys[0x0];
    printf("the key to detect is %c\n",test);
    while(1) {
        SDL_PumpEvents();
        if (keyState[SDL_GetScancodeFromName(&test)]) break;
    }
    printf("X pressed!\n");
	return 0;
    */
	// range(x,32) range(y,64) screen[x][y] = (x+y)%2?0:1;
	for(int x=0;x<80;x++) memory[x] = hexChars[x];
    const char* path;
    if (argc > 1) {
        path = argv[1];
        printf("File argument: %s\n", path);
    }
    else {
        path = ROM_FILEPATH;
        printf("Defaulting to %s.\n", path);
    }
    loadRom(path);
    printf("Successfully loaded file %s.\n\nPress [ESC] to quit.\n"
        "Press [SPACE] to pause or [L] to enter fullscreen.\n"
        "Press [K] to toggle space between pixels.\n\n",path);
    SDL_ShowCursor(0);
    gfx_flag = 1;
	while (pc<4096) {
		while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    //pc = 4096;
                    goto ending;
                    break;
                case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        //pc = 4096;
                        goto ending;
                        break;
                    case SDLK_SPACE:
                        pause ^= 1;
                        printf("The game has been %spaused.\n",pause?"":"un");
                        break;
                    case SDLK_l:
                        fullscreen ^= 1;
                        SDL_GetCurrentDisplayMode(0, &current);
                        SDL_SetWindowFullscreen(disp,fullscreen?SDL_WINDOW_FULLSCREEN_DESKTOP:0);
                        printf("The game has been set to %s.\n",fullscreen?"fullscreen":"windowed mode");
                        w = 64*round(current.w/64);
                        SDL_SetWindowSize(disp,w,w/2);
                        gfx_flag = 1;
                        break;
                    case SDLK_k:
                        pixelSpace ^= 1;
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
        if (!pause) {
            opcode = memory[pc]<<8|memory[pc+1];
            debug("Attempting to run opcode %04x at position %04x\n", opcode, pc);
            runOpcode(opcode);
            pc += 2;
            if (delay) {
                delay --;
            }
            if (sound) {
                sound --;
                if (sound==10) Mix_PlayChannel(1,beep,0);
            }
            SDL_Delay(FRAMETIME);
        }
        if (gfx_flag) {
            int w, h;
            SDL_GetWindowSize(disp,&w,&h);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_RenderClear(renderer);
            box.h = h/32-pixelSpace, box.w = w/64-pixelSpace;
            range(y,32) range(x,64) {
                box.x = w/64 * x;
                box.y = h/32 * y;
                pixVal = screen[y][x]*255;
                SDL_SetRenderDrawColor(renderer, pixVal, pixVal, pixVal,0);
                SDL_RenderFillRect(renderer,&box);
            }
            SDL_RenderPresent(renderer);
            gfx_flag = 0;
        }
    }
    ending:
    printf("\nWindow successfully closed.\n");
    return 0;
}
