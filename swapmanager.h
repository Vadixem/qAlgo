#ifndef SWAPMANAGER_H
#define SWAPMANAGER_H

#include <QTimer>
#include <QDebug>
#include <QObject>
#include "qgraphicsrectwidget.h"
#include "graphicsview.h"


enum AnimationType {Swap, Comparison};
enum Actions {Back, Forward};
enum ActionMode{Animation, NoAnimation};

/**
 * @brief The SwapManager class Manages the inner state of animation sequence
 */
class SwapManager : public QObject
{
    Q_OBJECT
public:
    SwapManager(GraphicsView &window, QMap<QString, void (*)(GraphicsView&, SwapManager&)> *sortFunctions = Q_NULLPTR ,unsigned int actionInterval = 1500,QObject *par = 0);
    GraphicsView &window;
    bool 									addPair(int i, int j, AnimationType atype = AnimationType::Swap); /* Adds a pair of elements action to be performed on */
    int  									getTimerInterval() const;
    QPair<QPair<int,int>, AnimationType> 	getCurrentPair();
    int 									getCurrentPairIndex() const;
    void 									setCurrentPairIndex(int index);
    Actions 								getLastAction() const;
    void 									setLastAction(Actions action);
    bool									isAnimationActive();

signals:
    void indexReachedEnd();				/* Emitted when currentPair index is actionPair.size() */
    void indexReachedBegin();			/* Emitted when currentPair index is -1 */
    void actionTimerStarted	();
    void actionTimerStoppedInMiddle();	/* Emitted when during automatical animation stop button is pressed */
    void droppedToDefault();			/* Emitted when action pairs are cleared */

public slots:

    void startNextPairActionTimer();
    void stopNextPairActionTimer(bool b = true);
    void slotStartAction(ActionMode actMode = ActionMode::Animation);
    void slotCheckIndex();
    void slotStepForward(ActionMode actmode = ActionMode::Animation);
    void slotStepBack(ActionMode actmode = ActionMode::Animation);
    void slotDropToDefault();
    void setTimerInterval(int msec);
    void slotFillActionPairs();
    void slotRestoreInitialOrder(ActionMode actMode = ActionMode::NoAnimation);
    void slotPutItemsInStr8Line();
    void slotHandleToggleStartStop(bool checked);
    void slotHandleSpeedSlider(int val);
    void slotSetCurrentSortType(QString s);

private:
    Actions 												lastAction;
    int 													currentPairIndex;
    unsigned int 											actionInterval;
    QTimer*													nextPairActionTimer;
    QVector<QPair<QPair<int,int>, AnimationType>> 			actionPairs;
    QMap<QString, void (*)(GraphicsView&, SwapManager&)>*	sortFunctions;
    QString													currentSortType;
};

#endif // SWAPMANAGER_H

