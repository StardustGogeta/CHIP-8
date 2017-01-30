#define range(x,y) for(int x=0;x<y;x++)
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

// Select options here:
#define FRAMETIME 2
#define BOXSIZE 12
#define ROM_FILEPATH "c8games/PONG2"


#ifdef DEBUG
//#define opcodeDEBUG
#define debug printf
#else
#define debug //
#endif // DEBUG

#ifdef opcodeDEBUG
#define opc printf
#else
#define opc //
#endif // DEBUG

uint16_t stackMem[16], delay, sound, pc=0x200, I, sc=15, BCD, newX, newY;
SDL_Event keyEvent;
bool screen[32][64], gfx_flag, unpressed, bit;
uint8_t memory[4096], reg[16], *keyState,
		hexChars[80] = {0xF0, 0x90, 0x90, 0x90, 0xF0,
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

const char* mapToKeys = "X123QWEASDZC4RFV";
char* key;

uint8_t mapFromKeys (char keyInput) {
    switch (keyInput) {
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
            debug("\nFailed: %c\n",keyInput);
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

void dumpStackMem() {
    printf("StackMem Dump:\n");
    range(x,16) printf("%x\n",stackMem[x]);
    printf("Attempting to jump to position %x.\n",stackMem[sc]);
}

void runOpcode(uint16_t opcode) {
	uint8_t x0=opcode>>12,
			x1=opcode%0x1000>>8,
			x2=opcode%0x100>>4,
			x3=opcode%0x10;
	switch(x0) {
	case 0x0:
		switch(opcode) {
		case 0x00e0:
            opc("%04x: Clear the display.\n",opcode);
			memset(screen,0,sizeof(screen));
			break;
		case 0x00ee:
			opc("%04x: Return from subroutine.\n",opcode);
            sc++;
            // dumpStackMem();
            pc = stackMem[sc];
            break;
        default:
			opc("The opcode %04x is not implemented.\n",opcode);
			break;
		}
		break;
	case 0x1:
		// Jump
		pc = opcode%0x1000-2;
		break;
	case 0x2:
		// Call subroutine
		stackMem[sc] = pc;
		// printf("Storing %04x as return position.\n",pc);
		sc--;
		pc = opcode%0x1000-2;
		break;
	case 0x3:
		// Conditional skip
		if (reg[x1] == opcode%0x100) pc += 2;
		break;
	case 0x4:
		// Conditional skip
		if (reg[x1] != opcode%0x100) pc += 2;
		break;
	case 0x5:
		// Conditional skip
		if (reg[x1] == reg[x2]) pc += 2;
		break;
	case 0x6:
		// Var set
		reg[x1] = opcode%0x100;
		break;
	case 0x7:
		// Var add
		reg[x1] += opcode%0x100;
		break;
	case 0x8:
		switch(x3) {
		case 0x0:
			// Var set
			reg[x1] = reg[x2];
			break;
		case 0x1:
			// Var or
			reg[x1] |= reg[x2];
			break;
		case 0x2:
			// Var and
			reg[x1] &= reg[x2];
			break;
		case 0x3:
			// Var xor
			reg[x1] ^= reg[x2];
			break;
		case 0x4:
			// Var add
			reg[15] = reg[x1]+reg[x2]>0x100 ? 1 : 0;
			reg[x1] += reg[x2];
			break;
		case 0x5:
			// Var sub
			reg[15] = reg[x1]-reg[x2]<0x0 ? 0 : 1;
			reg[x1] -= reg[x2];
			break;
		case 0x6:
			// Var rshft
			reg[15] = reg[x1]%2;
			reg[x1] >>= 1;
			break;
		case 0x7:
			// Var sub
			reg[15] = reg[x2]-reg[x1]<0x0 ? 0 : 1;
			reg[x1] = reg[x2]-reg[x1];
			break;
		case 0xe:
			// Var lshft
			reg[15] = reg[x1]>>7;
			reg[x1] <<= 1;
			break;
		}
		break;
	case 0x9:
		// Condtnl skip
		if (reg[x1] != reg[x2]) pc += 2;
		break;
	case 0xa:
		// Set I
		I = opcode%0x1000;
		break;
	case 0xb:
		// Add and jump
		pc = reg[0]+opcode%0x1000-2;
		break;
	case 0xc:
		// Random and
		reg[x1] &= rand()%0x100;
		break;
	case 0xd:
		// Draw sprt
		debug("Attempting to draw to (%d, %d)...\n",reg[x1],reg[x2]);
		debug("Reg Dump:\n");
        range(x,16) {
            debug("%x ",reg[x]);
        }
        debug("\n");
		reg[0xf] = 0;
		range (Y, x3) {
            //reg[x] = 0xF0 = 0b11110000
            range (X, 8) {
                bit = (memory[I+Y]>>(7-X)) & 1;
                //if (reg[x2]+Y>31) printf("Overflow down!!!!!!!!!!!!\n");
                //if (reg[x1]+X>63) printf("Overflow right!!!!!!!!!!!!\n");
                newY = (reg[x2]+Y) % 32;
                newX = (reg[x1]+X) % 64;
                if (screen[newY][newX] & bit) reg[0xf] = 1;
                screen[newY][newX] ^= bit;
            }
		}
		gfx_flag = 1;
		break;
	case 0xe:
        // Keyboard input
        SDL_PumpEvents();
        key = mapToKeys[reg[x1]];
		switch (x2) {
            case 0x9:
                // Conditional skip
                debug("Trying to grab key %c with code %x.\n",key,reg[x1]);
                if (keyState[SDL_GetScancodeFromName(&key)]) {
                        pc += 2;
                }
                break;
            case 0xa:
                // Conditional skip
                unpressed = 1;
                debug("Trying to grab key %c with code %x.\n",key,reg[x1]);
                if (keyState[SDL_GetScancodeFromName(&key)]) {
                    unpressed = 0;
                }
                if (unpressed) pc += 2;
                break;
		}
		break;
	case 0xf:
		switch (opcode%0x100) {
		break;
		case 0x07:
			// Var set
			reg[x1] = delay;
			break;
		case 0x0a:
			// Keypress
			unpressed = 1;
			while (unpressed) {
                SDL_WaitEvent(&keyEvent);
                while(SDL_PollEvent(&keyEvent)) if (keyEvent.type == SDL_KEYDOWN) {
                    reg[x1] = mapFromKeys(SDL_GetKeyName(keyEvent.key.keysym.sym));
                    unpressed = 0;
                }
            }
			break;
		case 0x15:
			// Timer set
			delay = reg[x1];
			break;
		case 0x18:
			// Timer set
			sound = reg[x1];
			break;
		case 0x1e:
			// Var add
			I += reg[x1];
			break;
		case 0x29:
			// Character location
			I = 5*reg[x1];
			break;
		case 0x33:
			// BCD
            debug("reg[x1] is %x, I is %x.\n\n\n\n",reg[x1],I);
			BCD = reg[x1];
			memory[I] = BCD/100;
			memory[I+1] = (BCD%100)/10;
			memory[I+2] = BCD%10;
			//SDL_Delay(500);
			break;

		case 0x55:
			// Store vars
			range(var,x1+1) {
                memory[I+var] = reg[var];
            }
			break;
		case 0x65:
			// Set vars
			range(var,x1+1) {
                reg[var] = memory[I+var];
            }
			break;
		}
	}
}

int main(int argc, char* argv[])
{
    /* Test keyboard to input translation, and back again:
    for (uint8_t x=0x0;x<0xf;x++) {printf("Original: %x, Chip-8: %c, Revert: %x\n",x, mapToKeys[x], mapFromKeys(mapToKeys[x]));}
    */
	debug("Program start.\n");
	SDL_Init(SDL_INIT_EVERYTHING);
	atexit(SDL_Quit);
	uint8_t pixVal;
	const int pixelSize = BOXSIZE;
	SDL_version compiled;
	SDL_version linked;
	SDL_VERSION(&compiled);
    debug("We compiled against SDL version %d.%d.%d ...\n",
       compiled.major, compiled.minor, compiled.patch);
	SDL_GetVersion(&linked);
	debug("But we are linking against SDL version %d.%d.%d.\n",
       linked.major, linked.minor, linked.patch);
	SDL_Window* disp = SDL_CreateWindow("Stardust - Chip-8 Emulator",
										SDL_WINDOWPOS_UNDEFINED,
										SDL_WINDOWPOS_UNDEFINED,
										64*pixelSize, 32*pixelSize, 0
										// SDL_WINDOW_RESIZABLE
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
    const char* path = ROM_FILEPATH;
    loadRom(path);
    printf("Successfully loaded file %s.\n\nPress enter to quit.\n\n",path);
    uint16_t opcode;
    gfx_flag = 1;
	while (pc<4096) {
		while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    //pc = 4096;
                    goto ending;
                    //break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        //pc = 4096;
                        goto ending;
                    }
                    //break;
            }
            /*switch (event.window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
			{
				int w = 64*round(event.window.data1/64);
				SDL_SetWindowSize(disp,w,w/2);
				gfx_flag = 1;
				break;
			}
            }*/
        }
		opcode = memory[pc]<<8|memory[pc+1];
		debug("Attempting to run opcode %04x at position %04x\n", opcode, pc);
		runOpcode(opcode);
		pc += 2;
        if (gfx_flag) {
        	int w, h;
        	SDL_GetWindowSize(disp,&w,&h);
        	box.h = h/32-1, box.w = w/64-1;
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
        if (delay) {
            delay --;
        }
        if (sound) {
            sound --;
            if (sound==10) Mix_PlayChannel(1,beep,0);
        }
        SDL_Delay(FRAMETIME);
    }
    ending:
    printf("Window successfully closed.");
    return 0;
}
