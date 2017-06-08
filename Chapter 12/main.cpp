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
    for (int i = 1; i <= 6; ++i)
    {
        char buf[256];
        sprintf(buf, "./testcases/test%d.tig", i);

        CodeGenerator cg(buf);

        sprintf(buf, "./bin/output/result%d.s", i);
        cg.ToFile(buf);

        sprintf(buf, "as -g -o ./bin/output/result%d.o ./bin/output/result%d.s --32", i, i);
        if (system(buf) == -1)
            return -1;
        
        sprintf(buf, 
                "ld -g -o ./bin/output/result%d ./bin/output/result%d.o"
                " -dynamic-linker /lib/ld-linux.so.2 -lc -m elf_i386",
                i, i);
        if (system(buf) == -1)
            return -1;                
    }

    return 0;
}