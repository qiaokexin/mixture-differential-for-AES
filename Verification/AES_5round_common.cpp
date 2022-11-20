/*
 * Quadruple distinguisher verification for CRAFT
 * Date: Dec 22, 2021
 * Author: Kexin Qiao
 * Contact: qiao.kexin@bit.edu.cn
 * 
 * small scale AES is from https://github.com/Krypto-iaik/Distinguisher_5RoundAES by Lorenzo Grassi
*/
// CRAFT Cipher 
//#define SDL_MAIN_HANDLED
#define __USE_MINGW_ANSI_STDIO 1  // to avoid %ll issue. special request for windows
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string>
#include <math.h>
#include "mpi.h"
#include <iostream>
#include <assert.h>
#include <random>
using namespace std;

typedef unsigned long long int UINT64;

std::mt19937 gen;
std::uniform_int_distribution<int> dis(0x0,0xf);
std::uniform_int_distribution<int> dis2(0x1,0xf);
std::uniform_int_distribution<int> dis_3bit(0x1,0x7);
std::uniform_int_distribution<int> dis_2bit(0x1,0x3);


//S-box
const uint8_t sBox[16] = {
  0x6, 0xB, 0x5, 0x4, 0x2, 0xE, 0x7, 0xA, 0x9, 0xD, 0xF, 0xC, 0x3, 0x1, 0x0, 0x8
};

//Inverse S-box
const uint8_t inv_s[16] = {
  0xE, 0xD, 0x4, 0xC, 0x3, 0x2, 0x0, 0x6, 0xF, 0x8, 0x7, 0x1, 0xB, 0x9, 0x5, 0xA
};

/*ByteTransformation*/

void byteSubTransformation(uint8_t p[4][4]){

  int i, j;

  for(i=0; i<4; i++){
    for(j=0; j<4; j++)
      p[i][j]=sBox[p[i][j]];
  }
}
//Multiplication
uint8_t multiplicationX(uint8_t byte){
    uint8_t bitTemp;

    bitTemp = (byte>>3) & 0x1;
    byte = (byte<<1) & 0xf;

    if(bitTemp==0)
        return byte;
    else
        return (byte^0x03);
}
//Multiplication byte n times
uint8_t multiplicationXN(uint8_t byte, int n){
    for (int i = 0; i < n; i++){
        byte = multiplicationX(byte);
    }
    return byte;
}

// Generate round key
void columnProcess(uint8_t *pCol, int numberofRound){
    uint8_t temp, rConst, colTemp[4];
    int i;

    //rotate 
    temp = *pCol;
    for (i = 0; i < 3; i++){
        colTemp[i] = *(pCol + i + 1);
    }
    colTemp[3] = temp;

    //s-box
    for (i = 0; i < 4; i++){
        colTemp[i] = sBox[colTemp[i]];
    }

    //
    if (numberofRound == 0){
        rConst = 0x01;
    }else{
        rConst = 0x02;
        for (i = 1; i < numberofRound; i++)
            rConst = multiplicationX(rConst);
    }

    colTemp[0] ^= rConst;

    //return
    for (i = 0; i < 4; i++){
        *(pCol + i) = colTemp[i];
    }
}

void generationRoundKey(uint8_t *pKey, int numberofRound){
    int i, j;
    uint8_t colTemp[4];

    for (i = 0 ; i < 4; i++){
        colTemp[i] = *(pKey + 3 + 4*i);
    }    

    columnProcess(&(colTemp[0]), numberofRound);

    for(i=0;i<4;i++)
        *(pKey+4*i)=*(pKey+4*i)^colTemp[i];

  
    for(i=1;i<4;i++){
        for(j=0;j<4;j++){
            *(pKey+i+4*j)=*(pKey+i+4*j)^*(pKey+i+4*j-1);
        }
    }
}

/*shift rows*/

void shiftRows(uint8_t p[4][4]){

  uint8_t temp[3];
  int i, j;

  for(i=1;i<4;i++){
    for(j=0;j<i;j++)
      temp[j]=p[i][j];

    for(j=0;j<(4-i);j++)
      p[i][j]=p[i][j+i];

    for(j=(4-i);j<4;j++)
      p[i][j]=temp[j-4+i];
  }

}
/*MixColumn*/

