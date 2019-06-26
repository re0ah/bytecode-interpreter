#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
extern uint16_t reg[14];
#define fl reg[8]
#define ip reg[9]
#define sp reg[10]
#define pr reg[11]
#define tmp0 reg[12]
#define tmp1 reg[13]
extern uint8_t ram[1 << 16];
#define IDT_SIZE 64 /*interrupt descriptor table located in first 64 bytes of RAM*/
uint16_t flen; /*file lenght in num of address*/
#define OPCODE_MASK 0x1f

static void execute(const uint8_t* const instr)
{
    static void(*callback_0[])(uint16_t* const val0, uint16_t val1) = 
    {
        _mov, _add, _sub, _mul, _div, _mod, _sal, _sar, _and, _or, _band, _bor, _xor, _cmp
    }; /*0-14*/
#define SIZE_CALLBACK_0 (sizeof(callback_0) / sizeof(callback_0[0]))
    static void(*callback_1[])(const uint16_t val) = 
    {
        _jmp, _jz, _jnz, _js, _jns, _jl, _jnl, _call
    }; /*0-7*/
#define SIZE_CALLBACK_1 (sizeof(callback_1) / sizeof(callback_1[0]))
    static void(*callback_2[])() = 
    {
        _pop, _not, _ret
    }; /*0-1*/
#define SIZE_CALLBACK_2 (sizeof(callback_2) / sizeof(callback_2[0]))
    uint8_t opcode = instr[0] & OPCODE_MASK; /*first 5 bits of 1-st byte is opcode*/
    uint8_t sender_receiver = instr[0] >> 5; /*last 3 bits of 1-st byte is types of arguments*/
    if((opcode >= OPCD_MOV) && (opcode <= OPCD_CMP))
    {
        const uint16_t arg_1 = ((uint16_t*)(instr + 1))[0];
        const uint16_t arg_2 = ((uint16_t*)(instr + 1))[1];
        printf("opcode = %x\n", opcode);
        printf("sender_receiver = %x\n", sender_receiver);
        printf("arg_1 = %x\n", arg_1);
        printf("arg_2 = %x\n", arg_2);
        switch (sender_receiver)
        {
            case SENDER_RECEIVER_RR: /*0x00*/
                callback_0[opcode](&reg[arg_1], reg[arg_2]);
                break;
            case SENDER_RECEIVER_RX: /*0x01*/
                callback_0[opcode](&reg[arg_1], arg_2);
                break;
            case SENDER_RECEIVER_RD: /*0x02*/
                callback_0[opcode](&reg[arg_1], *((uint16_t*)&ram[arg_2]));
                break;
            case SENDER_RECEIVER_RMR: /*0x03*/
                callback_0[opcode](&reg[arg_1], *((uint16_t*)&ram[reg[arg_2]]));
                break;
            case SENDER_RECEIVER_MRR: /*0x04*/
                callback_0[opcode]((uint16_t*)&ram[reg[arg_1]], reg[arg_2]);
                break;
            case SENDER_RECEIVER_MRM: /*0x05*/
                callback_0[opcode]((uint16_t*)&ram[reg[arg_1]], arg_2);
                break;
            case SENDER_RECEIVER_MRD: /*0x06*/
                callback_0[opcode]((uint16_t*)&ram[reg[arg_1]], arg_2);
                break;
            case SENDER_RECEIVER_MRMR: /*0x07*/
                callback_0[opcode]((uint16_t*)&ram[reg[arg_1]], arg_2);
                break;
            default:
                printf("bad sender_receiver\n");
        }
    }
    else if((opcode >= OPCD_JMP) && (opcode <= OPCD_CALL))
    {
        const uint16_t arg_1 = ((uint16_t*)(instr + 1))[0];
        callback_1[opcode - SIZE_CALLBACK_0 - 2](arg_1);
    }
    else if(opcode == OPCD_PUSH)
    {
        const uint16_t arg_1 = ((uint16_t*)(instr + 1))[0];
        printf("sender_receiver = %x\n", sender_receiver);
        printf("arg_1 = %x\n", arg_1);
        switch(sender_receiver)
        {
            case SEND_R:  /*0x00*/
                _push(reg[arg_1]);
                break;
            case SEND_MR: /*0x01*/
                _push(((uint16_t*)&ram)[reg[arg_1]]);
                break;
            case SEND_D:  /*0x02*/
                _push(arg_1);
                break;
            case SEND_DM: /*0x03*/
                _push(((uint16_t*)&ram)[arg_1]);
                break;
            default:
                printf("bad sender_receiver\n");
                break;
        }
    }
    else if((opcode >= OPCD_POP) && (opcode <= OPCD_RET))
    {
        callback_2[opcode - SIZE_CALLBACK_0 - SIZE_CALLBACK_1 - 4]();
    }
    else if(opcode == OPCD_IDT)
    {
        const uint16_t arg_1 = ((uint16_t*)(instr + 1))[0];
        const uint16_t arg_2 = ((uint16_t*)(instr + 1))[1];
        _idt(arg_1, arg_2);
    }
    else if(opcode == OPCD_BNOT)
    {
        const uint16_t arg_1 = ((uint16_t*)(instr + 1))[0];
        _bnot(&reg[arg_1]);
    }
}

void selection()
{
    static const uint8_t instruction_size[] = 
    {
        SIZE_MOV, SIZE_ADD, SIZE_SUB ,SIZE_MUL,
        SIZE_DIV, SIZE_MOD, SIZE_SAL ,SIZE_SAR,
        SIZE_AND, SIZE_OR , SIZE_BAND,SIZE_BOR,
        SIZE_XOR, SIZE_CMP, SIZE_BNOT,SIZE_IDT,
        SIZE_JMP,SIZE_JZ , SIZE_JNZ, SIZE_JS ,
        SIZE_JNS, SIZE_JL , SIZE_JNL, SIZE_CALL,
        SIZE_PUSH,SIZE_INT, SIZE_POP, SIZE_NOT,
        SIZE_RET
    };
#define SIZE_INSTRUCTION_SIZE (sizeof(instructon_size) / sizeof(instructon_size[0]))
    ip = ram[IDT_SIZE] + IDT_SIZE;/*start instruction segment*/
    /*stack pointer begin to highest address and grow down*/
    sp = (1 << 16) - 2;
    int i;
    uint8_t instr[5];
    uint16_t instr_size;
    while(1)
    {
        instr_size = instruction_size[ram[ip] & OPCODE_MASK];
        for(i = 0; i < instr_size; i++)
        {
            instr[i] = ram[ip++];
            printf("instr[%d] = %x\n", i, instr[i]);
        }
        execute(instr);
        memset(instr, 0, 5);
    }
}

void load_to_memory(char* filename)
{
    FILE* fptr = fopen(filename, "rb");
    if(fptr == NULL)
    {
        printf("fail fopen\n");
        exit(1);
    }
    /*calculate file tell.*/
    fseek(fptr, 0, SEEK_END);
    flen = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    fread(ram + IDT_SIZE, sizeof(uint8_t), flen, fptr);
    fclose(fptr);
}
