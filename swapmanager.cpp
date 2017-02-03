#include "swapmanager.h"
#include "swapanimation.h"

SwapManager::SwapManager(GraphicsView &window, QMap<QString, void (*)(GraphicsView&, SwapManager&)> *sortFunctions ,unsigned int actionInterval,QObject *par) :
    QObject(par), actionInterval(actionInterval) ,window(window), sortFunctions(sortFunctions), currentSortType(QString("Insertion Sort"))
{
    setLastAction(Actions::Back);
    currentPairIndex = -1;
    nextPairActionTimer = new QTimer(this);
    nextPairActionTimer->setTimerType(Qt::PreciseTimer);
    nextPairActionTimer->setInterval(actionInterval);
    nextPairActionTimer->setSingleShot(false);

    connect(nextPairActionTimer, SIGNAL(timeout()), this, SLOT(slotStepForward()));

    // When index reaches the end of action then stop the timer
    connect(this, SIGNAL(indexReachedEnd()), this, SLOT(stopNextPairActionTimer()));
}

bool SwapManager::addPair(int i, int j, AnimationType atype)
{
    if (i != j)
        actionPairs.append(QPair<QPair<int,int>, AnimationType>(QPair<int,int>(i, j), atype));
    return true;
}

int  SwapManager::getTimerInterval() const
{
    return nextPairActionTimer->interval();
}

QPair<QPair<int,int>, AnimationType> SwapManager::getCurrentPair()
{
    if (currentPairIndex < 0)
        return actionPairs[0];
    else if (currentPairIndex > actionPairs.size() - 1)
        return actionPairs[actionPairs.size() - 1];

    return actionPairs[currentPairIndex];
}

int SwapManager::getCurrentPairIndex() const
{
    return currentPairIndex;
}

void SwapManager::setCurrentPairIndex(int index)
{
    currentPairIndex = index;
}

Actions SwapManager::getLastAction() const
{
    return lastAction;
}

void SwapManager::setLastAction(Actions action)
{
    this->lastAction = action;
}

bool SwapManager::isAnimationActive()
{
    return nextPairActionTimer->isActive();
}

void SwapManager::startNextPairActionTimer()
{
    if (actionPairs.isEmpty() || currentPairIndex == actionPairs.size())
        return;
    nextPairActionTimer->start();
    emit actionTimerStarted();
    nextPairActionTimer->setInterval(actionInterval + 30);
}

void SwapManager::stopNextPairActionTimer(bool b)
{
    if (b)
    {
        if (currentPairIndex != actionPairs.size())
            emit actionTimerStoppedInMiddle();

        nextPairActionTimer->stop();

    }
}

/**
 * @brief SwapManager::slotStartAction performs action depending on action mode
 * @param actMode can be animation or no animation
 */
void SwapManager::slotStartAction(ActionMode actMode)
{
    slotPutItemsInStr8Line();
    int i = getCurrentPair().first.first;
    int j = getCurrentPair().first.second;
    auto actionType = getCurrentPair().second;

    auto w1 = static_cast<QGraphicsRectWidget*>(window.items().at(i));
    auto w2 = static_cast<QGraphicsRectWidget*>(window.items().at(j));

    switch (actionType)
    {
    case AnimationType::Comparison:
    {
        switch(actMode)
        {
        case ActionMode::Animation:
        {
            QParallelAnimationGroup *pparAnim = new QParallelAnimationGroup(this);
            QPropertyAnimation *panim = new QPropertyAnimation(w1, "pos");
            panim->setDuration(actionInterval);
            panim->setKeyValueAt(0.5, QPoint(w1->pos().x(), w1->pos().y() - w1->geometry().y() * 0.1));
            panim->setEndValue(w1->pos());

            pparAnim->addAnimation(panim);

            panim = new QPropertyAnimation(w2, "pos");
            panim->setDuration(actionInterval);
            panim->setKeyValueAt(0.5, QPoint(w2->pos().x(), w2->pos().y() - w2->geometry().y() * 0.1));
            panim->setEndValue(w2->pos());

            pparAnim->addAnimation(panim);
            pparAnim->start();

            break;
        }

        default: // If mode is no animation then do nothing
            break;

        }
        break;
    }
    case AnimationType::Swap:
    {
        /* SWAP Z VALUES OF THOSE ITEMS in order to prevent calling wrong item via window.items().at()*/
        auto zVal = w1->zValue();
        w1->setZValue(w2->zValue());
        w2->setZValue(zVal);
        switch (actMode)
        {
        case ActionMode::Animation:
        {
            double swapHeightRatio;
            if (abs(w1->zValue() - w2->zValue()) == 1)
                swapHeightRatio = 0.75;
            else swapHeightRatio = 1.2;

            auto swapAnim = getSwapAnimation(w1, w2, actionInterval, swapHeightRatio);

            swapAnim->setParent(this);
            swapAnim->start();
            break;
        }
        default:
            auto pos = w1->pos();
            w1->setPos(w2->pos());
            w2->setPos(pos);
            break;
        }

        break;
    }
    }
}


