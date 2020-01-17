#include <iostream>
#include <fstream>
#include "pin.H"
using std::cerr;
using std::ofstream;
using std::ios;
using std::string;
using std::endl;

ofstream OutFile;

/* ===================================================================== */
/* Initialize counters to 0.                                             */
/* ===================================================================== */

static UINT64 r8count   = 0;
static UINT64 r9count   = 0;
static UINT64 r10count  = 0;
static UINT64 r11count  = 0;
static UINT64 r12count  = 0;
static UINT64 r13count  = 0;
static UINT64 r14count  = 0;
static UINT64 r15count  = 0;
static UINT64 raxcount  = 0;
static UINT64 rbxcount  = 0;
static UINT64 rcxcount  = 0;
static UINT64 rdxcount  = 0;
static UINT64 rsicount  = 0;
static UINT64 rdicount  = 0;
static UINT64 rspcount  = 0;
static UINT64 rbpcount  = 0;
   
/* ===================================================================== */
/* Control registers types by using if conditions.                       */
/* ===================================================================== */
/* I just controlled registers which wrote a value by RegWContain, since */
/* we need to find destination registers. For example, - add r1, r2, r3  */
/* r1 will be our destination register. Therefore, I counted by using    */
/* RegWContain.                                                          */
/* ===================================================================== */

VOID Instruction(INS ins, VOID *v)
{

    if (INS_RegWContain(ins, REG_RAX)){
        raxcount++;
    } else if (INS_RegWContain(ins, REG_RBX)){
        rbxcount++;
    } else if (INS_RegWContain(ins, REG_RCX)){
        rcxcount++;
    } else if (INS_RegWContain(ins, REG_RDX)){
        rdxcount++;
    } else if (INS_RegWContain(ins, REG_RSI)){
        rsicount++;
    } else if (INS_RegWContain(ins, REG_RDI)){
        rdicount++;
    } else if (INS_RegWContain(ins, REG_RSP)){
        rspcount++;
    } else if (INS_RegWContain(ins, REG_RBP)){
        rbpcount++;
    } else if (INS_RegWContain(ins, REG_R8)){
        r8count++;
    } else if (INS_RegWContain(ins, REG_R9)){
        r9count++;
    } else if (INS_RegWContain(ins, REG_R10)){
        r10count++;
    } else if (INS_RegWContain(ins, REG_R11)){
        r11count++;
    } else if (INS_RegWContain(ins, REG_R12)){
        r12count++;
    } else if (INS_RegWContain(ins, REG_R13)){
        r13count++;
    } else if (INS_RegWContain(ins, REG_R14)){
        r14count++;
    } else if (INS_RegWContain(ins, REG_R15)){
        r15count++;
    } 
}

KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool",
    "o", "inscount.out", "specify output file name");

/* ===================================================================== */
/* Write all counters to outfile.                                        */
/* ===================================================================== */

VOID Fini(INT32 code, VOID *v)
{
    OutFile.setf(ios::showbase);
    OutFile << "Count RAX: " << raxcount << endl;
    OutFile << "Count RBX: " << rbxcount << endl;
    OutFile << "Count RCX: " << rcxcount << endl;
    OutFile << "Count RDX: " << rdxcount << endl;
    OutFile << "Count RSI: " << rsicount << endl;
    OutFile << "Count RDI: " << rdicount << endl;
    OutFile << "Count RSP: " << rspcount << endl;
    OutFile << "Count RBP: " << rbpcount << endl;
    OutFile << "Count R8 : " << r8count << endl;
    OutFile << "Count R9 : " << r9count << endl;
    OutFile << "Count R10 : " << r10count << endl;
    OutFile << "Count R11 : " << r11count << endl;
    OutFile << "Count R12 : " << r12count << endl;
    OutFile << "Count R13 : " << r13count << endl;
    OutFile << "Count R14 : " << r14count << endl;
    OutFile << "Count R15 : " << r15count << endl;
    OutFile.close();
}

/* ===================================================================== */
/* Print Help Message                                                    */
/* ===================================================================== */

INT32 Usage()
{
    cerr << "This tool counts the number of dynamic instructions executed" << endl;
    cerr << endl << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}

/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */
/*   argc, argv are the entire command line: pin -t <toolname> -- ...    */
/* ===================================================================== */

int main(int argc, char * argv[])
{
    // Initialize pin
    if (PIN_Init(argc, argv)) return Usage();

    OutFile.open(KnobOutputFile.Value().c_str());

    // Register Instruction to be called to instrument instructions
    INS_AddInstrumentFunction(Instruction, 0);

    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);
    
    // Start the program, never returns
    PIN_StartProgram();
    
    return 0;
}
