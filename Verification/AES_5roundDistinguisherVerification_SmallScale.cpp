/*
 * Quadruple distinguisher verification for small scale AES
 * Date: Sep 22, 2022
 * Author: Kexin Qiao
 * Contact: qiao.kexin@bit.edu.cn
 * 
*/

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
#include "fast_smallAES.h"
using namespace std;

typedef unsigned long long int UINT64;

std::mt19937 gen;
std::uniform_int_distribution<int> dis(0x0,0xf);
std::uniform_int_distribution<int> dis2(0x1,0xf);

void init_prng_mt(int offset){
    unsigned int initial_seed = 10*time(NULL) + offset;
    gen.seed(initial_seed);
    //printf("[+] PRNG initialized to 0x%08X\n", initial_seed);
}



void genQuadruple(uint8_t rd, uint8_t p0[16], uint8_t p1[16], uint8_t p2[16], uint8_t p3[16], string inpatt_state){
    int i;
    
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
    check if quadruple (c0,c1,c2,c3) satisfy pattern.
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
UINT64 * quadruple(uint8_t rd, uint16_t  *key, UINT64 N3, string inpatt, string outpatt){
    /*
    Construct N3 quadruples in inpatt under fixed round key.
    Return the number of right quadruples satisfying outpatt.
    */
    uint8_t p0[16],p1[16];
    uint8_t p2[16],p3[16];
    uint8_t c0[16],c1[16];
    uint8_t c2[16],c3[16];
    //bool debug = false;
    UINT64 bnum=0;
    static UINT64 count[2];
    count[0] = 0;
    count[1] = 0;
    while (bnum < N3){
        bnum++;
        genQuadruple(rd, p0, p1, p2, p3, inpatt);
        
        
        fast_encryption(p0, key, c0, rd);
        fast_encryption(p1, key, c1, rd);
        fast_encryption(p2, key, c2, rd);
        fast_encryption(p3, key, c3, rd);
        
        if (check_pattern(c0, c1, c2, c3,outpatt))
            count[0]++;
        
        fast_encryption(p0, key, c0, 20);
        fast_encryption(p1, key, c1, 20);
        fast_encryption(p2, key, c2, 20);
        fast_encryption(p3, key, c3, 20);
        if (check_pattern(c0, c1, c2, c3,outpatt))
            count[1]++;
        
    }
    return count;
}

UINT64 * testOnMultKey(uint16_t * rk, uint8_t rd, UINT64 N2, UINT64 N3, string inpatt, string outpatt){
  
    UINT64 bnum=0;
    static UINT64 count[2];
    UINT64 *count_itr;
    count[0] = 0;
    count[1] = 0;   
    
    while (bnum < N2){
        bnum++;
        /*
        If needed, set N2 multi masterkeys here. Under each masterkey, test on N3 quadruples.
        Return sum of right quadruple numbers under each key.
        */
        count_itr = quadruple(rd, rk, N3, inpatt, outpatt);
        count[0] += *count_itr;
        count[1] += *(count_itr + 1);
    }
    return count;
}

int main(int argc, char** argv){
    
    UINT64 N2 = 1; //test under N2 keys if needed
    UINT64 N3 = 1;;//generate N3 quadruples under each key
    uint8_t rd = 1;
    string inpatt;
    string outpatt;

    
    rd = 5;
    /* inpatt
    --cx
    x--c
    cx--
    -cx-

    outpatt
    s***
    ***s
    **s*
    *s**
    */
    inpatt = "-xc---xcc--xxc--";
    outpatt = "s******s**s**s**";
    
    N2 = 1 << 10; //test under N2 keys
    N3 = 1 << 14; //generate N3 quadruples under each key
    uint8_t masterKey[16];// = {0x1, 0x5, 0x7, 0x7, 0x8, 0x9, 0xA, 0xD, 0xF, 0xC, 0xE, 0xD, 0x7, 0x8, 0xB, 0xD}; 
    
    int seed = stoi(argv[1]); 
    init_prng_mt(0xBE889DB4+seed);
    for(int i = 0; i < 16; i++) masterKey[i] = dis(gen);
    //print_state_col(masterKey);
    uint16_t rk[4*(rd+1)];

    //Generate round key
    smallAES_set_encrypt_key(masterKey, rk, rd);
       
    /*
    For each processor, run 
    testOnMultKey(uint8_t rd, int64_t N2, int64_t N3, string inpatt, string outpatt):
    */
    MPI_Init(NULL, NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    //printf("\n[+] Proc %d\n", world_rank);
    init_prng_mt(world_rank);
    //Compute on each process
    UINT64 *count;
    
    clock_t clock_timer;
    clock_timer = clock();
    
    MPI_Barrier(MPI_COMM_WORLD);
   
    double proc_time;    
    proc_time = MPI_Wtime();
    count = testOnMultKey(rk, rd,N2,N3,inpatt,outpatt);
    
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
        //print_state_col(masterKey);
        //printf("\n Quaries on each process = 2^(%f)\n",log(N2 * N3)/log(2));
        for (int i = 0; i < world_size *2; i=i+2){
            
            right += *(NUM + i);
            rand_right += *(NUM + i + 1);
            //printf(" %d proc:     Right = 2^(%f)\n", i/2, log(*(NUM + i))/log(2));
            //printf(" %d proc: randRight = 2^(%f)\n", i/2, log(*(NUM + i + 1))/log(2));
        }
        //printf("\n                              Total queries = 2^(%f) \n", log(N2 * N3 * world_size)/log(2));
        //printf("smallAES:  Number of total right quadruples = 2^(%f)\n", log(right)/log(2));
        printf("%lld ", right);
        //printf("           Probability of right quadruples  = 2^(-%0.4f)\n", (log(N2 * N3 * world_size)-log(right))/log(2));
        //printf(" RandPerm: Number of total right quadruples = 2^(%f)\n", log(rand_right)/log(2));
        printf("%lld \n", rand_right);
        //printf("            Probability of right quadruples = 2^(-%0.4f)\n", (log(N2 * N3 * world_size)-log(rand_right))/log(2));
        //cout << " time on clock(): " << ((double)clock() - clock_timer) / CLOCKS_PER_SEC<<endl<<endl;
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

