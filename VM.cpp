#include "VM.h"
using namespace std;

bool Prof = false;
int prof_cost = 0; // for profiling
int mem_cost = 0; // for profiling

void STACK::op_add(){push(pop() + pop());}

void STACK::op_sub(){
    double op2 = pop(), op1 = pop();
    push(op1 - op2);
}

void STACK::op_mul(){push(pop() * pop());}

void STACK::op_div_i(){
    int op2 = pop(), op1 = pop();
    push(op1 / op2);
}

void STACK::op_div_f(){
    double op2 = pop(), op1 = pop();
    push(op1 / op2);
}

void STACK::op_mod(){
    int op2 = pop(), op1 = pop();
    push(op1 % op2);
}

void STACK::op_eq(){push(pop() == pop());}

void STACK::op_ne(){push(pop() != pop());}

void STACK::op_gt(){
    double op2 = pop(), op1 = pop();
    push(op1 > op2);
}
void STACK::op_lt(){
    double op2 = pop(), op1 = pop();
    push(op1 < op2);
}
void STACK::op_ge(){
    double op2 = pop(), op1 = pop();
    push(op1 >= op2);
}

void STACK::op_le(){
    double op2 = pop(), op1 = pop();
    push(op1 <= op2);
}

bool STACK::op_if(){return pop() != 0;}

double REG_FILE::read_reg(int id){
    return reg_space[id];
}

void REG_FILE::write_reg(int id, double data){
    reg_space[id] = data;
}

double DATA_MEM::read_mem(int id){
    if(Prof){prof_cost += 2;}
    return mem_space[id];
}

void DATA_MEM::write_mem(int id, double data){
    if(Prof){prof_cost += 2;}
    mem_space[id] = data;
}

void RUNTIME::Run(){
    LoadInstr(LIRset);

    runtime_stack.push(-1); // flag to terminate
    unordered_map<string, int> *new_map = new unordered_map<string, int>();
    AddrMaps.push_back(new_map);

    while(ip >= 0 && ip < InstrSet.size()){
        if(ShowRuntime){
            cout << "ip: " << ip << "  ";
        }
        string op("");
        string obj("");
        InstrDecode(InstrSet[ip], op, obj);
        if(ShowRuntime){
            cout << "op: " << op << "  obj: " << obj << endl;
        }
        InstrExe(op, obj);
    }

    delete AddrMaps.back();
    AddrMaps.pop_back();

    if(Prof){
        cout << "*** profiling result ***" << endl;
        cout << "time cost: " << prof_cost << endl;
        cout << "mem cost: " << mem_cost << endl;
    }
}

void RUNTIME::LoadInstr(vector<string>& LIRset){
    for(int i = 0; i < LIRset.size(); i++){
        if(i < LIRset.size() - 1 &&
           LIRset[i].substr(0, 3) == "pop" && LIRset[i + 1].substr(0, 4) == "push"){
            string op1 = LIRset[i].substr(4, LIRset[i].length() - 4);
            string op2 = LIRset[i + 1].substr(5, LIRset[i + 1].length() - 5);

            if(op1 == op2){
                InstrSet.push_back("mov " + op1);
                i += 1;
            }
            else{InstrSet.push_back(LIRset[i]);}
        } // simple opt: pop A; push A -> mov A
        else{InstrSet.push_back(LIRset[i]);}

        if(LIRset[i].substr(0, 5) == "label"){
            string bb = LIRset[i].substr(6, LIRset[i].length() - 6);
            // LabelMap[bb] = i;
            LabelMap[bb] = InstrSet.size() - 1;
        } // record addr of BasicBlock
    }
}

void RUNTIME::InstrDecode(string& instr, string& s1, string& s2){
    int i = 0;
    while(i < instr.length() && instr[i] != ' '){i++;}
    if(i == instr.length()){s1 = instr;}
    else{
        s1 = instr.substr(0, i);
        s2 = instr.substr(i + 1, instr.length() - i - 1);
    }
}

