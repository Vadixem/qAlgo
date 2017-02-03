#pragma once
#include <QWidget>
#include <QStateMachine>
#include <QPushButton>
#include <QListWidget>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "swapmanager.h"
#include "graphicsview.h"
#include "rangesetter.h"

/**
 * @brief The SortWidget class encompasses all other developed and existing classes to create main widget
 */
class SortWidget : public QWidget
{
    Q_OBJECT
public:
    int numberOfSortElements;
    int minSortItemQty;
    int maxSortItemQty;
    QGraphicsScene 	*pscene;
    GraphicsView	*pwindow;
    SwapManager		*pmanager;
    QStateMachine	*pmachine;

    QPushButton *pcmdStartStop;
    QPushButton *pcmdNextStep;
    QPushButton *pcmdPrevStep;
    QSpinBox	*pNoItemSpin;
    QListWidget	*pSortTypeList;
    QPushButton *pcmdRestoreOrder;
    QSlider 	*pspeedSlider;
    QPushButton *pcmdRandomize;
    QPushButton *pcmdFillInRange;
    RangeSetter *prangeSetter;

    SortWidget(QWidget *par = 0, int numberOfSortElements = 7);
    bool addSortItem();
    bool deleteSortItem(int index);
    void setControlsVisible(bool visible);

signals:
    void noOfSortItemsChanged(int);

public slots:
    void setNoSortItems(int);
    void slotFillSortItemsInRange(int a = 100, int b = 10);
    void slotFillSortItemsWithRand(int max = 100);

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
};
