#include "main.h"

void opc(const char *str, ...) {
    #ifdef opcodeDEBUG
    va_list l;
    va_start(l, str);
    vprintf(str, l);
    #else
    return;
    #endif // DEBUG
}

void dumpStackMem() {
    debug("StackMem Dump:\n");
    range(x,16) debug("%x\n",stackMem[x]);
    debug("Attempting to jump to position %x.\n",stackMem[sc]);
}

void dumpRegMem() {
    debug("Reg Dump:\n");
    range(x,16) debug("%x ",reg[x]);
    debug("\n");
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
        // dumpRegMem();
        opc("Attempting to draw to (%d, %d)...\n",reg[x1],reg[x2]);
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
                opc("Trying to grab key %c with code %x.\n",key,reg[x1]);
                if (keyState[SDL_GetScancodeFromName(&key)]) {
                        pc += 2;
                }
                break;
            case 0xa:
                // Conditional skip
                unpressed = 1;
                opc("Trying to grab key %c with code %x.\n",key,reg[x1]);
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
            opc("reg[x1] is %x, I is %x.\n\n\n\n",reg[x1],I);
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
