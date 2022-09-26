#include "basic.h"

void add_constraints_from_template(GRBModel& model, vector<GRBVar>& x, const int *pattern, int row, int col){
    
    GRBLinExpr lhs = 0;
    for (int i = 0; i < row; i++){
        lhs = 0;
        for (int j = 0; j < col-1; j++){
            lhs += *(pattern + i * col + j) * x[j];
        }
        model.addConstr(lhs >= -1 * (*(pattern + i * col + col -1)));
        model.update();
    }
    
}
void add_constraints_constant(GRBModel& model, GRBVar * x, string value, int num){
    for (int i = 0; i < num; i++){
        model.addConstr(*(x + i) == (int)(value[i]-'0'));
        model.update();
    }
}

string getVectorVarValue(GRBModel& model, vector<GRBVar> var){
    string s="";
    for (int i= 0; i < var.size(); i++){
        s = s + to_string((int)round(var[i].get(GRB_DoubleAttr_Xn))); //注意这里一定是取Xn属性
        //s = s + bitset<1>(var[i].get(GRB_DoubleAttr_Xn)).to_string();//这个不行，有时会把小数的解解析错
    }
    return s;
}
vector<GRBVar> getVarVectorByName(GRBModel& model, string s, const int num){
    vector<GRBVar> vars(num);
    for (int i = 0; i < num; i++){
        vars[i] = model.getVarByName(s+to_string(i));
    }
    return vars;
}

string value_to_pattern(unordered_map<string, string> e_value){
    vector<string> Ind = {"01", "02", "03", "12", "13", "23"};
    int num = e_value["01"].size();
    string res= "";
    for (int j = 0; j < num; j++){
        string s ="";
        for (int i = 0; i < 6; i++){
            s += e_value[Ind[i]][j];
        }
        if (s=="000000"){
            res += "-";
        }
        else if (s=="101101"){
            res += "c";
        }
        else if (s=="110011"){
            res += "x";
        }
        else if (s=="011110"){
            res += "s";
        }
        else if (s=="111111"){
            res += "*";
        }
        else{
            res += "o";
            //cout<<"singular pattern"<<endl;
        }

    }
    return res;
}

void print_state_pattern(string s){
    if (s.length()==16){
        for (int i = 0; i< 4; i++){
            for (int j=0; j < 4; j++){
                cout << s[4*j+i];
            }
            cout <<endl;
        }
    }else{
        exit(-1);
    }
}
string find_first_col_pattern(GRBModel& model, int r){
    vector<string> Ind = {"01", "02", "03", "12", "13", "23"};
    unordered_map<string, vector<GRBVar>> e_map;
    unordered_map<string, string> e_value;
    for (int i = 0; i < 6; i++){
        e_map[Ind[i]] = getVarVectorByName(model, "e"+Ind[i]+"_0_", 16);
        e_value[Ind[i]] = getVectorVarValue(model, e_map[Ind[i]]);
    }
    string s;
    s = value_to_pattern(e_value); 
    
    return s.substr(0,4);

}
void printPattern_latex(string s){

    for (int i = 0; i < 16; i++){
        if (s[i]=='-'){
            cout << "{" << s[i] << "}";
        }else if(s[i]=='*'){
            cout<< "$\\ast$";
        }
        else{
            cout << s[i];
        }
        if (i%4 == 3){
            cout <<" ";
        }
    }

}
void parse_sol(GRBModel& model, int r, int printMode){
    vector<string> Ind = {"01", "02", "03", "12", "13", "23"};
    unordered_map<string, vector<GRBVar>> e_map;
    unordered_map<string, string> e_value;
    for (int i = 0; i < 6; i++){
        e_map[Ind[i]] = getVarVectorByName(model, "e"+Ind[i]+"_0_", 16);
        e_value[Ind[i]] = getVectorVarValue(model, e_map[Ind[i]]);
    }

    //retrieve activity variables
    vector<GRBVar> A_SB(16), finalAH(16), finalAL(16),minus(16);
    string A_SB_val, finalAH_val, finalAL_val, minus_val;
    for (int j = 0; j < 16; j++){
        A_SB[j] = model.getVarByName("A_SB_"+to_string(1-1)+"_"+to_string(j));
        finalAH[j] = model.getVarByName("finalAH_"+to_string(1-1)+"_"+to_string(j));
        finalAL[j] = model.getVarByName("finalAL_"+to_string(1-1)+"_"+to_string(j));
        minus[j] = model.getVarByName("minus_"+to_string(1-1)+"_"+to_string(j));
    }
    A_SB_val = getVectorVarValue(model, A_SB);
    finalAH_val = getVectorVarValue(model, finalAH);
    finalAL_val = getVectorVarValue(model, finalAL);
    minus_val = getVectorVarValue(model, minus);

    if (printMode == 1){// print a pattern string
        cout<<value_to_pattern(e_value); 
        //printPattern_latex(value_to_pattern(e_value));
    }
    else if (printMode == 2){ // print pattern in state
        cout << "0-th state "<<endl;
        print_state_pattern(value_to_pattern(e_value));
        cout << "A_SB        = " << A_SB_val << endl;
        cout << "finalAH_val = " << finalAH_val << endl;
        cout << "finalAL_val = " << finalAL_val << endl;
        cout << "minus_val   = " << minus_val << endl;
    }else{
        cout <<"Illegal printMode!" << endl;
        exit(-1);
    }
    
    
    for (int j = 1; j <= r; j++){
        for (int i = 0; i < 6; i++){
            e_map[Ind[i]] = getVarVectorByName(model, "e"+Ind[i]+"_"+to_string(j)+"_", 16);
            e_value[Ind[i]] = getVectorVarValue(model, e_map[Ind[i]]);
        }

        for (int k = 0; k < 16; k++){
            A_SB[k] = model.getVarByName("A_SB_"+to_string(j-1)+"_"+to_string(k));
            finalAH[k] = model.getVarByName("finalAH_"+to_string(j-1)+"_"+to_string(k));
            finalAL[k] = model.getVarByName("finalAL_"+to_string(j-1)+"_"+to_string(k));
            minus[k] = model.getVarByName("minus_"+to_string(j-1)+"_"+to_string(k));
        }
        A_SB_val = getVectorVarValue(model, A_SB);
        finalAH_val = getVectorVarValue(model, finalAH);
        finalAL_val = getVectorVarValue(model, finalAL);
        minus_val = getVectorVarValue(model, minus);
        if (printMode == 1){// print a pattern string
            cout<<value_to_pattern(e_value); 
            //cout<<" & ";
            //printPattern_latex(value_to_pattern(e_value));

        }
        else if (printMode == 2){ // print pattern in state
            cout << j << "-th state "<<endl;
            print_state_pattern(value_to_pattern(e_value));
            cout << "A_SB        = " << A_SB_val << endl;
            cout << "finalAH_val = " << finalAH_val << endl;
            cout << "finalAL_val = " << finalAL_val << endl;
            cout << "minus_val   = " << minus_val << endl;
        }
        
    }
    //cout<<"\\\\"<<endl;
    //cout<<"\\hline"<<endl;
    
}



