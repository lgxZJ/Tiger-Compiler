#ifndef MY_CODE_GEN_H
#define MY_CODE_GEN_H

#include <string>

namespace lgxZJ
{
    class CodeGenerator
    {
    public:
        explicit CodeGenerator(std::string srcFile);

        void ToFile(std::string dstFile);

    private:
        RA::RegisterMaps maps;
        IS::Instructions ins;
    };
}

#endif