#include "myCodeGen.h"
#include "CXX_frontTester.h"
#include "myCanonical.h"
#include "myLiveness.h"
#include "myRegisterAllocation.h"

#include <ofstream>

using namespace lgxZJ::RA;

using namespace std;

namespace lgxZJ
{
    CodeGenerator::CodeGenerator(string srcFile)
    {
        //  front end work
        IR_myStatement statements = frontTester(srcFile.c_str());

        //  canonical phase
        Statements states = Canonical::Linearize(statements);
        Blocks blocks = Canonical::ToBlocks(states);
        Blocks blocksTraced = Canonical::Trace(blocks);
        Statements statesFlattened = Canonical::Flatten(blocksTraced);

        //  munch phase
        Munch::All(statesFlattened);
        Instructions ins = Munch::GetIns();

        //  liveness analysis and register allocation phases
        RegisterAllocation ra(ins, Trans_outermostLevel(), Frame_registerNumber);

        this->maps = ra.GetRegisterMaps();
        this->ins = ra.GetIns();
    }

    void CodeGenerator::ToFile(string dstFile)
    {
        ofstream outFile(dstFile.c_str(), ios_base::out | ios_base::trunc);

        if (outFile.is_open())
        {
            //  todo: write to file

            outFile.close();
        }
        else
        {
            cerr << "open file: " << dtsFile << " error!";
        }
    }
}