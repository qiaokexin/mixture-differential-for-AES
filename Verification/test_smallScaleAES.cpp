#define __USE_MINGW_ANSI_STDIO 1  // to avoid %ll issue. special request for windows
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string>
#include <math.h>

#include <iostream>
#include <assert.h>
#include <random>
#include "fast_smallAES.h"
using namespace std;



int main(){

  uint8_t message[16] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf};
  uint8_t key[16] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf};
  const int rd=10;
  uint8_t cipher[16];
  uint16_t  rk[4*(rd+1)];
  

  printf("plaintext\n");
  print_state_col(message);
  printf("\n");

  printf("key\n");
  print_state_col(key);
  printf("\n");

  printf("ciphertext\n");
  smallAES_set_encrypt_key(key, rk, rd);
  fast_encryption(message, rk, cipher, rd);
  print_state_col(cipher);
  
    return 0;
}