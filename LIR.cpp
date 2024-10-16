#include "LIR.h"
#include <vector>
#include <string>
#define REG_NUM 16
using namespace std;

vector<string> LIRset;
vector<GlobalArrayInit *> GAIset;

void Convertor::Run(){
    ConvertPass();

    // just for test
    if(ShowLIR){LIRPrint();}
}

void Convertor::LIRPrint(){
    for(int i = 0; i < LIRset.size(); i++){
        cout << LIRset[i] << endl;
    }
}

void Convertor::ConvertPass(){
    // cout << "LIR:" << endl;

    for(auto iter = pGT->begin(); iter != pGT->end(); iter++){
        // cout << "  alloca " << iter->first->name << endl;
        LIRset.push_back("alloca " + iter->first->name);
        bool isArray = (iter->first->V_Data.sym_type == Array);
        if(isArray){
            // cout << "  with_space " << ConvertPtrType(instr->to) << endl;
            LIRset.push_back("with_space " + ConvertPtrType(iter->first));
        } // only array needs extra space
        Value *from = (Alloca *)iter->second;
        if(from->V_type == ZeroInit){
            if(!isArray){LIRset.push_back("init 0");}
            else{LIRset.push_back("array_init ZERO");}
        }
        else if(from->V_type == ArrayInit && isArray){
            GAIset.push_back((GlobalArrayInit *)from);
            LIRset.push_back("array_init " + to_string(GAIset.size()));
        }
        else{LIRset.push_back("init " + from->name);}
    }
    for(auto iter = pFT->begin(); iter != pFT->end(); iter++){
        FuncModule *pF = iter->second;
        NameMap = &(pF->RAmap);
        PreAllocaSet.clear();
        // cout << "label " << pF->name << endl;
        LIRset.push_back("label " + pF->name);

        LIRset.push_back("new_func"); // allocate mem space for new func

        for(int i = 0; i < pF->ParamList.size(); i++){
            // cout << "  pop " << ConvertReg(pF->ParamList[i]->name) << endl;
            string param = ConvertReg(pF->ParamList[i]->name);
            if(param[0] == '@'){
                LIRset.push_back("alloca " + param);
                PreAllocaSet.insert(param);
            }
            LIRset.push_back("pop " + param);
        }

        BasicBlock *bb_cur = pF->entry();
        while(bb_cur != pF->bbList.end()){
            // cout << "label " << bb_cur->name << endl;
            LIRset.push_back("label " + bb_cur->name);

            // for(int i = 0; i < pF->ParamList.size(); i++){
            //     // cout << "  pop " << ConvertReg(pF->ParamList[i]->name) << endl;
            //     string param = ConvertReg(pF->ParamList[i]->name);
            //     if(param[0] == '@'){
            //         LIRset.push_back("alloca " + param);
            //         PreAllocaSet.insert(param);
            //     }
            //     LIRset.push_back("pop " + param);
            // }

            Instruction *instr_cur = bb_cur->entry();
            while(!bb_cur->instrList.isend(instr_cur)){
                ConvertInstr(instr_cur);
                instr_cur = (Instruction *)(instr_cur->next);
            }

            bb_cur = (BasicBlock *)(bb_cur->next);
        }
    }
}

string Convertor::ConvertReg(string name){
    if(!NameMap->count(name) && name[0] != '%'){return name;}
    else{
        int id = (*NameMap)[name]; 
        if(id < REG_NUM){return "%r" + to_string(id);}
        else{return "@reg" + to_string(id);}
    }
}

string Convertor::ConvertPtrType(Value *V){
    int other_index = V->V_Data.other_index;
    vector<int>& dim_list = sym.Array_list[V->V_Data.val.i]->dim_list;
    string s("");
    if(!dim_list.empty()){
        for(int i = dim_list.size() - other_index; i < dim_list.size(); i++){
            s += to_string(dim_list[i]) + " ";
        }
    }
    s = "[" + s + "1 ]"; // adding a '1' can be more convenient 
    return s;
}

