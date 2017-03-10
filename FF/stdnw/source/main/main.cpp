/** @file
 *  @brief ÄÏÍøÖ÷³ÌĞò
 *  @author 
 *  @date 2011/11/30
 *  @version 0.1
 *  @see appcmd.h
 */
#include "nwappcmd.h"

int main(int argc, char *argv[])
{
	C_NwAppCmd NwAppCmd(argc,argv);
	return NwAppCmd.ExecuteCmd();
}

