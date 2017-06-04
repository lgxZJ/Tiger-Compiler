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
    if (argc != 2)  return -1;
    
    CodeGenerator cg(argv[1]);
    cg.ToFile("./bin/result.s");

    return 0;
}