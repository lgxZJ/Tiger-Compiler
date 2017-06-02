#include "myCodeGen.h"
#include "CXX_frontTester.h"

#include <fstream>
#include <iostream>
#include <cassert>

using namespace lgxZJ::RA;
using namespace lgxZJ::IS;
using namespace lgxZJ::Canonical;

using namespace std;

namespace lgxZJ
{
    CodeGenerator::CodeGenerator(string srcFile)
    {
        //  front end work
        this->statements = frontTester(srcFile.c_str());
    }

    void CodeGenerator::ToFile(string dstFileName)
    {
        ofstream outFile(dstFileName.c_str(), ios_base::out | ios_base::trunc);

        if (outFile.is_open())
        {
            //  calling sequence is important because text segment begins
            //  in WriteMainProc() function
            WriteMainProc(outFile, statements);
            WriteSubProcs(outFile);
            outFile.close();
        }
        else
        {
            cerr << "open file: " << dstFileName << " error!";
        }
    }

    void CodeGenerator::WriteMainProc(ofstream& outFile, IR_myStatement statements)
    {
        outFile << ".section .text\n"
                << ".globl _start\n"
                << "_start:\n";
        WriteProcBody(outFile, statements);
        WriteMainExit(outFile);
    }

    void CodeGenerator::WriteProcBody(ofstream& outFile, IR_myStatement statements)
    {
        Instructions ins;
        RegisterMap map;
        GenOneProcIns(statements, &map, &ins);

        WritePrologue(outFile, map);
        for (auto oneIns : ins)
            outFile << oneIns->ToCode(map) << endl;
        WriteEpilogue(outFile, map);

        outFile << endl;
    }
 
    void CodeGenerator::WritePrologue(ofstream& outFile, RegisterMap& map)
    {
        outFile << "\tpushl %ebp\n"
                << "\tmovl %esp, %ebp\n";
        WriteCalleeRegs(outFile, "pushl", false, map);
    }

    void CodeGenerator::WriteEpilogue(ofstream& outFile, RegisterMap& map)
    {
        //  write an epilogue label definition
        outFile << Temp_getLabelString(Canonical::Canonical::GetEpilogueLabel()) << ":"
                << "\t;the epilogue label\n";

        //  the `popl` instructions inside epilogue block should in reverse order
        //  compared to prologue block
        WriteCalleeRegs(outFile, "popl", true, map);

        outFile << "\tmovl %ebp, %esp\n"
                << "\tpopl %ebp\n";
    }

    void CodeGenerator::WriteCalleeRegs(ofstream& outFile, std::string instruction, bool reverse, RA::RegisterMap& map)
    {
        string content;

        myTempList regs = Frame_calleeSaveRegs();
        while (regs)
        {
            //  stack-pointer is also callee-save, but we do not use push/pop
            //  to save it
            if (regs->head != Frame_ESP()) {
                string str;
                str = "\t" + instruction + " ";
                str += TwoOperandOperate::OneRegToCode(regs->head, map) + "\n";

                if (reverse)
                    content = str + content;
                else
                    content = content + str;
            }
            regs = regs->tail;
        }

        outFile << content;
    }

    void CodeGenerator::WriteMainExit(ofstream& outFile)
    {
        outFile << "\tmovl $0, %ebx\n"
                << "\tmovl $1, %eax\n"
                << "\tint $0x80\n\n";
    }

    void CodeGenerator::WriteSubProcs(ofstream& outFile)
    {
        Frame_myFragList procFrags = Trans_getProcFrags();
        while (procFrags)
        {
            Frame_myFrag frag = procFrags->head;
            assert (frag->kind == Frame_myFrag_::Frame_ProcFrag);

            myString procName = MySymbol_GetName(frag->u.procFrag.funcName);
            outFile << ".type " << procName << ", @function\n";
            outFile << procName << ":\n";
            
            WriteProcBody(outFile, frag->u.procFrag.body);
            outFile << "\tret";

            procFrags = procFrags->tail;
        }
    }

    void CodeGenerator::GenOneProcIns(
        IR_myStatement statements, RegisterMap* map, Instructions* ins)
    {
        //  canonical phase
        Statements states = Canonical::Canonical::Linearize(statements);
        Blocks blocks = Canonical::Canonical::ToBlocks(states);
        Blocks blocksTraced = Canonical::Canonical::Trace(blocks);
        Statements statesFlattened = Canonical::Canonical::Flatten(blocksTraced);

        //  munch phase
        Munch::Reset();
        Munch::All(statesFlattened);

        //  liveness analysis and register allocation phases
        RegisterAllocation ra(
            Munch::GetIns(), Trans_outermostLevel(), Frame_registerNumber);

        *map = ra.GetRegisterMap();
        *ins = ra.GetIns();
    }
}