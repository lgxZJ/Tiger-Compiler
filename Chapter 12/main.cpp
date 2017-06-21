#include "myCodeGen.h"

#include <cassert>
#include <iostream>
#include <cstring>

using namespace lgxZJ;
using namespace lgxZJ::Canonical;
using namespace lgxZJ::IS;
using namespace lgxZJ::LA;
using namespace lgxZJ::RA;

using namespace std;

void CompileTigerRuntime()
{
    char buf[256];

    sprintf(buf, "gcc -g -o ./tiger_rt.s ./tiger_rt.c -S -m32");
    if (system(buf) == -1)
    {
        printf("compile error : tiger runtime!\n");
        exit(-1);
    }

    sprintf(buf, "as -g -o ./bin/output/tiger_rt.o ./tiger_rt.s --32");
    if (system(buf) == -1)
    {
        printf("compile error : tiger runtime!\n");
        exit(-1);
    }
}

char* ChangeExtensionToAsm(char* sourceFilename)
{
    char* dotLocation = strchr(sourceFilename, '.');
    if (dotLocation == NULL)
    {
        printf("source filename error: should in format `*.tig`!\n");
        exit(-1);
    }

    *(dotLocation + 1) = 's';
    *(dotLocation + 2) = '\0';
    return sourceFilename;
}

void CompileAsm(char* asmFilename)
{
    char buf[256];
    sprintf(buf, "as -g -o %s %s --32", asmFilename, asmFilename);

    //  set obj filename
    char* objFilenameDot = strstr(buf, ".s");
    if (objFilenameDot == NULL)
    {
        printf("asm filename error!\n");
        exit(-1);
    }
    *(objFilenameDot + 1) = 'o';

    if (system(buf) == -1)
    {
        printf("as error!\n");
        exit(-1);
    }
}

char* ChangeExtensionToExec(char* asmFilename)
{
    char* dotLocation = strchr(asmFilename, '.');
    if (dotLocation == NULL)
    {
        printf("asm filename error: should in format `*.s`!\n");
        exit(-1);
    }

    *dotLocation = '\0';
    return asmFilename;
}

void LinkToExecutable(char* execFilename)
{
    char buf[256];
    sprintf(buf, 
            "ld -g -o %s %s.o ./bin/output/tiger_rt.o"
            " -dynamic-linker /lib/ld-linux.so.2 -lc -m elf_i386",
            execFilename, execFilename);

    if (system(buf) == -1)
    {
        printf("link error!\n");
        exit(-1);
    }
}

int main(int argc, char* argv[])
{   
    CompileTigerRuntime();
    
    for (int i = 1; i < argc; ++i)
    {
        char * filename = argv[i];
        CodeGenerator cg(filename);

        char* asmFilename = ChangeExtensionToAsm(filename);
        cg.ToFile(asmFilename);

        CompileAsm(asmFilename);
        char* execFilename = ChangeExtensionToExec(asmFilename);

        LinkToExecutable(execFilename);
    }

    printf("==== Executables generated successfully! ====\n");

    // for (int i = 1; i <= 22; ++i)
    // {
    //     char buf[256];
    //     sprintf(buf, "./testcases/test%d.tig", i);

    //     CodeGenerator cg(buf);

    //     sprintf(buf, "./bin/output/result%d.s", i);
    //     cg.ToFile(buf);

    //     sprintf(buf, "as -g -o ./bin/output/result%d.o ./bin/output/result%d.s --32", i, i);
    //     if (system(buf) == -1)
    //         return -1;
        
    //     sprintf(buf, 
    //             "ld -g -o ./bin/output/result%d ./bin/output/result%d.o" 
    //             " ./bin/output/tiger_rt.o"
    //             " -dynamic-linker /lib/ld-linux.so.2 -lc -m elf_i386",
    //             i, i);
    //     if (system(buf) == -1)
    //         return -1;  

    //     sprintf(buf, "./bin/output/result%d", i);
    //     if (system(buf) == -1)
    //         return -1;                
    // }
    return 0;
}