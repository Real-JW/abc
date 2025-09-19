#include "base/main/main.h"
#include "testfirst.h"

ABC_NAMESPACE_IMPL_START

static int TestFirst_CommandTestFirst( Abc_Frame_t * pAbc, int argc, char ** argv );
static int TestFirst_CommandRewriteArea( Abc_Frame_t * pAbc, int argc, char ** argv );

void TestFirst_Init( Abc_Frame_t * pAbc )
{
    Cmd_CommandAdd( pAbc, "Various",   "firstcmd",     TestFirst_CommandTestFirst, 0 );
    Cmd_CommandAdd( pAbc, "Synthesis", "rewrite_area", TestFirst_CommandRewriteArea, 1 );
}

static int TestFirst_CommandTestFirst( Abc_Frame_t * pAbc, int argc, char ** argv )
{
    int fVerbose = 0;
    int c, result;

    Extra_UtilGetoptReset();
    while ( ( c = Extra_UtilGetopt( argc, argv, "vh" ) ) != EOF )
    {
        switch ( c )
        {
        case 'v':
            fVerbose ^= 1;
            break;
        case 'h':
            goto usage;
        default:
            goto usage;
        }
    }

    result = TestFirst_FirstFunctionAbc( pAbc );

    if ( fVerbose )
    {
        Abc_Print( 1, "\nVerbose mode is on.\n" );
        Abc_Print( 1, "The command %s.\n", result ? "finished successfully" : "execution has failed" );
    }
    return 0;

usage:
    Abc_Print( -2, "usage: firstcmd [-vh] \n" );
    Abc_Print( -2, "\t        Our first command in ABC. It prints information about the function read into ABC\n" );
    Abc_Print( -2, "\t-v    : toggle printing verbose information [default = %s]\n", fVerbose ? "yes" : "no" );
    Abc_Print( -2, "\t-h    : print the command usage\n" );
    return 1;
}

static int TestFirst_CommandRewriteArea( Abc_Frame_t * pAbc, int argc, char ** argv )
{
    Abc_Ntk_t * pNtk = Abc_FrameReadNtk( pAbc ), * pDup;
    int c, RetValue;
    int fUpdateLevel = 1;
    int fPrecompute  = 0;
    int fUseZeros    = 0;
    int fVerbose     = 0;
    int fVeryVerbose = 0;
    int fPlaceEnable = 0;
    extern void Rwr_Precompute();

    Extra_UtilGetoptReset();
    while ( ( c = Extra_UtilGetopt( argc, argv, "lxzvwh" ) ) != EOF )
    {
        switch ( c )
        {
        case 'l':
            fUpdateLevel ^= 1;
            break;
        case 'x':
            fPrecompute ^= 1;
            break;
        case 'z':
            fUseZeros ^= 1;
            break;
        case 'v':
            fVerbose ^= 1;
            break;
        case 'w':
            fVeryVerbose ^= 1;
            break;
        case 'p':
            fPlaceEnable ^= 1;
            break;
        case 'h':
            goto usage;
        default:
            goto usage;
        }
    }

    if ( fPrecompute )
    {
        Rwr_Precompute();
        return 0;
    }

    if ( pNtk == NULL )
    {
        Abc_Print( -1, "Empty network.\n" );
        return 1;
    }
    if ( !Abc_NtkIsStrash( pNtk ) )
    {
        Abc_Print( -1, "This command can only be applied to an AIG (run \"strash\").\n" );
        return 1;
    }
    if ( Abc_NtkGetChoiceNum( pNtk ) )
    {
        Abc_Print( -1, "AIG resynthesis cannot be applied to AIGs with choice nodes.\n" );
        return 1;
    }

    pDup = Abc_NtkDup( pNtk );
    RetValue = Abc_NtkRewrite( pNtk, fUpdateLevel, fUseZeros, fVerbose, fVeryVerbose, fPlaceEnable, 1 );
    if ( RetValue == -1 )
    {
        Abc_FrameReplaceCurrentNetwork( pAbc, pDup );
        printf( "An error occurred during computation. The original network is restored.\n" );
    }
    else
    {
        Abc_NtkDelete( pDup );
        if ( RetValue == 0 )
        {
            Abc_Print( 0, "Area-oriented rewriting has failed.\n" );
            return 1;
        }
    }
    return 0;

usage:
    Abc_Print( -2, "usage: rewrite_area [-lxzvwh]\n" );
    Abc_Print( -2, "\t         performs rewriting prioritizing area even if depth grows\n" );
    Abc_Print( -2, "\t-l     : toggle preserving the number of levels [default = %s]\n", fUpdateLevel ? "yes" : "no" );
    Abc_Print( -2, "\t-z     : toggle using zero-cost replacements [default = %s]\n", fUseZeros ? "yes" : "no" );
    Abc_Print( -2, "\t-v     : toggle verbose printout [default = %s]\n", fVerbose ? "yes" : "no" );
    Abc_Print( -2, "\t-w     : toggle printout subgraph statistics [default = %s]\n", fVeryVerbose ? "yes" : "no" );
//    Abc_Print( -2, "\t-p     : toggle placement-aware rewriting [default = %s]\n", fPlaceEnable ? "yes" : "no" );
    Abc_Print( -2, "\t-h     : print the command usage\n" );
    return 1;
}

ABC_NAMESPACE_IMPL_END
