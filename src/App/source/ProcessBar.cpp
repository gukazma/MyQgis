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
        const int nThisTick = std::min(100, std::max(0,
        static_cast<int>(dfComplete * 100) ));
        ProcessDialog* processdialog = (ProcessDialog*) pProgressArg;
            
        processdialog->setValue(nThisTick);
        std::cout << nThisTick  << std::endl;
        return true;
    }
}