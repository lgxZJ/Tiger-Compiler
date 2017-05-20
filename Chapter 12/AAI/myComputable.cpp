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
            return ToCommonString("add");
        }

        string Add::ToCode(RegisterMap& map) const
        {
            return ToCommonCode("add", map);
        }

        //////////////////////////////////////////////////////////////////
        //                          Sub class
        //////////////////////////////////////////////////////////////////

        DEFINE_TWOCOMPUTE_CTOR(Sub);

        string Sub::ToString() const
        {
            return ToCommonString("sub");
        }

        string Sub::ToCode(RegisterMap& map) const
        {
            return ToCommonCode("sub", map);
        }

        //////////////////////////////////////////////////////////////////
        //                          IMul class
        //////////////////////////////////////////////////////////////////

        DEFINE_ONECOMPUTE_CTOR(IMul);

        string IMul::ToString() const
        {
            return ToCommonString("imul");
        }

        string IMul::ToCode(RegisterMap& map) const
        {
            return ToCommonCode("imul", map);
        }

        ////////////////////////////////////////////////////////////////////
        //                          IDiv class
        ////////////////////////////////////////////////////////////////////

        DEFINE_ONECOMPUTE_CTOR(IDiv);

        string IDiv::ToString() const
        {
            return ToCommonString("idiv");
        }

        string IDiv::ToCode(RegisterMap& map) const
        {
            return ToCommonCode("idiv", map);
        }

        ////////////////////////////////////////////////////////////////////
        //                          Xor class
        ////////////////////////////////////////////////////////////////////

        DEFINE_TWOCOMPUTE_CTOR(Xor);

        string Xor::ToString() const
        {
            return ToCommonString("xor");
        }

        string Xor::ToCode(RegisterMap& map) const
        {
            return ToCommonCode("xor", map);
        }
    }
}