#ifndef BASIC_H
#define BASIC_H

#include<vector>
#include<set>
#include <bitset>
#include<cmath>
#include<string>
#include<iostream>
#include<unordered_map>
#include<algorithm>

#include "gurobi_c++.h"
using namespace std;
const int MAX = 200000;
const int threads = 32;

const int eij_15feasible_pattern_tmplt[11][7]={0, 0, 0, 1, -1, 1, 0,\
1, 1, 0, -1, 0, 0, 0,\
0, 0, 0, 1, 1, -1, 0,\
-1, -1, 1, -1, 1, 1, 1,\
1, -1, 0, 1, 0, 0, 0,\
-1, 1, 0, 1, 0, 0, 0,\
1, 1, 1, -1, -1, -1, 1,\
1, 0, -1, 0, 1, 0, 0,\
0, 1, -1, 0, 0, 1, 0,\
0, -1, 1, 0, 0, 1, 0,\
-1, 0, 1, 0, 1, 0, 0};

const int mixtures[4][7]={2,2,1,1,0,0,-4,\
0,1,1,0,0,1,-2,\
0,-2,0,-1,1,-1,2,\
-2,0,-1,0,-1,1,2};

const int e_SB_ij_MC_e_ij_pure_tmple[8][9]={1, 1, 1, 1, 1, -4, 1, 1, 0,\
1, 1, 1, 1, -4, 1, 1, 1, 0,\
1, -4, 1, 1, 1, 1, 1, 1, 0,\
1, 1, 1, 1, 1, 1, 1, -4, 0,\
1, 1, 1, -4, 1, 1, 1, 1, 0,\
-4, 1, 1, 1, 1, 1, 1, 1, 0,\
1, 1, -4, 1, 1, 1, 1, 1, 0,\
1, 1, 1, 1, 1, 1, -4, 1, 0};

const int deA_e_AHAL_pattern[15][15] = {1, 2, 3, 3, 1, 2, -2, -1, -3, -1, -2, -1, -5, -3, 0, \
8, -14, -14, 4, 4, -10, -6, 6, 6, -1, -1, 7, 16, 6, 14, \
-4, 0, 4, -2, 2, -2, 4, -1, -3, 2, -2, 3, 6, 4, 0, \
-10, -14, 4, -14, 4, 8, 7, 6, -1, 6, -1, -6, 16, 6, 14, \
-14, 8, -14, 4, -10, 4, 6, -6, 6, -1, 7, -1, 16, 6, 14, \
4, -6, 4, 8, 4, -8, -2, 4, -6, -5, -1, 5, 0, 4, 2, \
-2, -4, 2, 0, -2, 4, 2, 4, -2, -1, 3, -3, 6, 4, 0, \
2, 4, -2, -2, 4, 6, -1, -1, 0, 0, -1, -3, -6, -4, 0, \
4, 2, -2, -4, 0, -2, -3, -2, 3, 4, -1, 2, 6, 4, 0, \
4, 4, 4, 4, 4, 4, -1, -1, -2, -3, -4, -4, -14, -10, 0, \
-2, 4, 4, 2, 4, -2, -2, -3, 1, -2, 0, 1, -6, -2, 0, \
4, -2, 4, 4, -2, 2, 1, -2, -3, 0, 1, -2, -6, -2, 0, \
0, 0, 0, -2, -2, -2, 1, 1, -2, 1, 2, 2, 4, 2, 2, \
-4, -3, -2, -4, -3, -2, -1, 3, 3, 2, 3, -1, 6, 2, 10, \
-2, -2, -2, -2, -2, -2, 2, 0, 2, 1, -1, 1, 3, 2, 7};

const int e_SB_ij_Delta_A_pattern_tmplt[8][9]={1, -1, 1, 1, -1, 1, -1, -2, 0,\
-1, 1, 1, 1, 1, -1, -1, -2, 0,\
1, 1, -1, -1, 1, 1, -1, -2, 0,\
1, 1, 1, -1, -1, -1, 1, 0, 0,\
1, -1, -1, 1, 1, -1, 1, 0, 0,\
-1, 1, -1, 1, -1, 1, 1, 0, 0,\
-1, -1, 1, -1, 1, 1, 1, 0, 0,\
0, 0, -1, 0, -1, -1, 1, 1, 2};

const int lab_dop_eij_tmp[13][9]={1, 1, -1, -1, 1, -1, 1, 1, 0,\
0, 0, 1, 1, 0, -1, 0, 0, 0,\
0, 0, 0, 0, 0, 1, 1, -1, 0,\
0, 0, 0, 0, 0, 1, -1, 1, 0,\
0, 0, 0, 1, -1, 0, 0, 1, 0,\
0, 0, 1, 0, 1, 0, -1, 0, 0,\
1, 1, 1, -1, -1, 1, 1, -1, 0,\
0, 0, -1, 1, 0, 1, 0, 0, 0,\
0, 0, 0, 1, 1, 0, 0, -1, 0,\
0, 0, -1, -1, 1, -1, 1, 1, 1,\
0, 0, 1, -1, 0, 1, 0, 0, 0,\
0, 0, 1, 0, -1, 0, 1, 0, 0,\
1, 1, 1, 1, 1, -1, -1, -1, 0};

const int lab_deA_dop_fAHAL_minus_pattern[4][6]={0, -1, 0, 0, -1, 1,\
0, 0, 1, 0, -1, 0,\
1, 1, -1, 0, 1, 0,\
-1, 0, 0, 0, -1, 1};

const int perm[16] = {0,5,10,15, 4,9,14,3, 8,13,2,7, 12,1,6,11};

void add_constraints_from_template(GRBModel&, vector<GRBVar>&, const int *, int, int);
void add_constraints_constant(GRBModel& , GRBVar* , string , int num);
string getVectorVarValue(GRBModel& , vector<GRBVar>);
vector<GRBVar> getVarVectorByName(GRBModel&, string, const int);
string value_to_pattern(unordered_map<string, string> );
void print_state_pattern(string);
void parse_sol(GRBModel& , int, int);
string find_first_col_pattern(GRBModel& , int );
void printPattern_latex(string s);
#endif // BASIC_H