#pragma once
#define OPCD_MOV  0x00
    #define SIZE_MOV  0x05

#define OPCD_ADD  0x01
    #define SIZE_ADD  0x05

#define OPCD_SUB  0x02
    #define SIZE_SUB  0x05

#define OPCD_MUL  0x03
    #define SIZE_MUL  0x05

#define OPCD_DIV  0x04
    #define SIZE_DIV  0x05

#define OPCD_MOD  0x05
    #define SIZE_MOD  0x05

#define OPCD_SAL  0x06
    #define SIZE_SAL  0x05

#define OPCD_SAR  0x07
    #define SIZE_SAR  0x05

#define OPCD_AND  0x08
    #define SIZE_AND  0x05

#define OPCD_OR   0x09
    #define SIZE_OR   0x05

#define OPCD_BAND 0x0a
    #define SIZE_BAND 0x05

#define OPCD_BOR  0x0b
    #define SIZE_BOR  0x05

#define OPCD_XOR  0x0c
    #define SIZE_XOR  0x05

#define OPCD_CMP  0x0d
    #define SIZE_CMP  0x05

#define OPCD_BNOT 0x0e
    #define SIZE_BNOT 0x05

#define OPCD_IDT  0x0f
    #define SIZE_IDT  0x05

#define OPCD_JMP  0x10
    #define SIZE_JMP  0x03

#define OPCD_JZ   0x11
    #define SIZE_JZ   0x03

#define OPCD_JNZ  0x12
    #define SIZE_JNZ  0x03

#define OPCD_JS   0x13
    #define SIZE_JS   0x03

#define OPCD_JNS  0x14
    #define SIZE_JNS  0x03

#define OPCD_JL   0x15
    #define SIZE_JL   0x03

#define OPCD_JNL  0x16
    #define SIZE_JNL  0x03

#define OPCD_CALL 0x17
    #define SIZE_CALL 0x03

#define OPCD_PUSH 0x18
    #define SIZE_PUSH 0x03

#define OPCD_INT  0x19
    #define SIZE_INT  0x03

#define OPCD_POP  0x1a
    #define SIZE_POP  0x01

#define OPCD_NOT  0x1b
    #define SIZE_NOT  0x01

#define OPCD_RET  0x1c
    #define SIZE_RET  0x01
/*------------------*/
/*it's just differences in arguments who being sent*/
#define SENDER_RECEIVER_RR   0x00   /*val reg = val reg*/
#define SENDER_RECEIVER_RX   0x01   /*val reg = address*/
#define SENDER_RECEIVER_RD   0x02   /*val reg = val at address*/
#define SENDER_RECEIVER_RMR  0x03   /*val reg = val at address in reg*/
#define SENDER_RECEIVER_MRR  0x04   /*val at address in reg = val reg*/
#define SENDER_RECEIVER_MRM  0x05   /*val at address in reg = address*/
#define SENDER_RECEIVER_MRD  0x06   /*val at address in reg = val at address*/
#define SENDER_RECEIVER_MRMR 0x07   /*val at address in reg = val at address in reg*/
#if 0
#define SENDER_RECEIVER_DR   0x08   /*val at address = val reg*/
#define SENDER_RECEIVER_DMR  0x09   /*val at address = val at address in reg*/
#endif
/*sender for push*/
#define SEND_R  0x00 /*val reg*/
#define SEND_MR 0x01 /*val at address in reg*/
#define SEND_D  0x02 /*val*/
#define SEND_DM 0x03 /*val at address*/
