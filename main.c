#include "cpu.h"
uint16_t reg[14];
#define r0 reg[0]
#define r1 reg[1]
#define r2 reg[2]
#define r3 reg[3]
#define r4 reg[4]
#define r5 reg[5]
#define r6 reg[6]
#define r7 reg[7]
#define fl reg[8] /*flag register*/
/*
*bit 0: zero flag (if instruction result has 0, flag set set 1)
*bit 1: sign flag (equal to the highest bit of the result)
*bit 2: cmp flag  (0 - equal, 1 - not equal)
*bit 3: logic flag(result of logic instructions(and, or)
*/
#define ip reg[9] /*instruction pointer, current memory location*/
#define sp reg[10] /*stack pointer, grow down*/
#define pr reg[11] /*pop register, here return value in pop*/
#define tmp0 reg[12] /*temp register, used in assembler.*/
#define tmp1 reg[13] /*temp register, used in assembler. */
uint8_t ram[1 << 16];
#define IDT_SIZE 64 /*interrupt descriptor table located in first 64 bytes of RAM*/
int main()
{
    char* file = "bin";
    load_to_memory(file);
    selection();
    return 0;
}
