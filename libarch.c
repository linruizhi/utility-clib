#include "libarch.h"


/* 对所有模块进行初始化，包括utility模组和以太网模块、ewlink*/
void utility_lib_Init( void )
{
    // DSAFE_PRINT( "Initialling UTILITYLib...\r\n" );
    
#if UTILITY_LIB_PARAMETER_MANAGEMENT
    Para_Init();
#endif // UTILITY_LIB_PARAMETER_MANAGEMENT

#if UTILITY_LIB_PROTOCOL_STACK
    Pstk_Init();
#endif //UTILITY_LIB_PROTOCOL_STACK
}


