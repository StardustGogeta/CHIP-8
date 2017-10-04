#include "main.h"
#include "display.h"

// Select options here:
#define FRAMETIME 2
#define OPS_REFRESH 1
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

void loadRom(const char* path) {
    FILE* ROM = fopen(path,"rb");
    fseek(ROM, 0, SEEK_END);
    int ROMsize = ftell(ROM);
    debug("The file size is %d bytes.\n", ROMsize);
    rewind(ROM);
    for (int x=0; x<ROMsize; x++)
        fscanf (ROM, "%c", &memory[0x200+x]);
    fclose(ROM);
    /* Example for printing hex values.
    printf("The first byte is %x\n",memory[512]);
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
    initializeWindow(BOXSIZE);
    debug("We compiled against SDL version %d.%d.%d ...\n",
       compiled.major, compiled.minor, compiled.patch);
    debug("But we are linking against SDL version %d.%d.%d.\n",
       linked.major, linked.minor, linked.patch);
    initializeAudio();
    debug("The sound has been initialized.\n");
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
    gfx_flag = 1;
    start = time(NULL);
    while (pc < 4096) {
        OPS ++;
        check = time(NULL);
        if (check - start >= OPS_REFRESH) {
            debug("Opcodes per second: %d\n",OPS/OPS_REFRESH);
            start = time(NULL);
            OPS = 0;
        }
        if (!handleEvents()) pc = 4096;
        if (!pause) {
            opcode = memory[pc]<<8|memory[pc+1];
            opc("Attempting to run opcode %04x at position %04x\n", opcode, pc);
            runOpcode(opcode);
            pc += 2;
            if (delay) {
                delay --;
            }
            if (sound) {
                sound --;
                if (sound == 10) playBeep();
            }
            delayTime(FRAMETIME);
        }
        if (gfx_flag) drawGraphics();
    }
    printf("\nWindow successfully closed.\n");
    return 0;
}
