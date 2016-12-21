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

        //////////////////////////////////////////////////////////////////
        //                          Sub class
        //////////////////////////////////////////////////////////////////

        DEFINE_TWOCOMPUTE_CTOR(Sub);

        string Sub::ToString() const
        {
            return  ToCommonString("sub");
        }

        //////////////////////////////////////////////////////////////////
        //                          IMul class
        //////////////////////////////////////////////////////////////////

        DEFINE_ONECOMPUTE_CTOR(IMul);

        string IMul::ToString() const
        {
            return ToCommonString("imul");
        }

        ////////////////////////////////////////////////////////////////////
        //                          IDiv class
        ////////////////////////////////////////////////////////////////////

        DEFINE_ONECOMPUTE_CTOR(IDiv);

        string IDiv::ToString() const
        {
            return ToCommonString("idiv");
        }

        ////////////////////////////////////////////////////////////////////
        //                          Xor class
        ////////////////////////////////////////////////////////////////////

        DEFINE_TWOCOMPUTE_CTOR(Xor);

        string Xor::ToString() const
        {
            return ToCommonString("xor");
        }
    }
}