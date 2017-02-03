#include <QApplication>
#include "sortwidget.h"
#include "mainwidget.h"

int main(int argc, char **argv)
{

    QApplication app(argc, argv);

    MainWidget	wgt;
    SortWidget	*psortWidget = new SortWidget(&wgt, 5);

    // Layout setup
    QGridLayout	*pgridLayout = new QGridLayout;
    pgridLayout->addWidget(psortWidget);
    wgt.setLayout(pgridLayout);
    wgt.setWindowTitle("qAlgo v.0.01");
    wgt.setMinimumSize(500, 400);
    wgt.show();

    return app.exec();
}

