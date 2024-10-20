# SysY-compiler

## About

This is a compiler called SysYVM for SysY, which is a C-like language supporting float-type, array, function (the detail about SysY can be found in the [SysY2022 specification](https://github.com/LordLKY/SysY-compiler/blob/main/Doc/SysY2022%E8%AF%AD%E8%A8%80%E5%AE%9A%E4%B9%89.pdf)).

SysYVM is written in C++ (about 5K lines of code). The frontend is built with the help of flex & bison. A LLVM-style IR, which is in Static Single Assignment (SSA) form, serves as midend. On that base, several optimizations are implemented. The backend is a self-defined virtual machine which is based on stacks (so SysYVM is more like a interpreter than a compiler). A low-level language called LIR is designed and runs on the virtual machine. The framework of SysYVM is shown as follows:

![SysYVM framework](https://github.com/LordLKY/SysY-compiler/blob/main/asset/SysYVM%20framework.png)

Although SysYVM is not a perfect compiler (compared to those won prize in compiler-design competitions), it is a good reference for those "compiler-beginner" who wants to implement a toy compiler.

## Build

### Requirement

- a C++ compiler
- bison (GNU Bison) 2.7
- win_flex.exe 2.6.4

```bash
git clone https://github.com/LordLKY/SysY-compiler.git
cd SysY-compiler
make
```

or you can dowload [SysYVM.exe](https://github.com/LordLKY/SysY-compiler/blob/main/SysYVM.exe) directly.

## Usage

```bash
SysYVM [filename] [options] 
```

```
options:
-O0
    compile without optimization
-O1
    complile with optimization(default)
-ShowIR
    show IR of midend(for debugging)
-ShowLIR
    show LIR of backend(for debugging)
-ShowRuntime
    show runtime information(for debugging)
-Prof
    do simple profiling
```

## Test

The ./test folder contains some test cases. And the details of profiling result on those cases can be found [here](https://github.com/LordLKY/SysY-compiler/blob/main/Doc/profiling_results.xlsx). SysYVM can pass all the cases and achieves obvious optmization.

## Feature

### Frontend

Support all the grammar of [SysY2022 specification](https://github.com/LordLKY/SysY-compiler/blob/main/Doc/SysY2022%E8%AF%AD%E8%A8%80%E5%AE%9A%E4%B9%89.pdf).

### IR

Refering the Value-Use mechanism in LLVM IR, IR of SysYVM constructs explicit def-use/use-def chains to facilitate optimization passes.

### Optimization

Include several optimization passes:

- Control Flow Analysis
- Mem2Reg (convert IR to SSA form)
- Dead Code Elimination (non-aggressive)
- Sparse Conditional Constant Propagation
- Loop Analysis
- Global Value Numbering & Global Code Motion
- Phi Elimination (SSA destruction)
- Register Allocation (based on graph coloring)

Most of the optimization are based on SSA form, which is more effcient than traditional ways based on Data Flow Analysis.

### Backend

The detail of LIR & VM can be found in [this document](https://github.com/LordLKY/SysY-compiler/blob/main/Doc/SysY%20LIR.docx).


There more details about SysYVM in ./Doc.