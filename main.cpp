#include <QApplication>
#include "mainwidget.h"
#include "sortwidget.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    MainWidget wgt;
    SortWidget *psortWidget = new SortWidget(&wgt, 5);

    // Layout setup
    QGridLayout *pgridLayout = new QGridLayout;
    pgridLayout->addWidget(psortWidget);
    wgt.setLayout(pgridLayout);
    wgt.setWindowTitle("qAlgo v.1.0");
    wgt.setMinimumSize(500, 400);
    wgt.show();

    return app.exec();
}
