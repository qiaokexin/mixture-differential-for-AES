
/*
 * Searching for mixture differential distinguishers for 5- and 6- round AES
 * Date: Sep 22, 2022
 * Author: Kexin Qiao
 * Contact: qiao.kexin@bit.edu.cn
 * 
*/

#include "basic.h"
#include <unordered_map>

vector<GRBVar> addVar_16(GRBModel& model, int r, string s){
    vector<GRBVar> x(16);
    for (int i=0; i < 16; i++){
        x[i] = model.addVar(0, 1, 0, GRB_BINARY, s + to_string(r) + "_" + to_string(i));
    }
    model.update();
    return x;
}

vector<GRBVar> addVar_4(GRBModel& model, int r, string s){
    vector<GRBVar> x(4);
    for (int i=0; i < 4; i++){
        x[i] = model.addVar(0, 1, 0, GRB_BINARY, s + to_string(r) + "_" + to_string(i));
    }
    model.update();
    return x;
}
void genConstraints_of_Round(GRBModel& model, int r, unordered_map<string, vector<GRBVar>> e_map, unordered_map<string, vector<GRBVar>> er_map){

    for (int i = 0; i<16; i++){
        vector<GRBVar> e_Ind = {er_map["01"][i], er_map["02"][i], er_map["03"][i], er_map["12"][i], er_map["13"][i], er_map["23"][i]};
       
        add_constraints_from_template(model, e_Ind, eij_15feasible_pattern_tmplt[0], 11, 7);
    }
    
    //deA vars
    unordered_map<string, vector<GRBVar>> deA_map;
    for (auto& x: e_map){ //x.first is string "01"/"02"/.../"23"
        deA_map[x.first] = addVar_4(model, r-1, "deA"+x.first+"_");
    }

    //finalAH, finalAL vars
    vector<GRBVar> finalAH(16), finalAL(16);
    finalAH = addVar_16(model, r-1, "finalAH_");
    finalAL = addVar_16(model, r-1, "finalAL_");

    for (auto& x: e_map){ //x.first is string "01"/"02"/.../"23"
        vector<GRBVar> eij_SR(16);
        for (int i=0; i<16; i++){
            eij_SR[i]=x.second[perm[i]];
        }
        
        //each column 
        for (int i = 0; i < 4; i++){
            vector<GRBVar> var_list = {eij_SR[4*i], eij_SR[4*i+1], eij_SR[4*i+2], eij_SR[4*i+3], er_map[x.first][4*i], er_map[x.first][4*i+1], er_map[x.first][4*i+2], er_map[x.first][4*i+3]};            
            add_constraints_from_template(model, var_list, e_SB_ij_MC_e_ij_pure_tmple[0], 8, 9);         

            model.addGenConstrOr(deA_map[x.first][i], &eij_SR[4*i], 4, "deA"+x.first+"_"+to_string(r-1)+"_"+to_string(i)+"_def");

            model.update();
        }       
    }
    
    
    for(int i=0; i<4; i++){       
        for (int j = 0; j < 4; j++){
            vector<GRBVar> deA_e_fAHAL = {deA_map["01"][i], deA_map["02"][i], deA_map["03"][i], deA_map["12"][i], deA_map["13"][i], deA_map["23"][i], \
            er_map["01"][4*i+j], er_map["02"][4*i+j], er_map["03"][4*i+j], er_map["12"][4*i+j], er_map["13"][4*i+j], er_map["23"][4*i+j], \
            finalAH[4*i+j], finalAL[4*i+j]};

            add_constraints_from_template(model, deA_e_fAHAL, deA_e_AHAL_pattern[0], 15, 15);

        }
    }
   
    vector<GRBVar> Delta_SB(16), A_SB(16);
    Delta_SB = addVar_16(model, r-1, "Delta_SB_");
    A_SB = addVar_16(model, r-1, "A_SB_");

    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            vector<GRBVar> var_list = {e_map["01"][4*i+j], e_map["02"][4*i+j], e_map["03"][4*i+j], e_map["12"][4*i+j], e_map["13"][4*i+j], e_map["23"][4*i+j], Delta_SB[4*i+j], A_SB[4*i+j]};

            add_constraints_from_template(model, var_list, e_SB_ij_Delta_A_pattern_tmplt[0], 8, 9);
        }
    }

    vector<GRBVar> dop(4);
    dop = addVar_4(model, r-1, "dop_");

    vector<GRBVar> Delta_SB_SR(16);
    for (int i = 0; i < 16; i++){
        Delta_SB_SR[i] =  Delta_SB[perm[i]];
    }

    for (int i = 0; i < 4; i++){
        model.addGenConstrOr(dop[i], &Delta_SB_SR[4*i], 4, "dop_"+to_string(r-1)+"_"+to_string(i)+"def");
        model.update();
    }

    vector<GRBVar> lab(4);
    lab = addVar_4(model, r-1, "lab_");
    
    for (int i = 0; i < 4; i++){
        for(auto& x:deA_map){
            model.addConstr(x.second[i] + lab[i] >= 1);
            model.update();
        }
        model.addConstr(lab[i] + deA_map["01"][i] + deA_map["02"][i] + deA_map["03"][i] + deA_map["12"][i] + deA_map["13"][i] + deA_map["23"][i] <= 6);
        model.update();
    }
    //lab=0, dop=0, constrain e_Ind to mixtures
    for (int i=0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            vector<GRBVar> var_list = {lab[i], dop[i], er_map["01"][4*i+j], er_map["02"][4*i+j], er_map["03"][4*i+j], er_map["12"][4*i+j], er_map["13"][4*i+j], er_map["23"][4*i+j]};
            add_constraints_from_template(model, var_list, lab_dop_eij_tmp[0], 13, 9);
        }
    }

    vector<GRBVar> minus(16);
    minus = addVar_16(model, r-1, "minus_");
    
    for(int i=0; i< 4; i++){
        for (int j=0; j < 4; j++){
            vector<GRBVar> var_list = {lab[i], dop[i], finalAH[4*i+j], finalAL[4*i+j], minus[4*i+j]};
            add_constraints_from_template(model, var_list, lab_deA_dop_fAHAL_minus_pattern[0], 4, 6);
        }
    }
    
}

