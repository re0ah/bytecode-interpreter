#include "instructions.h"
#include "opcode.h"
#include <stdio.h>
#include <stdlib.h>
extern uint16_t reg[14];
#define fl reg[8]
#define ip reg[9]
#define sp reg[10]
#define pr reg[11]
#define tmp0 reg[12]
#define tmp1 reg[13]
extern uint8_t ram[1 << 16];
#define IDT_SIZE 64
struct call_stack
{
    uint16_t stack[32]; /*keep address where was made call*/
    uint8_t  cur_pos;
} cs = 
{
    {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0
    },
    0
};
#define check_signed(val)\
    if((*val >> 15) == 1)\
        fl |= (1 << 2);\
    else\
       fl &= ~(1 << 2);
#define check_zero(val)\
    if(*val0 == 0)\
        fl |= (1 << 3);\
    else\
        fl &= ~(1 << 3);

void _mov(uint16_t* const val0, const uint16_t val1)
{
    *val0 = val1;
};
void _add(uint16_t* const val0, const uint16_t val1)
{
    *val0 += val1;
    check_signed(val0);
    check_zero(val0);
};
void _sub(uint16_t* const val0, const uint16_t val1)
{
    *val0 -= val1;
    check_signed(val0);
    check_zero(val0);
};
void _mul(uint16_t* const val0, const uint16_t val1)
{
    *val0 *= val1;
    check_signed(val0);
    check_zero(val0);
};
void _div(uint16_t* const val0, const uint16_t val1)
{
    *val0 /= val1;
    check_signed(val0);
    check_zero(val0);
};
void _mod(uint16_t* const val0, const uint16_t val1)
{
    *val0 %= val1;
    check_signed(val0);
    check_zero(val0);
};
void _sal(uint16_t* const val0, const uint16_t val1)
{
    *val0 = *val0 << val1;
};
void _sar(uint16_t* const val0, const uint16_t val1)
{
    *val0 = *val0 >> val1;
};
void _and(uint16_t* const val0, const uint16_t val1)
{
    fl |= (val0 && val1) << 3;
};
void _or(uint16_t* const val0, const uint16_t val1)
{
    fl |= (val0 || val1) << 3;
};
void _not()
{
    fl = (fl & 8) == 0 ? 1 << 3 : 0;
};
void _bnot(uint16_t* const val)
{
    *val = ~(*val);
};
void _band(uint16_t* const val0, const uint16_t val1)
{
    *val0 &= val1;
};
void _bor(uint16_t* const val0, const uint16_t val1)
{
    *val0 |= val1;
};
void _xor(uint16_t* const val0, const uint16_t val1)
{
    *val0 ^= val1;
};
void _cmp(uint16_t* const val0, const uint16_t val1)
{
    fl = (*val0 == val1) ? fl | (1 << 2) : 0;
};
void _jmp(const uint16_t val)
{
    ip = val;
};
void _jz(const uint16_t val)
{
    ip = (fl & 1) == 0 ? val : ip;
    ip = (fl & 4) == 0 ? val : ip;
    printf("ip = %d\n", ip);
};
void _jnz(const uint16_t val)
{
    ip = (fl & 1) == 1 ? val : ip;
    ip = (fl & 4) == 4 ? val : ip;
    printf("ip = %d\n", ip);
};
void _js(const uint16_t val)
{
    ip = (fl & 2) == 2 ? val : ip;
};
void _jns(const uint16_t val)
{
    ip = (fl & 2) == 0 ? val : ip;
};
void _jl(const uint16_t val)
{
    ip = (fl & 8) == 8 ? val : ip;
};
void _jnl(const uint16_t val)
{
    ip = (fl & 8) == 0 ? val : ip;
};
void _push(const uint16_t val)
{
    ((uint16_t*)ram)[sp / 2] = val;
    sp -= 2;
};
void _pop()
{
    sp += 2;
    pr = ((uint16_t*)ram)[sp / 2];
};
void _call(const uint16_t label)
{
    cs.stack[cs.cur_pos++] = ip;
    ip = label + IDT_SIZE;
};
void _ret()
{
    if(cs.cur_pos == 0)
    {
        exit(0);
    }
    ip = cs.stack[--cs.cur_pos];
};
void _int(const uint16_t val)
{
    _call(ram[val]);
};
void _idt(const uint8_t num, const uint16_t label)
{
    ram[num] = label;
};
