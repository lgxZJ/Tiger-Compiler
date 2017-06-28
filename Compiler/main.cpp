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

void CompileTigerRuntime(const char* buildDir)
{
    char buf[256];

    sprintf(buf, "gcc -g -o %s/tiger_rt.s ./tiger_rt.c -S -m32", buildDir);
    if (system(buf) == -1)
    {
        printf("compile error : tiger runtime!\n");
        exit(-1);
    }

    sprintf(buf, "as -g -o %s/tiger_rt.o %s/tiger_rt.s --32", buildDir, buildDir);
    if (system(buf) == -1)
    {
        printf("compile error : tiger runtime!\n");
        exit(-1);
    }
}

char* ChangeExtensionToAsm(char* sourceFilename)
{
    char* dotLocation = strstr(sourceFilename, ".tig");
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
    char* dotLocation = strstr(asmFilename, ".s");
    if (dotLocation == NULL)
    {
        printf("asm filename error: should in format `*.s`!\n");
        exit(-1);
    }

    *dotLocation = '\0';
    return asmFilename;
}

void LinkToExecutable(char* execFilename, const char* runtimeDir)
{
    char buf[256];
    sprintf(buf, 
            "ld -g -o %s %s.o %s/tiger_rt.o"
            " -dynamic-linker /lib/ld-linux.so.2 -lc -m elf_i386",
            execFilename, execFilename, runtimeDir);

    if (system(buf) == -1)
    {
        printf("link error!\n");
        exit(-1);
    }
}

int main(int argc, char* argv[])
{   
    CompileTigerRuntime("./builds");
    
    for (int i = 1; i < argc; ++i)
    {
        char * filename = argv[i];
        CodeGenerator cg(filename);

        char* asmFilename = ChangeExtensionToAsm(filename);
        cg.ToFile(asmFilename);

        CompileAsm(asmFilename);
        char* execFilename = ChangeExtensionToExec(asmFilename);

        LinkToExecutable(execFilename, "./builds");
    }

    printf("==== %d Executables generated successfully! ====\n", --argc);
    return 0;
}