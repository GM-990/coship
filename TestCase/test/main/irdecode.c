//LIUZHENGHUA MODIFY 
#include "cs_input.h"

#include "cs_typedef.h"
#include "cs_ir.h"
#include "cs_assert.h"
#include "cs_event.h"

#include "irdecode.h"
#include "cs_panel.h"

#include "cs_memory.h"

#include "cs_testkit.h"

extern CSIR_Error_t CSIRSetClk(int nClk) ;
 

void UI_IRInitialize( int nPriority )
{
#ifdef st7100
#if  defined(TESTAPP_COCOREF)  ||  (STCLKRV_EXT_CLK_MHZ == 27)
    CSIRSetClk(99000000);
#else
    CSIRSetClk(90000000);
#endif
#endif

    {
        char buf[12];
        if(CSTKGetConfigInfo("PORTING", "IR_PROTOCOL", buf, sizeof(buf)) == CS_TK_CONFIG_SUCCESS)
        {
            int nProtocol = CSTKGetIntFromStr(buf,10);
            if(nProtocol >=0 && nProtocol < CSIR_PROTOCOL_NUM)
            {
                CSIRSelectProtocol(nProtocol);
            }            
        }
        else
        {
            CSIRSelectProtocol(CSIR_PROTOCOL_NEC);
        }
    }
    

	CSIRInitialize( nPriority);
}

