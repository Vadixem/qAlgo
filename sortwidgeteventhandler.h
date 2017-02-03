#ifndef SORTWIDGETEVENTHANDLER_H
#define SORTWIDGETEVENTHANDLER_H
#include <QObject>

class SortWidgetEventHandler : public QObject
{
public:
    SortWidgetEventHandler(QObject *par = 0) : QObject(par){}
protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // SORTWIDGETEVENTHANDLER_H
