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
        void WriteSubProcs(std::ofstream& outFile);
        void WriteProcBody(std::ofstream& outFile, IR_myStatement statements);
        void SaveCalleeRegs(std::ofstream& outFile, RA::RegisterMap& map);
        void UnsaveCalleeRegs(std::ofstream& outFile, RA::RegisterMap& map);
        void WriteCalleeRegs(std::string instruction, std::ofstream& outFile, RA::RegisterMap& map);
        void GenOneProcIns(
            IR_myStatement statements, RA::RegisterMap* map, IS::Instructions* ins);

    private:
        IR_myStatement statements;
    };
}

#endif