/**
 * @brief slotCheckIndex Checks position of index. If it reached end or beginning of history emits corresponding signal
 */
void SwapManager::slotCheckIndex()
{
    if (currentPairIndex == -1)
    {
        emit indexReachedBegin(); // Disable back button etc.
        setLastAction(Actions::Back);
    }
    else if (currentPairIndex == actionPairs.size())
    {
        emit indexReachedEnd();		// Disable forward button etc.
        setLastAction(Actions::Forward);
    }
}

/**
 * @brief slotStepForward Allows to perform single swap(or other animation) forward
 * @brief Must be connected to its's stackPopTimer timeout() signal
 */
void SwapManager::slotStepForward(ActionMode actmode)
{
    slotCheckIndex();

    // If swap consequence is empty, then generate new
    if (actionPairs.isEmpty())
    {
        slotFillActionPairs();
    }

    if (getLastAction() == Actions::Back)
    {
        ++currentPairIndex;
        slotStartAction(actmode);
        ++currentPairIndex;
        setLastAction(Actions::Forward);
    }
    else // LastAction == Forward
    {
        slotStartAction(actmode);
        ++currentPairIndex;
        setLastAction(Actions::Forward);
    }


    slotCheckIndex();
}

/**
 * @brief slotStepBack Allows to perform single animation back
 */
void SwapManager::slotStepBack(ActionMode actmode)
{
    slotCheckIndex();

    if (getLastAction() == Actions::Forward)
    {
        --currentPairIndex;
        slotStartAction(actmode);
        --currentPairIndex;
        setLastAction(Actions::Back);
    }
    else // LastAction == Back
    {
        slotStartAction(actmode);
        --currentPairIndex;
        setLastAction(Actions::Back);
    }

    slotCheckIndex();
}

/**
 * @brief slotDropToDefault called to remove all actionPairs, drop current Pair index to default(-1), set last action to back
 */
void SwapManager::slotDropToDefault()
{
    // Restore initial order
    if (!actionPairs.isEmpty())
    {
        setCurrentPairIndex(-1);
        actionPairs.clear();
        setLastAction(Actions::Back);

        emit droppedToDefault();

        slotPutItemsInStr8Line();
    }
}

/**
 * @brief SetTimerInterval  Changes speed of animation
 * @param msec time in ms
 */
void SwapManager::setTimerInterval(int msec)
{
    actionInterval = abs(msec);
    nextPairActionTimer->setInterval(actionInterval + 30);
}

void SwapManager::slotHandleSpeedSlider(int val)
{
    qreal min, max, maxVal;
    min = 100.0;
    max = 3000.0;
    maxVal = 7.0;

    setTimerInterval(max - (max * qreal(val)/maxVal) + min);
}

void SwapManager::slotFillActionPairs()
{
    slotRestoreInitialOrder();

    void (*func)(GraphicsView&, SwapManager&) = sortFunctions->find(currentSortType).value();
    // Reset all data
    slotDropToDefault();

    // Generate animation sequence
    (*func)(window, *this);
}

void SwapManager::slotRestoreInitialOrder(ActionMode actMode)
{
    if (!currentPairIndex || actionPairs.isEmpty()) return;
    else if (!actionPairs.isEmpty())
    {
        while (currentPairIndex)
            slotStepBack(actMode);
    }
    slotPutItemsInStr8Line();
    emit droppedToDefault();
}

void SwapManager::slotPutItemsInStr8Line()
{
    auto items = window.items();
    int noItems = items.size();
    int noSpace = noItems + 1;
    double viewWidth = window.width();
    double viewHeight = window.height();
    double elementWidth = viewWidth * 0.8 / (double)noItems;
    double spaceWidth	= viewWidth * 0.2 / (double)noSpace;
    QPointF startPoint(spaceWidth, viewHeight/2.0 - elementWidth/2.0);
    for (auto item : items)
    {
        QGraphicsWidget *wgt = static_cast<QGraphicsWidget*>(item);
        wgt->resize(elementWidth, elementWidth);
        wgt->setPos(startPoint.x(), startPoint.y());
        wgt->setVisible(true);
        startPoint.setX(startPoint.x() + spaceWidth + elementWidth);
    }
}

// Must be connected with start/stop toggle button
void SwapManager::slotHandleToggleStartStop(bool checked)
{
    if (checked) // If button is pressed that means that animation sequence must run on it's own
    {
        slotStepForward();
        startNextPairActionTimer();
    }
    else
    {
        stopNextPairActionTimer();
    }
}

void SwapManager::slotSetCurrentSortType(QString stype)
{
    currentSortType = stype;
}