void Convertor::ConvertInstr(Instruction *instr){
    if(instr->isExp()){
        if(instr->op_type == _ftoi_){ConvertFtoi((Ftoi *)instr);}
        else if(instr->op_type == _itof_){ConvertItof((Itof *)instr);}
        else if(instr->op_type == _zext_){ConvertZext((Zext *)instr);}
        else{ConvertBiOp(instr);}
    }
    else if(instr->isBr()){
        if(instr->op_type == _br_){ConvertBr((Br *)instr);}
        else if(instr->op_type == _jump_){ConvertJump((Jump *)instr);}
    }
    else if(instr->isPhi()){ConvertPhi((Phi *)instr);}
    else if(instr->op_type <= _itof_){
        if(instr->op_type == _alloca_){ConvertAlloca((Alloca *)instr);}
        else if(instr->op_type == _load_){ConvertLoad((Load *)instr);}
        else if(instr->op_type == _store_){ConvertStore((Store *)instr);}
        else if(instr->op_type == _zext_){ConvertZext((Zext *)instr);}
        else if(instr->op_type == _ftoi_){ConvertFtoi((Ftoi *)instr);}
        else if(instr->op_type == _itof_){ConvertItof((Itof *)instr);}
    }
    else if(instr->op_type >= _GEP_){
        if(instr->op_type == _GEP_){ConvertGep((GEP *)instr);}
        else if(instr->op_type == _ret_){ConvertRet((Ret *)instr);}
        else if(instr->op_type == _call_){ConvertCall((Call *)instr);}
        else if(instr->op_type == _mov_){ConvertMove((Move *)instr);}
        else if(instr->op_type == _print_){ConvertPrintCall((PrintCall *)instr);}
    }
}

void Convertor::ConvertAlloca(Alloca *instr){
    // cout << "  alloca " << ConvertReg(instr->to->name) << endl;
    if(PreAllocaSet.count(ConvertReg(instr->to->name))){return;}
    LIRset.push_back("alloca " + ConvertReg(instr->to->name));
    if(instr->isToArray && instr->to->V_Data.val_type <= F32){
        // cout << "  with_space " << ConvertPtrType(instr->to) << endl;
        LIRset.push_back("with_space " + ConvertPtrType(instr->to));
    } // only array needs extra space
}

void Convertor::ConvertLoad(Load *instr){
    // cout << "  push " << ConvertReg(instr->from->name) << endl;
    string from = ConvertReg(instr->from->name);
    if(from[0] == '%'){LIRset.push_back("push @" + from);}
    else if(from.length() >= 4 && from.substr(0, 4) == "@reg"){LIRset.push_back("push @" + from);}
    else{LIRset.push_back("push " + from);}
    // cout << "  pop " << ConvertReg(instr->to->name) << endl;
    LIRset.push_back("pop " + ConvertReg(instr->to->name));
}

void Convertor::ConvertStore(Store *instr){
    // cout << "  push " << ConvertReg(instr->from->name) << endl;
    LIRset.push_back("push " + ConvertReg(instr->from->name));
    // cout << "  pop " << ConvertReg(instr->to->name) << endl;
    string to = ConvertReg(instr->to->name);
    if(to[0] == '%'){LIRset.push_back("pop @" + to);}
    else if(to.length() >= 4 && to.substr(0, 4) == "@reg"){LIRset.push_back("pop @" + to);}
    else{LIRset.push_back("pop " + to);}
}

void Convertor::ConvertZext(Zext *instr){
    // cout << "Zext is not currently supported in LIR" << endl;
    // exit(-1);
    LIRset.push_back("push " + ConvertReg(instr->useeList[0]->V->name));
    // our VM doesn't need to do Zext
    LIRset.push_back("pop " + ConvertReg(instr->name));
}

void Convertor::ConvertFtoi(Ftoi *instr){
    // cout << "  push " << ConvertReg(instr->useeList[0]->V->name) << endl;
    LIRset.push_back("push " + ConvertReg(instr->useeList[0]->V->name));
    // cout << "  ftoi" << endl;
    LIRset.push_back("ftoi");
    // cout << "  pop " << ConvertReg(instr->name) << endl;
    LIRset.push_back("pop " + ConvertReg(instr->name));
}

void Convertor::ConvertItof(Itof *instr){
    // cout << "  push " << ConvertReg(instr->useeList[0]->V->name) << endl;
    LIRset.push_back("push " + ConvertReg(instr->useeList[0]->V->name));
    // cout << "  itof" << endl;
    LIRset.push_back("itof");
    // cout << "  pop " << ConvertReg(instr->name) << endl;
    LIRset.push_back("pop " + ConvertReg(instr->name));
}

void Convertor::ConvertBiOp(Instruction *instr){
    string op = "";
    switch(instr->op_type){
    case _add_: op = "add"; break;
    case _sub_: op = "sub"; break;
    case _mul_: op = "mul"; break;
    case _div_: op = "div"; break;
    case _mod_: op = "mod"; break;
    case _eq_: op = "eq"; break;
    case _ne_: op = "ne"; break;
    case _lt_: op = "lt"; break;
    case _gt_: op = "gt"; break;
    case _le_: op = "le"; break;
    case _ge_: op = "ge"; break;
    default: break;
    }
    if(op == ""){
        cout << "Op is not currently supported in LIR" << endl;
        exit(-1);
    }
    if(instr->V_Data.val_type == F32 || instr->V_Data.val_type == CONST_F32){
        if(op == "mod"){
            cout << "MOD of FLOAT is not currently supported in LIR" << endl;
            exit(-1);
        }
        if(op == "div"){op += "_f";}
    }
    // cout << "  push " << ConvertReg(instr->useeList[0]->V->name) << endl;
    LIRset.push_back("push " + ConvertReg(instr->useeList[0]->V->name));
    // cout << "  push " << ConvertReg(instr->useeList[1]->V->name) << endl;
    LIRset.push_back("push " + ConvertReg(instr->useeList[1]->V->name));
    // cout << "  " + op << endl;
    LIRset.push_back(op);
    // cout << "  pop " << ConvertReg(instr->name) << endl;
    LIRset.push_back("pop " + ConvertReg(instr->name));
}

