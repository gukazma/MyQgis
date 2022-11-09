#include "ProcessBar.h"
#include <iostream>
#include <ostream>
#include <QtWidgets/QProgressBar>
#include <qprogressbar.h>
#include "ProcessDialog.h"
namespace Stone {

    int GDALTermProgress( double dfComplete,
                                    const char * pszMessage,
                                    void * pProgressArg )
    {
        const int nThisTick = std::min(40, std::max(0,
        static_cast<int>(dfComplete * 40.0) ));
        ProcessDialog* processdialog = (ProcessDialog*) pProgressArg;
        // Have we started a new progress run?
        static int nLastTick = -1;
        if( nThisTick < nLastTick && nLastTick >= 39 )
            nLastTick = -1;

        if( nThisTick <= nLastTick )
            return true;

        while( nThisTick > nLastTick )
        {
            ++nLastTick;
            processdialog->setValue(nLastTick);
        }
        
        std::cout << nLastTick << std::endl;
        return true;
    }
}