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

static UINT64 icount = 0;
static UINT64 branchcount = 0;
static UINT64 memorycount = 0;

/* ===================================================================== */
/* Create counter functions.                                             */
/* ===================================================================== */

VOID dobranchcount() { branchcount++; }
VOID domemorycount() { memorycount++; }
VOID docount() { icount++; }

/* ===================================================================== */
/* Control register types by using if conditions. For branch instruction */
/* there was a INS_IsBranch function; for memory instructions, there are */
/* two types of fundtions IsMemoryRead and IsMemoryWrite; and others are */
/* arithmetic and logic instructions.                                    */
/* ===================================================================== */

VOID Instruction(INS ins, VOID *v)
{
    if(INS_IsBranch(ins)){
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)dobranchcount, IARG_END);
    } else if (INS_IsMemoryRead(ins) || INS_IsMemoryWrite(ins)){
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)domemorycount, IARG_END);  
    } else {
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)docount, IARG_END);
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
    OutFile << "Count Branch: " << branchcount << endl;
    OutFile << "Count Memory: " << memorycount << endl;
    OutFile << "Count Others: " << icount << endl;
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