void genModel(int r, float objbound, int printMode){
    GRBEnv env = GRBEnv();
    
    if (objbound == INFINITY){
        env.set(GRB_IntParam_LogToConsole, 1);
        env.set(GRB_IntParam_PoolSearchMode, 0);  //find an optimal solution
    }else{
        env.set(GRB_IntParam_LogToConsole, 0);
        env.set(GRB_IntParam_PoolSearchMode, 2);  //PoolSearchMode
    }
    
    env.set(GRB_IntParam_Threads, threads );
    env.set(GRB_IntParam_PoolSolutions, MAX); 

    GRBModel model = GRBModel(env);

    vector<GRBVar> e01(16), e02(16), e03(16), e12(16), e13(16), e23(16);
    e01 = addVar_16(model, 0, "e01_");
    e02 = addVar_16(model, 0, "e02_");
    e03 = addVar_16(model, 0, "e03_");
    e12 = addVar_16(model, 0, "e12_");
    e13 = addVar_16(model, 0, "e13_");
    e23 = addVar_16(model, 0, "e23_");

    unordered_map<string, vector<GRBVar>> e_map;
    e_map["01"] = e01; e_map["02"] = e02; e_map["03"] = e03; 
    e_map["12"] = e12; e_map["13"] = e13; e_map["23"] = e23;

    for (int i = 0; i<16; i++){
        vector<GRBVar> e_Ind = {e_map["01"][i], e_map["02"][i], e_map["03"][i], e_map["12"][i], e_map["13"][i], e_map["23"][i]};       
        add_constraints_from_template(model, e_Ind, eij_15feasible_pattern_tmplt[0], 11, 7);
    }

    
    vector<double> a1(16), a2(16), aminus1(16);
    for (int i = 0; i< 16; i++){
        a1[i] = 1;
        a2[i] = 2;
        aminus1[i] = -1;
    }
    //---------nontrivial initial constraints-----------
    
    for (auto& x: e_map){
        GRBLinExpr lhs = 0;
        lhs.addTerms(&a1[0], &x.second[0], 16);
        model.addConstr(lhs >= 1);
        model.update();
    } 
    
    //-----end of nontrivial initial constraints-----------

    //--------gen plaintextconstraints--------------
    
    //e.g. H__ = {1,1,0,0}, H_x = {0,0,0,1}, H_c = {0,0,1,0};
    //On diagonals indicated by H__, patterns equal 'aaaa' by coding 000000;
    //On diagonals indicated by H_x, patterns equal 'abba' by coding 110011;
    //on diagonals indicated by H_c, patterns equal 'abab' by coding 101101;
    vector<int> H__(4), H_x(4), H_c(4);
    H__ = {1,1,0,0};
    H_c = {0,0,1,0};
    H_x = {0,0,0,1};
    
    for (int i = 0; i < 4; i++){
        unordered_map<int, vector<GRBVar>> diag;
        for (int j = 0; j < 4; j++){
            diag[j] = {e_map["01"][perm[4*i+j]], e_map["02"][perm[4*i+j]], e_map["03"][perm[4*i+j]], e_map["12"][perm[4*i+j]], e_map["13"][perm[4*i+j]], e_map["23"][perm[4*i+j]]};
        }

        if (H__[i] == 1){ // identity: aaaa
            for (int j = 0; j < 4; j++){
                add_constraints_constant(model, &diag[j][0], "000000", 6);
            }          
        }
        else if (H_x[i] == 1){ // exchange: abba
            for (int j = 0; j < 4; j++){
                add_constraints_constant(model, &diag[j][0], "110011", 6);
            }
            
        }else if (H_c[i] == 1){ // copy: abab
            for (int j = 0; j < 4; j++){
                add_constraints_constant(model, &diag[j][0], "101101", 6);
            }
           
        } else { // shift: aacc
            for (int j = 0; j < 4; j++){
                add_constraints_constant(model, &diag[j][0], "011110", 6);
            }
        }
    }

    //--------Generate constraints for each round---------------
    for (int i = 1; i <= r; i++){
        unordered_map<string, vector<GRBVar>> er_map;
        for (auto& x: e_map){
            er_map[x.first] = addVar_16(model, i, "e"+x.first+"_");
        }
        genConstraints_of_Round(model, i, e_map, er_map);
        e_map = er_map;     
    }


    // ----------gen ciphertext constraints---------------
    vector<int> d;
    if (r==3){
        d={0}; //collide on the d-th diagonal, e01=0, e23=0
    }
    else{
        d={0,1,2};
    }
    
    vector<GRBVar> ciph01(16);
    vector<GRBVar> ciph23(16);
    for (int i = 0; i<16; i++){
        ciph01[i] = e_map["01"][perm[i]];
        ciph23[i] = e_map["23"][perm[i]];
    }

    for (int i = 0; i < d.size(); i++){
        add_constraints_constant(model, &ciph01[4*d[i]], "0000", 4);
        add_constraints_constant(model, &ciph23[4*d[i]], "0000", 4);
    }
    

    //--------- set objective function --------------------
    GRBLinExpr ActiveVar = 0;
    for (int i=1; i<=r; i++){
        vector<GRBVar> A_SB(16), finalAH(16), finalAL(16), minus(16);
        for (int j = 0; j < 16; j++){
            A_SB[j] = model.getVarByName("A_SB_"+to_string(i-1)+"_"+to_string(j));
            finalAH[j] = model.getVarByName("finalAH_"+to_string(i-1)+"_"+to_string(j));
            finalAL[j] = model.getVarByName("finalAL_"+to_string(i-1)+"_"+to_string(j));
            minus[j] = model.getVarByName("minus_"+to_string(i-1)+"_"+to_string(j));
        }
        ActiveVar.addTerms(&a1[0], &A_SB[0], 16);
        ActiveVar.addTerms(&a2[0], &finalAH[0], 16);
        ActiveVar.addTerms(&a1[0], &finalAL[0], 16);
        ActiveVar.addTerms(&aminus1[0], &minus[0], 16);
    }
    
    if (objbound != 0){
        model.addConstr(ActiveVar <= objbound);
        model.update();
    }
    
    model.setObjective(ActiveVar, GRB_MINIMIZE);
    
    //---------- end of set objective function -------------
    
    cout<< "NumVar "<<model.get(GRB_IntAttr_NumVars)<<endl;
    cout<< "NumConstr "<<model.get(GRB_IntAttr_NumConstrs)<<endl;
    
    model.write("AES.lp");
    model.optimize();
   
    if (model.get( GRB_IntAttr_Status ) == GRB_OPTIMAL){
        int solCount = model.get(GRB_IntAttr_SolCount);
        cout<< solCount <<" Feasible solutions."<<endl;
        
		if ( solCount <= MAX )
		{           
			for (int ii = 0; ii < solCount; ii++){
                model.set(GRB_IntParam_SolutionNumber, ii );
                cout<< ii << "-th solution: vobj = "  << model.get(GRB_DoubleAttr_ObjVal)<<endl;
                parse_sol(model, r, printMode);
                cout<<endl;

            }        			
		}
        else{ 
            cerr << "solCount value  is too big !" << endl;           
            exit(-1);
        }   
        
        
    }  
     
}

int main(int argc, char ** argv){ //
    if (argc != 4){
        cout<< "Input number of rounds, obj bound and printMode."<<endl;
        return -1;
    } else {
        int r = stoi(argv[1]);
        
        int printMode = stoi(argv[3]);
        
        float objBound;
        if (string(argv[2])=="NoObjBound"){
            
            genModel(r, INFINITY, printMode);
        }else{
            
            objBound = stoi(argv[2]);
            cout<<"Set objBound to "<<objBound<<endl;
            genModel(r, objBound, printMode);
        }     
    } 
    return 0;
}