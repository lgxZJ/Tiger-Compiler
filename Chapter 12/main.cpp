#include "myCodeGen.h"

#include <cassert>
#include <iostream>

using namespace lgxZJ;
using namespace lgxZJ::Canonical;
using namespace lgxZJ::IS;
using namespace lgxZJ::LA;
using namespace lgxZJ::RA;

using namespace std;

int main(int argc, char* argv[])
{   
    for (int i = 1; i <= 2; ++i)
    {
        char filename[256];
        sprintf(filename, "./testcases/test%d.tig", i);

        CodeGenerator cg(filename);

        sprintf(filename, "./bin/output/result%d.s", i);
        cg.ToFile(filename);
    }

    return 0;
}