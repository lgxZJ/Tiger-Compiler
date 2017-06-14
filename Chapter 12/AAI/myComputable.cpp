#include "myComputable.h"
#include "../CXX_myFrame.h"

using namespace std;

namespace lgxZJ
{
    namespace IS
    {
        //////////////////////////////////////////////////////////////
        //                      Add class
        //////////////////////////////////////////////////////////////

        DEFINE_TWOCOMPUTE_CTOR(Add);

        string Add::ToString() const
        {
            return ToCommonString("\taddl");
        }

        string Add::ToCode(RegisterMap& map) const
        {
            return ToCommonCode("\taddl", map);
        }

        //////////////////////////////////////////////////////////////////
        //                          Sub class
        //////////////////////////////////////////////////////////////////

        DEFINE_TWOCOMPUTE_CTOR(Sub);

        string Sub::ToString() const
        {
            return ToCommonString("\tsubl");
        }

        string Sub::ToCode(RegisterMap& map) const
        {
            return ToCommonCode("\tsubl", map);
        }

        //////////////////////////////////////////////////////////////////
        //                          IMul class
        //////////////////////////////////////////////////////////////////

        IMul::IMul(myTemp oneSrcReg)
        {
            srcRep.kind = BinaryUnion::Kind::Reg;
            srcRep.u.reg = oneSrcReg;
        } 

        string IMul::ToString() const
        {
            return ToCommonString("\timull");
        }

        string IMul::ToCode(RegisterMap& map) const
        {
            return ToCommonCode("\timull", map);
        }

        ////////////////////////////////////////////////////////////////////
        //                          IDiv class
        ////////////////////////////////////////////////////////////////////

        IDiv::IDiv(myTemp oneSrcReg)
        {
            srcRep.kind = BinaryUnion::Kind::Reg;
            srcRep.u.reg = oneSrcReg;
        } 

        string IDiv::ToString() const
        {
            return ToCommonString("\tidivl");
        }

        string IDiv::ToCode(RegisterMap& map) const
        {
            return  string("\tmovl $0, %edx\n") + ToCommonCode("\tidivl", map);
        }

        ////////////////////////////////////////////////////////////////////
        //                          Xor class
        ////////////////////////////////////////////////////////////////////

        DEFINE_TWOCOMPUTE_CTOR(Xor);

        string Xor::ToString() const
        {
            return ToCommonString("\txorl");
        }

        string Xor::ToCode(RegisterMap& map) const
        {
            return ToCommonCode("\txorl", map);
        }
    }
}