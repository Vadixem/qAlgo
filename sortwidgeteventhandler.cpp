#include "sortwidgeteventhandler.h"
#include "sortwidget.h"

bool SortWidgetEventHandler::eventFilter(QObject *pobj, QEvent *pe)
{
    if (pe->type() == QEvent::Enter)
    {
        static_cast<SortWidget*>(pobj)->setControlsVisible(true);
        return true;
    }
    else if (pe->type() == QEvent::Leave)
    {
        static_cast<SortWidget*>(pobj)->setControlsVisible(false);
        return true;
    }

    return false;
}