void mixColumn(uint8_t p[4][4]){

  int i, j;
  uint8_t col[4], tempCol[4];

  
  for(i=0;i<4;i++){

    
    for(j=0;j<4;j++){
      col[j]=p[j][i];
    }

    
    tempCol[0]= multiplicationX(col[0]) ^ multiplicationX(col[1]) ^ col[1] ^ col[2] ^ col[3];
    tempCol[1]= col[0] ^ multiplicationX(col[1]) ^ multiplicationX(col[2]) ^ col[2] ^ col[3];
    tempCol[2]= col[0] ^ col[1] ^ multiplicationX(col[2]) ^ multiplicationX(col[3]) ^ col[3];
    tempCol[3]= multiplicationX(col[0]) ^ col[0] ^ col[1] ^ col[2] ^ multiplicationX(col[3]);

    
    for(j=0;j<4;j++){
      p[j][i]=tempCol[j];
    }

  }

}

/*Add Round Key*/

void addRoundKey(uint8_t p[4][4], uint8_t key[][4]){

  int i, j;

  for(i=0; i<4; i++){
    for(j=0; j<4; j++){
      p[i][j] ^= key[i][j];
    }
  }

}

/*Encryption*/

void encryption( uint8_t initialState[][4],  uint8_t masterKey[][4], uint8_t ciphertext[4][4], int N_Round){

  int i, j;
  uint8_t state[4][4];
  uint8_t key[4][4];

  for (i = 0; i < 4; i++){
    for (j = 0; j < 4; j++){
        state[i][j] = initialState[i][j];
        key[i][j] = masterKey[i][j];
    }
  }
 //Initial Round
  addRoundKey(state, key);

  //Round
  for(i=0; i<N_Round-1; i++){
    generationRoundKey(&(key[0][0]), i);
    byteSubTransformation(state);
    shiftRows(state);
    mixColumn(state);
    addRoundKey(state, key);

  }

  //Final Round
  generationRoundKey(&(key[0][0]), N_Round-1);
  byteSubTransformation(state);
  shiftRows(state);
  addRoundKey(state, key);

  //store the ciphertext
  for(i=0; i<4; i++){
    for(j=0; j<4; j++)
      ciphertext[i][j]=state[i][j];
  }

}


void print_state(uint8_t state[16])
{
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            printf("%01x", state[4*i + j]);
        }
        
        printf("\n");
    }
        
}

void init_prng_mt(int offset){
    unsigned int initial_seed = 10*time(NULL) + offset;
    gen.seed(initial_seed);
    //printf("[+] PRNG initialized to 0x%08X\n", initial_seed);
}



void genQuadruple(uint8_t rd, uint8_t p0[16], uint8_t p1[16], uint8_t p2[16], uint8_t p3[16], string inpatt_state, uint8_t fix[]){
    int i;
    int j=0;
    //randcell = 0xA;
    for (i = 0; i < 16; i++){
        switch (inpatt_state[i]){
            case '-':
                p0[i] = dis(gen);               
                p1[i] = p0[i];
                p2[i] = p0[i];
                p3[i] = p0[i];
                break;
            case 's':
                p0[i] = dis(gen);
                p1[i] = p0[i];
                p2[i] = p0[i]^ dis2(gen);
                p3[i] = p2[i];
                break;
            case 'c':
                p0[i] = dis(gen);
                p1[i] = p0[i] ^ dis2(gen);           
                p2[i] = p0[i];
                p3[i] = p1[i];
                break;
            case 'x':
                p0[i] = dis(gen);
                p1[i] = p0[i] ^ dis2(gen); 
                p2[i] = p1[i];
                p3[i] = p0[i];
                break;
            default: // '*'
                p0[i] = dis(gen);
                p1[i] = dis(gen);
                p2[i] = dis(gen);
                p3[i] = dis(gen);
                break;
        }
    }
    
        
}

