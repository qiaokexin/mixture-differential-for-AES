#include <iostream>

# define GETU16(pt) (((uint16_t)(pt)[0] << 12) ^ ((uint16_t)(pt)[1] << 8) ^ ((uint16_t)(pt)[2] <<  4) ^ ((uint16_t)(pt)[3]))
# define PUTU16(ct, st) { (ct)[0] = (uint8_t)(((st) >> 12) & 0xf); (ct)[1] = (uint8_t)(((st) >> 8) & 0xf); (ct)[2] = (uint8_t)(((st) >>  4) & 0xf); (ct)[3] = (uint8_t)((st) & 0xf); }
//S-box
const uint8_t sBox[16] = {
  0x6, 0xB, 0x5, 0x4, 0x2, 0xE, 0x7, 0xA, 0x9, 0xD, 0xF, 0xC, 0x3, 0x1, 0x0, 0x8
};
const uint8_t rcon[16] = {0x1, 0x2, 0x4, 0x8, 0x3, 0x6, 0xc, 0xb, 0x5, 0xa, 0x7, 0xe, 0xf, 0xd, 0x9, 0x1};
uint8_t times2(uint8_t byte);
uint8_t times3(uint8_t byte);
void genConst();
int smallAES_set_encrypt_key(uint8_t * userkey, uint16_t *rk, int rd);
void fast_encryption(uint8_t * state,  uint16_t * rk, uint8_t * ciphertext, int N_Round);
void print_state_col(uint8_t state[16]);