#ifndef MY_CODE_GEN_H
#define MY_CODE_GEN_H

#include "myRegisterAllocation.h"
#include "myMunch.h"

#include <string>

namespace lgxZJ
{
    class CodeGenerator
    {
    public:
        explicit CodeGenerator(std::string srcFile);

        void ToFile(std::string dstFile);

    private:
        void WriteMainProc(std::ofstream& outFile, IR_myStatement statements);
        void WriteMainExit(std::ofstream& outFile);
        void WriteSubProcs(std::ofstream& outFile);
        void WriteStringData(std::ofstream& outFile);
        void WriteProcBody(std::ofstream& outFile, IR_myStatement statements);
        void WritePrologue(std::ofstream& outFile, RA::RegisterMap& map, int localCount);
        void WriteEpilogue(std::ofstream& outFile, RA::RegisterMap& map);
        void WriteCalleeRegs(std::ofstream& outFile, std::string instruction, bool reverse, RA::RegisterMap& map);
        void GenOneProcIns(
            IR_myStatement statements, RA::RegisterMap* map, IS::Instructions* ins, int* localCount);

    private:
        IR_myStatement statements;
    };
}

#endif