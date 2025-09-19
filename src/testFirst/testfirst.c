#include "base/main/main.h"

ABC_NAMESPACE_IMPL_START

int TestFirst_FirstFunctionAbc(Abc_Frame_t * pAbc)
{
    Abc_Ntk_t * pNtk;
    int result;

    // Get the network currently loaded in ABC
    pNtk = Abc_FrameReadNtk(pAbc);

    if (pNtk == NULL) {
        Abc_Print(-1, "TestFirst_FirstFunctionAbc: Getting the target network has failed.\n");
        return 0;
    }

    // Call the main worker on the network
    result = TestFirst_FirstFunction(pNtk);

    return result;
}


int TestFirst_FirstFunction(Abc_Ntk_t * pNtk) {
    // check if the network is strashed
    if(!Abc_NtkIsStrash(pNtk)){
        Abc_Print(-1, "TestFirst_FirstFunction: This command is only applicable to strashed networks.\n");
        return 0;
    }

    // print information about the network
    Abc_Print(1, "The network with name %s has:\n", Abc_NtkName(pNtk));
    Abc_Print(1, "\t- %d primary inputs;\n", Abc_NtkPiNum(pNtk));
    Abc_Print(1, "\t- %d primary outputs;\n", Abc_NtkPoNum(pNtk));
    Abc_Print(1, "\t- %d AND gates.\n", Abc_NtkNodeNum(pNtk));

    return 1;
}

ABC_NAMESPACE_IMPL_END