bool check_pattern(uint8_t c0[16], uint8_t c1[16], uint8_t c2[16], uint8_t c3[16], string patt){
    /*
    check if quadruple (c0,c1,c2,c3) satisfy patt.
    */
    bool flag = true;
    for (int i =0; i<16; i++){
        switch (patt[i]){
            case '-':
                if ((c0[i]==c1[i]) and (c0[i]==c2[i]) and (c0[i]==c3[i]) )
                    break;
                else{
                    flag = false;
                    break;
                }
            case 'c':
                if ((c0[i]==c2[i]) and (c1[i]==c3[i]))
                
                    break;
                else{
                    flag = false;
                    break;
                }
            case 'x':
                if ((c0[i]==c3[i]) and (c1[i]==c2[i]))
                    break;
                else{
                    flag = false;
                    break;
                }
            case 's':
                if ((c0[i]==c1[i]) and (c2[i]==c3[i]))
                    break;
                else{
                    flag = false;
                    break; 
                }   
            default:
                break;     
        }
    }
    return flag;
}
UINT64 * quadruple(uint8_t rd, uint8_t key[][4], UINT64 N3, string inpatt, string outpatt, uint8_t fix[]){
    /*
    Construct N3 quadruples in inpatt under a fixed tweakey.
    Return the number of right quadruples satisfying outpatt.
    */
    uint8_t p0[4][4],p1[4][4];
    uint8_t p2[4][4],p3[4][4];
    uint8_t c0[4][4],c1[4][4];
    uint8_t c2[4][4],c3[4][4];
    //bool debug = false;
    UINT64 bnum=0;
    static UINT64 count[2];
    count[0] = 0;
    count[1] = 0;
    while (bnum < N3){
        bnum++;
        genQuadruple(rd, &p0[0][0], &p1[0][0], &p2[0][0], &p3[0][0], inpatt, fix);
        /*
        cout<<"A quadruple "<<endl;
        print_state(&p0[0][0]);
        cout<<endl;
        print_state(&p1[0][0]);
        cout<<endl;
        print_state(&p2[0][0]);
        cout<<endl;
        print_state(&p3[0][0]);
        cout<<endl;
        */
        
        encryption(p0, key, c0, rd);
        encryption(p1, key, c1, rd);
        encryption(p2, key, c2, rd);
        encryption(p3, key, c3, rd);
        
        if (check_pattern(&c0[0][0], &c1[0][0], &c2[0][0], &c3[0][0],outpatt))
            count[0]++;
        
        encryption(p0, key, c0, 20);
        encryption(p1, key, c1, 20);
        encryption(p2, key, c2, 20);
        encryption(p3, key, c3, 20);
        if (check_pattern(&c0[0][0], &c1[0][0], &c2[0][0], &c3[0][0],outpatt))
            count[1]++;
        
    }
    return count;
}

UINT64 * testOnMultKey(uint8_t masterKey[][4], uint8_t rd, UINT64 N2, UINT64 N3, string inpatt, string outpatt, uint8_t fix[]){
    /*
    Set N2 multi masterkeys. Under each masterkey, test on N3 quadruples.
    Return sum of right quadruple numbers under each key.
    */
    
    
    UINT64 bnum=0;
    static UINT64 count[2];
    UINT64 *count_itr;
    count[0] = 0;
    count[1] = 0;
    
    while (bnum < N2){
        bnum++;
        //generate a random key
        

        count_itr = quadruple(rd, masterKey, N3, inpatt, outpatt, fix);
        count[0] += *count_itr;
        count[1] += *(count_itr + 1);
    }
    return count;
}

