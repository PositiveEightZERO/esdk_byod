#ifndef _TOOLS_SM_H_
#define _TOOLS_SM_H_

#include "tools_common.h"

#define TOOLS_SM_MAX_NUM   0xffffffff
#define TOOLS_SEMA4_FIFO   0x00000001
#define TOOLS_SEMA4_PRIOR  0x00000002   
#define TOOLS_SEMA4_GLOBAL 0x00000010
#define TOOLS_WAIT		((ULONG)1 << 30)
#define TOOLS_NO_WAIT		((ULONG)1 << 31)
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
ULONG Tools_API_Sm_Create( CHAR chSmName[4], ULONG ulSmInitialNum,ULONG ulSmMaxNum,
                    ULONG ulFlags, ULONG *pulRetSmID );

ULONG Tools_API_Sm_Delete(ULONG ulSmID);

ULONG Tools_API_Sm_P( ULONG ulSmID,
               ULONG ulFlags,
               ULONG ulTimeOutInMillSec );

ULONG Tools_API_Sm_V( ULONG ulSmID );

#ifdef __cplusplus
}
#endif /* __cpluscplus */
#endif