void Convertor::ConvertGep(GEP *instr){
    // cout << "  push " << ConvertReg(instr->useeList[0]->V->name) << endl;
    string name = ConvertReg(instr->useeList[0]->V->name);
    if(name[0] == '@' && !instr->gp){
        if(name.length() >= 4 && name.substr(0, 4) == "@reg"){LIRset.push_back("push " + name);}
        else{LIRset.push_back("push &" + name);}
    }
    else{LIRset.push_back("push " + name);}
    // cout << "  with_type " << ConvertPtrType(instr->useeList[0]->V) << endl;
    LIRset.push_back("with_type " + ConvertPtrType(instr->useeList[0]->V));
    if(instr->gp){
        // cout << "  gp " << ConvertReg(instr->useeList[1]->V->name) << endl;
        LIRset.push_back("gp " + ConvertReg(instr->useeList[1]->V->name));
    }
    else{
        //cout << "  gep " << ConvertReg(instr->useeList[1]->V->name) << endl;
        LIRset.push_back("gep " + ConvertReg(instr->useeList[1]->V->name));
    }
    // cout << "  pop " << ConvertReg(instr->name) << endl;
    LIRset.push_back("pop " + ConvertReg(instr->name));
}

void Convertor::ConvertCall(Call *instr){
    // cout << "  call " << instr->useeList[0]->V->name << endl; // protect spot
    LIRset.push_back("push_all_reg");
    LIRset.push_back("push_ip");
    LIRset.push_back("push " + to_string(instr->useeList.size() + 2));
    LIRset.push_back("add");
    for(int i = instr->useeList.size() - 1; i > 0; i--){
        // cout << "  push " << ConvertReg(instr->useeList[i]->V->name) << endl;
        LIRset.push_back("push " + ConvertReg(instr->useeList[i]->V->name));
    }
    // cout << "  call " << instr->useeList[0]->V->name << endl; // shift to new func
    LIRset.push_back("goto " + instr->useeList[0]->V->name);

    LIRset.push_back("pop_all_reg");
    if(instr->useeList[0]->V->V_Data.val_type != VOID_TYPE){
        LIRset.push_back("push_ret");
        LIRset.push_back("pop " + ConvertReg(instr->name));
    }
}

void Convertor::ConvertRet(Ret *instr){
    // cout << "  ret " << ConvertReg(instr->useeList[0]->V->name) << endl;
    if(!instr->useeList.empty()){
        LIRset.push_back("push " + ConvertReg(instr->useeList[0]->V->name));
        LIRset.push_back("pop_ret");
    }
    LIRset.push_back("free_func"); // free mem space of the func
    LIRset.push_back("pop_ip");
}

void Convertor::ConvertBr(Br *instr){
    string cond = ConvertReg(instr->useeList[0]->V->name);
    // cout << "  push " << cond << endl;
    LIRset.push_back("push " + cond);
    // cout << "  if" << endl;
    LIRset.push_back("if");
    // cout << "  goto " << instr->useeList[1]->V->name << " else " << instr->useeList[2]->V->name << endl;
    LIRset.push_back("goto " + instr->useeList[1]->V->name);
    LIRset.push_back("else " + instr->useeList[2]->V->name);
}

void Convertor::ConvertJump(Jump *instr){
    // cout << "  goto " << instr->getTarget()->name << endl;
    LIRset.push_back("goto " + instr->getTarget()->name);
}

void Convertor::ConvertPhi(Phi *instr){
    cout << "Phi is not currently supported in LIR" << endl;
    exit(-1);
}

void Convertor::ConvertPrintCall(PrintCall *instr){
    Value *v = instr->useeList[0]->V;
    if(v->V_Data.val_type == F32 || v->V_Data.val_type == CONST_F32){
        LIRset.push_back("print_float " + ConvertReg(v->name));
    }
    else{LIRset.push_back("print_int " + ConvertReg(v->name));}
}

void Convertor::ConvertMove(Move *instr){
    // cout << "  push " << ConvertReg(instr->useeList[0]->V->name) << endl;
    LIRset.push_back("push " + ConvertReg(instr->useeList[0]->V->name));
    // cout << "  pop " << ConvertReg(instr->name) << endl;
    LIRset.push_back("pop " + ConvertReg(instr->name));
}