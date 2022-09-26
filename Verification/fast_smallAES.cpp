#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "fast_smallAES.h"

using namespace std;


//Multiplication
uint8_t times2(uint8_t byte){
    uint8_t bitTemp;

    bitTemp = (byte>>3) & 0x1;
    byte = (byte<<1) & 0xf;

    if(bitTemp==0)
        return byte;
    else
        return (byte^0x03);
}

uint8_t times3(uint8_t byte){
    
    return (times2(byte)^byte);
}

/*
Te0[x] = S [x].[02, 01, 01, 03];
Te1[x] = S [x].[03, 02, 01, 01];
Te2[x] = S [x].[01, 03, 02, 01];
Te3[x] = S [x].[01, 01, 03, 02];
*/
uint16_t Te0[16] = {0xc66a, 0x5bbe, 0xa55f, 0x844c, 0x4226, 0xfee1, 0xe779, 0x7aad, 0x1998, 0x9dd4, 0xdff2, 0xbcc7, 0x6335, 0x2113, 0x0, 0x388b};
uint16_t Te1[16]={0xac66, 0xe5bb, 0xfa55, 0xc844, 0x6422, 0x1fee, 0x9e77, 0xd7aa, 0x8199, 0x49dd, 0x2dff, 0x7bcc, 0x5633, 0x3211, 0x0, 0xb388};
uint16_t Te2[16]={0x6ac6, 0xbe5b, 0x5fa5, 0x4c84, 0x2642, 0xe1fe, 0x79e7, 0xad7a, 0x9819, 0xd49d, 0xf2df, 0xc7bc, 0x3563, 0x1321, 0x0, 0x8b38};
uint16_t Te3[16]={0x66ac, 0xbbe5, 0x55fa, 0x44c8, 0x2264, 0xee1f, 0x779e, 0xaad7, 0x9981, 0xdd49, 0xff2d, 0xcc7b, 0x3356, 0x1132, 0x0, 0x88b3};

void genTe0(){
    uint16_t element;
    printf("Te0: ");
    for (int i = 0; i < 16; i++){
        
        element = (times2(sBox[i])<<12) ^ (sBox[i] << 8) ^ (sBox[i] << 4) ^ times3(sBox[i]);
        printf("0x%01x, ", element);
    }
    printf("\n");
}