int main(int argc, char** argv){
    
    UINT64 N2 = 10; //= 1 << 4; //test under N2 keys
    UINT64 N3 = 15;;// = 1 << 10; //generate N3 quadruples under each key
    uint8_t rd = 1;
    string inpatt;
    string outpatt;

    
    rd = 5;
    inpatt = "--cxx--ccx---cx-";
    outpatt = "s******s**s**s**";
    //outpatt = "s***************";
    N2 = 1 << 10; //test under N2 keys
    N3 = 1 << 15; //generate N3 quadruples under each key
    uint8_t masterKey[4][4];// = {0x1, 0x5, 0x7, 0x7, 0x8, 0x9, 0xA, 0xD, 0xF, 0xC, 0xE, 0xD, 0x7, 0x8, 0xB, 0xD}; // Key 0
    
    int seed = stoi(argv[1]);
    init_prng_mt(0xBE889DB4+seed);
    for(int i = 0; i < 16; i++) masterKey[i/4][i%4] = dis(gen);

    uint8_t fix[16]; //fix inactive bytes to constants, i.e. plaintexts in a structure.
    for (int i = 0; i < 16; i++){
        fix[i] = dis(gen);
    }
    /*
    For each processor, run 
    testOnMultKey(uint8_t rd, int64_t N2, int64_t N3, string inpatt, string outpatt):
    Set N2 multi masterkeys. Under each masterkey, test on N3 quadruples.
    Return sum of right quadruple numbers under each key.
    */
    MPI_Init(NULL, NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    //printf("\n[+] Proc %d\n", world_rank);
    init_prng_mt(10*time(NULL)+ world_rank);
    //Compute on each process
    UINT64 *count;
    
    clock_t clock_timer;
    clock_timer = clock();
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    
    //init_prng_mt(world_rank);
    double proc_time;
    
    proc_time = MPI_Wtime();
    //print_state(&masterKey[0][0]);
    count = testOnMultKey(masterKey, rd,N2,N3,inpatt,outpatt,fix);
    proc_time = MPI_Wtime() - proc_time;
    
    MPI_Barrier(MPI_COMM_WORLD);
    //Gather all number of right quadruples down to the root process
    UINT64 *NUM = NULL;
    if (world_rank == 0){
        NUM = (UINT64 *)malloc(world_size * sizeof(UINT64) * 2);
        assert(NUM != NULL);
    }

    MPI_Gather(count, 2, MPI_UNSIGNED_LONG_LONG, NUM, 2, MPI_UNSIGNED_LONG_LONG,0,MPI_COMM_WORLD);

    //Compute sum on the root process
    if (world_rank == 0){
        UINT64 right,rand_right;
        right = 0;
        rand_right = 0;
        
        //printf("\n Quaries on each process = 2^(%f)\n",log(N2 * N3)/log(2));
        for (int i = 0; i < world_size *2; i=i+2){
            
            right += *(NUM + i);
            rand_right += *(NUM + i + 1);
            //printf(" %d proc:     Right = 2^(%f)\n", i/2, log(*(NUM + i))/log(2));
            //printf(" %d proc: randRight = 2^(%f)\n", i/2, log(*(NUM + i + 1))/log(2));
        }
        //printf("\n                              Total queries = 2^(%f) \n", log(N2 * N3 * world_size)/log(2));
        //printf("smallAES:  Number of total right quadruples = 2^(%f)\n", log(right)/log(2));
        //printf("           Probability of right quadruples  = 2^(-%0.4f)\n", (log(N2 * N3 * world_size)-log(right))/log(2));
        //printf(" RandPerm: Number of total right quadruples = 2^(%f)\n", log(rand_right)/log(2));
        //printf("            Probability of right quadruples = 2^(-%0.4f)\n", (log(N2 * N3 * world_size)-log(rand_right))/log(2));
        //cout << " time on clock(): " << ((double)clock() - clock_timer) / CLOCKS_PER_SEC<<endl<<endl;
        printf("%lld ", right);
        printf("%lld ", rand_right);
        printf("%f \n",((double)clock() - clock_timer) / CLOCKS_PER_SEC);
    }

    /*
    printf("[+] Time : %lf\n",proc_time);
    printf("[+] smallAES:      Number of right quadruples = 2^(%f)\n", log(*count)/log(2));
    printf("[+]           Probability of right quadruples = 2^(-%0.4f)\n\n", (log(N2 * N3)-log(*count))/log(2));
    printf("[+] RandPerm:      Number of right quadruples = 2^(%f)\n", log(*(count+1))/log(2));
    printf("[+]           Probability of right quadruples = 2^(-%0.4f)\n\n", (log(N2 * N3)-log(*(count+1)))/log(2));
    */
    //Clean up
    if (world_rank == 0){
        free(NUM);
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}

