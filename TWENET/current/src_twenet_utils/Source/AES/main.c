#include <string.h>
#include <stdio.h>
#include "ccm.h"

/* AES Key */
//long sKey[] = {0xc0c1c2c3, 0xc4c5c6c7, 0xc8c9cacb, 0xcccdcecf};
unsigned char sKey[] = {
    0xc0, 0xc1, 0xc2, 0xc3,
    0xc4, 0xc5, 0xc6, 0xc7,
    0xc8, 0xc9, 0xca, 0xcb,
    0xcc, 0xcd, 0xce, 0xcf };
//long sNonce[] = {0x000000a0, 0xa1a2a3a4, 0xa5a6a703, 0x02010006};
unsigned char sNonce[] = {
    0xa0, 0xa1, 0xa2, 0xa3,
    0xa4, 0xa5, 0xa6, 0xa7,
    0x03, 0x02, 0x01, 0x00,
    0x06, 0xcd, 0xce, 0xcf };

unsigned char au8authDataBuffer[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
/* input message data */
unsigned char au8inputPacketBuffer[23] =
{0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e};

unsigned char au8CheckSum[8];
ccm_ctx sCCM[1];

main() {
    int i;
    ret_type ret;
    
    memset(sCCM, 0, sizeof(sCCM));
    memset(au8CheckSum, 0, sizeof(au8CheckSum));
    
    for (i = 0; i < 23; i++) {
        printf("%02x", au8inputPacketBuffer[i]);
        if (i % 8 == 7) {
            printf("\n");
        }
    }
    printf("\n");
    
    ccm_init_and_key(
                     (unsigned char*)sKey, 16, sCCM);
	ret = ccm_encrypt_message(
		(unsigned char*)sNonce, 13,
		(unsigned char*)au8authDataBuffer, 8,
		au8inputPacketBuffer, 23,
		au8CheckSum, 8, sCCM);

    printf("%d\n", ret);

    for (i = 0; i < 23; i++) {
        printf("%02x", au8inputPacketBuffer[i]);
        if (i % 8 == 7) {
            printf("\n");
        }
    }
    printf("\n");
    for (i = 0; i < 8; i++) {
        printf("%02x", au8CheckSum[i]);
        if (i % 8 == 7) {
            printf("\n");
        }
    }
    printf("\n");
}