void genTe1(){
    uint16_t element;
    printf("Te1: ");
    for (int i = 0; i < 16; i++){
        
        element = (times3(sBox[i])<<12) ^ (times2(sBox[i]) << 8) ^ (sBox[i] << 4) ^ sBox[i];
        printf("0x%01x, ", element);
    }
    printf("\n");
}
void genTe2(){
    uint16_t element;
    printf("Te2: ");
    for (int i = 0; i < 16; i++){
        
        element = (sBox[i]<<12) ^ (times3(sBox[i]) << 8) ^ (times2(sBox[i]) << 4) ^ sBox[i];
        printf("0x%01x, ", element);
    }
    printf("\n");
}
void genTe3(){
    uint16_t element;
    printf("Te3: ");
    for (int i = 0; i < 16; i++){
        
        element = (sBox[i]<<12) ^ (sBox[i] << 8) ^ (times3(sBox[i]) << 4) ^ times2(sBox[i]);
        printf("0x%01x, ", element);
    }
    printf("\n");
}
void genConst(){
    uint8_t cst = 1;
    for (int i = 1; i< 16;i++){
        cst = times2(cst);
        printf("0x%01x, ", cst);
    }
    printf("\n");
}
//Generate round key
int smallAES_set_encrypt_key(uint8_t * userkey, uint16_t *rk, int rd){
    uint16_t temp;
    int i;
    rk[0] = GETU16(userkey     );
    rk[1] = GETU16(userkey +  4);
    rk[2] = GETU16(userkey +  8);
    rk[3] = GETU16(userkey + 12);
    i=0;
    while (1){
        temp = rk[3];
        rk[4] = rk[0] ^ 
            (Te2[(temp >>  8) & 0xf] & 0xf000) ^
            (Te3[(temp >>  4) & 0xf] & 0x0f00) ^
            (Te0[(temp      ) & 0xf] & 0x00f0) ^
            (Te1[(temp >> 12)      ] & 0x000f) ^
            ((rcon[i] << 12) & 0xffff);
        rk[5] = rk[1] ^ rk[4];
        rk[6] = rk[2] ^ rk[5];
        rk[7] = rk[3] ^ rk[6];
        if (++i == rd) {
            return 0;
        }
        rk += 4;
    }
}
void print_rk(uint16_t * key){
    for (int i = 0; i<4; i++){
        printf("%01x,", *(key+i));
    }
    cout<<endl;
}
void fast_encryption(uint8_t * state,  uint16_t * rk, uint8_t * ciphertext, int N_Round){
    uint16_t s0, s1, s2, s3, t0, t1, t2, t3;
    uint8_t temp;
    s0 = GETU16(state) ^ rk[0];
    s1 = GETU16(state + 4) ^ rk[1];
    s2 = GETU16(state + 8) ^ rk[2];
    s3 = GETU16(state + 12) ^ rk[3];
    
    int r;
    r = (N_Round >> 1);
    
    for (;;){
        t0 = Te0[s0 >> 12] ^ Te1[(s1 >> 8) & 0xf] ^ Te2[(s2 >>  4) & 0xf] ^ Te3[s3 & 0xf] ^ rk[ 4];
   	    t1 = Te0[s1 >> 12] ^ Te1[(s2 >> 8) & 0xf] ^ Te2[(s3 >>  4) & 0xf] ^ Te3[s0 & 0xf] ^ rk[ 5];
   	    t2 = Te0[s2 >> 12] ^ Te1[(s3 >> 8) & 0xf] ^ Te2[(s0 >>  4) & 0xf] ^ Te3[s1 & 0xf] ^ rk[ 6];
   	    t3 = Te0[s3 >> 12] ^ Te1[(s0 >> 8) & 0xf] ^ Te2[(s1 >>  4) & 0xf] ^ Te3[s2 & 0xf] ^ rk[ 7];
        
        rk += 8;
        
        if (--r == 0){
            break;
        }
                
        s0 = Te0[(t0 >> 12) & 0xf] ^ Te1[(t1 >> 8) & 0xf] ^ Te2[(t2 >>  4) & 0xf] ^ Te3[t3 & 0xf] ^ rk[ 0];    
   	    s1 = Te0[t1 >> 12] ^ Te1[(t2 >> 8) & 0xf] ^ Te2[(t3 >>  4) & 0xf] ^ Te3[t0 & 0xf] ^ rk[ 1];
   	    s2 = Te0[t2 >> 12] ^ Te1[(t3 >> 8) & 0xf] ^ Te2[(t0 >>  4) & 0xf] ^ Te3[t1 & 0xf] ^ rk[ 2];
   	    s3 = Te0[t3 >> 12] ^ Te1[(t0 >> 8) & 0xf] ^ Te2[(t1 >>  4) & 0xf] ^ Te3[t2 & 0xf] ^ rk[ 3];
        
    }
    
    if ((N_Round & 0x1 )== 1){
        s0 = Te0[t0 >> 12] ^ Te1[(t1 >> 8) & 0xf] ^ Te2[(t2 >>  4) & 0xf] ^ Te3[t3 & 0xf] ^ rk[ 0 ];
   	    s1 = Te0[t1 >> 12] ^ Te1[(t2 >> 8) & 0xf] ^ Te2[(t3 >>  4) & 0xf] ^ Te3[t0 & 0xf] ^ rk[ 1];
   	    s2 = Te0[t2 >> 12] ^ Te1[(t3 >> 8) & 0xf] ^ Te2[(t0 >>  4) & 0xf] ^ Te3[t1 & 0xf] ^ rk[ 2];
   	    s3 = Te0[t3 >> 12] ^ Te1[(t0 >> 8) & 0xf] ^ Te2[(t1 >>  4) & 0xf] ^ Te3[t2 & 0xf] ^ rk[ 3];


        t0 = (Te2[s0 >> 12       ] & 0xf000) ^ 
             (Te3[(s1 >> 8) & 0xf] & 0x0f00) ^ 
             (Te0[(s2 >> 4) & 0xf] & 0x00f0) ^ 
             (Te1[s3        & 0xf] & 0x000f) ^ 
             rk[ 4];
        PUTU16(ciphertext    , t0);
   	    t1 = (Te2[s1 >> 12       ] & 0xf000) ^ 
             (Te3[(s2 >> 8) & 0xf] & 0x0f00) ^ 
             (Te0[(s3 >> 4) & 0xf] & 0x00f0) ^ 
             (Te1[s0        & 0xf] & 0x000f) ^ 
             rk[ 5];
        PUTU16(ciphertext  + 4, t1);
   	    t2 = (Te2[s2 >> 12       ] & 0xf000) ^ 
             (Te3[(s3 >> 8) & 0xf] & 0x0f00) ^ 
             (Te0[(s0 >> 4) & 0xf] & 0x00f0) ^ 
             (Te1[s1        & 0xf] & 0x000f) ^ 
            rk[ 6];
        PUTU16(ciphertext  + 8, t2);
   	    t3 = (Te2[s3 >> 12       ] & 0xf000) ^ 
             (Te3[(s0 >> 8) & 0xf] & 0x0f00) ^ 
             (Te0[(s1 >> 4) & 0xf] & 0x00f0) ^ 
             (Te1[s2        & 0xf] & 0x000f) ^ 
             rk[ 7];
        PUTU16(ciphertext  + 12, t3);

                
    } else{
        
        s0 =
            (Te2[(t0 >> 12)      ] & 0xf000) ^
            (Te3[(t1 >>  8) & 0xf] & 0x0f00) ^
            (Te0[(t2 >>  4) & 0xf] & 0x00f0) ^
            (Te1[(t3      ) & 0xf] & 0x000f) ^
            rk[0];
            
	    
        PUTU16(ciphertext    , s0);
        
	    s1 =
            (Te2[(t1 >> 12)     ] & 0xf000) ^
            (Te3[(t2 >> 8) & 0xf] & 0x0f00) ^
            (Te0[(t3 >> 4) & 0xf] & 0x00f0) ^
            (Te1[(t0     ) & 0xf] & 0x000f) ^
            rk[1];
	    PUTU16(ciphertext +  4, s1);
	    s2 =
            (Te2[(t2 >> 12)     ] & 0xf000) ^
            (Te3[(t3 >> 8) & 0xf] & 0x0f00) ^
            (Te0[(t0 >> 4) & 0xf] & 0x00f0) ^
            (Te1[(t1     ) & 0xf] & 0x000f) ^
            rk[2];
        PUTU16(ciphertext +  8, s2);
        s3 =
            (Te2[(t3 >> 12)      ] & 0xf000) ^
            (Te3[(t0 >> 8)  & 0xf] & 0x0f00) ^
            (Te0[(t1 >>  4) & 0xf] & 0x00f0) ^
            (Te1[(t2      ) & 0xf] & 0x000f) ^
            rk[3];
        PUTU16(ciphertext + 12, s3);
    }
}
void print_state_col(uint8_t state[16])
{
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            printf("%01x", state[4*j + i]);
        }
        
        printf("\n");
    }
        
}
/*
int main(){
    genTe0();
    genTe1();
    genTe2();
    genTe3();
    genConst();
    return 0;
}
*/