void RUNTIME::InstrExe(string& op, string& obj){
    ip += 1;
    if(obj.length() == 0){
        if(op == "ftoi"){
            float data = runtime_stack.pop();
            runtime_stack.push((int)data);
            if(Prof){prof_cost += 2;}
        }
        else if(op == "itof"){
            int data = runtime_stack.pop();
            runtime_stack.push((float)data);
            if(Prof){prof_cost += 2;}
        }
        else if(op == "add"){runtime_stack.op_add(); if(Prof){prof_cost += 1;}}
        else if(op == "sub"){runtime_stack.op_sub(); if(Prof){prof_cost += 1;}}
        else if(op == "mul"){runtime_stack.op_mul(); if(Prof){prof_cost += 5;}}
        else if(op == "div"){runtime_stack.op_div_i(); if(Prof){prof_cost += 10;}}
        else if(op == "div_f"){runtime_stack.op_div_f(); if(Prof){prof_cost += 15;}}
        else if(op == "mod"){runtime_stack.op_mod(); if(Prof){prof_cost += 10;}}
        else if(op == "eq"){runtime_stack.op_eq(); if(Prof){prof_cost += 1;}}
        else if(op == "ne"){runtime_stack.op_ne(); if(Prof){prof_cost += 1;}}
        else if(op == "lt"){runtime_stack.op_lt(); if(Prof){prof_cost += 1;}}
        else if(op == "gt"){runtime_stack.op_gt(); if(Prof){prof_cost += 1;}}
        else if(op == "le"){runtime_stack.op_le(); if(Prof){prof_cost += 1;}}
        else if(op == "ge"){runtime_stack.op_ge(); if(Prof){prof_cost += 1;}}
        else if(op == "if"){
            if(!runtime_stack.op_if()){ip++;}
            if(Prof){prof_cost += 1;}
        }
        else if(op == "push_ip"){runtime_stack.push(ip);}
        else if(op == "pop_ip"){ip = runtime_stack.pop();}
        else if(op == "push_ret"){runtime_stack.push(ret);}
        else if(op == "pop_ret"){ret = runtime_stack.pop();}
        else if(op == "push_all_reg"){
            for(int i = 0; i < reg_file.reg_space.size(); i++){
                runtime_stack.push(reg_file.read_reg(i));
            }
        }
        else if(op == "pop_all_reg"){
            for(int i = reg_file.reg_space.size() - 1; i >= 0; i--){
                reg_file.write_reg(i, runtime_stack.pop());
            }
        }
        else if(op == "new_func"){
            unordered_map<string, int> *new_map = new unordered_map<string, int>();
            (*new_map)["__MEM__"] = data_mem.top;
            AddrMaps.push_back(new_map);
        }
        else if(op == "free_func"){
            unordered_map<string, int> *addr_map = AddrMaps.back();
            data_mem.top = (*addr_map)["__MEM__"];
            addr_map->clear();
            delete addr_map;
            AddrMaps.pop_back();
        }
    }
    else{
        if(op == "alloca"){
            (*AddrMaps.back())[obj] = data_mem.top;
            data_mem.top++;
            if(Prof){mem_cost = max(mem_cost, data_mem.top);}
        }
        else if(op == "with_space"){
            (*AddrMaps.back())[obj] = data_mem.top;
            stride = CalStride(obj, true);
            data_mem.top += stride;
            if(Prof){mem_cost = max(mem_cost, data_mem.top);}
        }
        else if(op == "push"){
            if(obj.length() >= 1 && obj[0] <= '9' && obj[0] >= '0'){
                runtime_stack.push(stod(obj));
            } // push 2
            else if(obj.length() >= 2 && obj[0] == '-'){
                runtime_stack.push(stod(obj));
            } // push -2
            else if(obj.length() >= 2 && obj.substr(0, 2) == "%r"){
                int r_id = stoi(obj.substr(2, obj.length() - 2));
                double data = reg_file.read_reg(r_id);
                runtime_stack.push(data);
            } // push %r2
            else if(obj.length() >= 3 && obj.substr(0, 3) == "@%r"){
                int r_id = stoi(obj.substr(3, obj.length() - 3));
                int addr = reg_file.read_reg(r_id);
                double data = data_mem.read_mem(addr);
                runtime_stack.push(data);
                return;
            } // push @%r2 (push [%r2])
            else if(obj.length() >= 5 && obj.substr(0, 5) == "@@reg"){
                string var_name = obj.substr(1);
                int addr1 = FindAddr(var_name);
                int addr2 = data_mem.read_mem(addr1);
                double data = data_mem.read_mem(addr2);
                runtime_stack.push(data);
                return;
            } // push @@reg2 (used when reg storing addr is spilled)
            else if(obj.length() >= 1 && obj[0] == '@'){
                int addr = FindAddr(obj);
                double data = data_mem.read_mem(addr);
                runtime_stack.push(data);
            } // push @x2
            else if(obj.length() >= 1 && obj[0] == '&'){
                string Array = obj.substr(1, obj.length() - 1);
                int addr = FindAddr(Array);
                runtime_stack.push(addr);
            } // push &@x2 (used when meet array)
        }
        else if(op == "pop" || op == "mov"){
            if(obj.length() >= 2 && obj.substr(0, 2) == "%r"){
                int r_id = stoi(obj.substr(2, obj.length() - 2));
                double data = op == "pop" ? runtime_stack.pop() : runtime_stack.top();
                reg_file.write_reg(r_id, data);
            } // pop %r2
            else if(obj.length() >= 3 && obj.substr(0, 3) == "@%r"){
                int r_id = stoi(obj.substr(3, obj.length() - 3));
                int addr = reg_file.read_reg(r_id);
                double data = op == "pop" ? runtime_stack.pop() : runtime_stack.top();
                data_mem.write_mem(addr, data);
                return;
            } // pop @%r2 (pop [%r2])
            else if(obj.length() >= 5 && obj.substr(0, 5) == "@@reg"){
                string var_name = obj.substr(1);
                int addr1 = FindAddr(var_name);
                int addr2 = data_mem.read_mem(addr1);
                double data = op == "pop" ? runtime_stack.pop() : runtime_stack.top();
                data_mem.write_mem(addr2, data);
                return;
            } // pop @@reg2 (used when reg storing addr is spilled)
            else if(obj.length() >= 1 && obj[0] == '@'){
                int addr = FindAddr(obj);
                double data = op == "pop" ? runtime_stack.pop() : runtime_stack.top();
                data_mem.write_mem(addr, data);
            } // pop @x2
        }
        else if(op == "print_int" || op == "print_float"){
            if(obj.length() >= 1 && obj[0] <= '9' && obj[0] >= '0'){
                cout << (op == "print_int" ? stoi(obj) : stof(obj)) << endl;
            } // print 2
            else if(obj.length() >= 2 && obj[0] == '-'){
                cout << (op == "print_int" ? stoi(obj) : stof(obj)) << endl;
            } // print -2
            else if(obj.length() >= 2 && obj.substr(0, 2) == "%r"){
                int r_id = stoi(obj.substr(2, obj.length() - 2));
                double data = reg_file.read_reg(r_id);
                cout << (op == "print_int" ? (int)data : (float)data) << endl;
            } // print %r2
            else if(obj.length() >= 1 && obj[0] == '@'){
                int addr = FindAddr(obj);
                double data = data_mem.read_mem(addr);
                cout << (op == "print_int" ? (int)data : (float)data) << endl;
            } // print @x2
        }
        else if(op == "with_type"){
            if(ip < InstrSet.size() && InstrSet[ip].substr(0, 2) == "gp"){
                stride = CalStride(obj, true);
            }
            else{stride = CalStride(obj, false);}
        }
        else if(op == "gep" || op == "gp"){
            int addr = runtime_stack.pop();
            int step = 0;
            if(obj.length() >= 1 && obj[0] <= '9' && obj[0] >= '0'){
                step =  stoi(obj);
            } // gep 2
            else if(obj.length() >= 2 && obj.substr(0, 2) == "%r"){
                int r_id = stoi(obj.substr(2, obj.length() - 2));
                double data = reg_file.read_reg(r_id);
                step = (int)data;
            } // gep %r2
            else if(obj.length() >= 1 && obj[0] == '@'){
                int addr = FindAddr(obj);
                double data = data_mem.read_mem(addr);
                step = (int)data;
            } // gep @x2
            runtime_stack.push(addr + step * stride);
            stride = 1;
        }
        // else if(op == "label"){}
        else if(op == "goto" || op == "else"){ip = LabelMap[obj];}
        else if(op == "init"){
            string prev_op("");
            string target("");
            InstrDecode(InstrSet[ip - 2], prev_op, target);
            if(prev_op == "alloca"){
                int addr = (*AddrMaps.back())[target];
                data_mem.write_mem(addr, stod(obj));
            }
        }
        else if(op == "array_init"){
            string prev_op("");
            string target("");
            InstrDecode(InstrSet[ip - 3], prev_op, target);
            if(prev_op == "alloca"){
                int addr = (*AddrMaps.back())[target];
                if(obj == "ZERO"){
                    for(int i = 0; i < stride; i++){
                        data_mem.write_mem(addr + i, 0);
                    }
                }
                else{
                    GlobalArrayInit *GAI = GAIset[stoi(obj) - 1];
                    for(int i = 0; i < GAI->Pos.size(); i++){
                        if(GAI->val_type == CONST_I32 || GAI->val_type == I32){
                            data_mem.write_mem(addr + GAI->Pos[i], GAI->IArray[i]);
                        }
                        else{data_mem.write_mem(addr + GAI->Pos[i], GAI->FArray[i]);}
                    }
                }
            }
        }
    }
}

int RUNTIME::CalStride(string& obj, bool all){
    int len = obj.length();
    if(obj[0] == '[' && obj[len - 1] == ']'){
        vector<int> dims;
        int num_start = 1, stride = 1;
        for(int i = 1; i < len - 1; i++){
            if(obj[i] == ' '){
                dims.push_back(stoi(obj.substr(num_start, i - num_start)));
                num_start = i + 1;
            }
        }
        for(int i = all ? 0 : 1; i < dims.size(); i++){
            stride *= dims[i];
        }
        return stride;
    }
    else{return 1;}
} // string("[a b c ]") -> all ? a*b*c : b*c 

int RUNTIME::FindAddr(string& obj){
    if((*AddrMaps.back()).count(obj)){return (*AddrMaps.back())[obj];}
    else{return (*AddrMaps[0])[obj];}
}