#include "sortwidget.h"
#include <QTime>
#include <QLabel>
#include "qgraphicsrectwidget.h"
#include "sortwidgeteventhandler.h"

// ================================================================== SORT FUNCTIONS =============================================================================


bool less(QVector<int> &arr, int a, int b, SwapManager& manager)
{
    manager.addPair(a, b, AnimationType::Comparison);
    return arr[a] < arr[b];
}

void qswap(QVector<int> &arr, int a, int b)
{
    int temp = arr.at(a);
    arr.replace(a,arr.at(b));
    arr.replace(b, temp);
}


void bubble_sort(GraphicsView &window, SwapManager &manager)
{
    auto list = window.items();
    QVector<int> arr;
    for (auto item : list)
        arr.push_back(static_cast<QGraphicsRectWidget*>(item)->m_number);

    for(int i = 1; i < arr.size(); ++i)
    {
        for(int j = 0; j < (arr.size() - i); ++j)
            if (less(arr, j+1, j, manager))
            {
                qswap(arr, j+1, j);
                manager.addPair(j+1, j);
            }
    }

}

void shaker_sort(GraphicsView &window, SwapManager &manager)
{
    auto list = window.items();
    QVector<int> arr;
    for (auto item : list)
        arr.push_back(static_cast<QGraphicsRectWidget*>(item)->m_number);


    for (int i = 0; i < arr.size()/2; i++)
    {
        bool swapped = false;
        //one way
        for (int j = i; j < arr.size() - i - 1; j++)
        {
            if (less(arr, j + 1, j, manager))
            {
                qswap(arr, j, j + 1);
                manager.addPair(j, j + 1);
                swapped = true;
            }
        }
        //and back
        for (int j = arr.size() - 2 - i; j > i; j--)
        {
            if (less(arr, j, j - 1, manager))
            {
                qswap(arr, j, j - 1);
                manager.addPair(j, j - 1);
                swapped = true;
            }
        }
        if(!swapped) break; //block (break if no element was swapped - the array is sorted)
    }
}

void shell_sort(GraphicsView &window, SwapManager &manager)
{
    auto list = window.items();
    QVector<int> arr;
    for (auto item : list)
        arr.push_back(static_cast<QGraphicsRectWidget*>(item)->m_number);
    int N = arr.size();
    int h = 1;
    while (h < N/3) h = 3*h + 1;
    while (h >= 1)
    {
        for (int i = h; i < N; ++i)
        {
            for (int j = i; j >= h && less(arr, j, j-h, manager); j -= h)
            {
                qswap(arr, j, j-h);
                manager.addPair(j, j - h);
            }
        }
        h /= 3;
    }
}

void ins_sort(GraphicsView &window, SwapManager &manager)
{
    auto list = window.items();
    int i, j;
    QVector<int> arr;

    for (auto item : list)
    {
        arr.push_back(static_cast<QGraphicsRectWidget*>(item)->m_number);
    }
    int N = arr.size();
    for (i = 1.0; i < N; ++i)
    {
        for (j = i - 1; j != -1  && less(arr, j + 1, j, manager); --j)
        {
            qswap(arr, j, j + 1);
            manager.addPair(j, j + 1);
        }
    }
}

// ================================================ QUICK SORT ========================================================

int partition(QVector<int> &arr, int lo, int hi, SwapManager &manager)
{
    int i = lo;
    int j = hi + 1;
    while (true)
    {
        while (less(arr, ++i, lo, manager)) if (i == hi) break;
        while (less(arr, lo, --j, manager)) if (j == lo) break;
        if (i >= j) break;
        qswap(arr, i, j);
        manager.addPair(i, j);

    }
    qswap(arr, lo, j);
    manager.addPair(lo, j);

    return j;
}

void quick_sort(QVector<int> &arr, int lo, int hi, SwapManager &manager)
{
    if (hi <= lo) return;
    int j = partition(arr, lo, hi, manager);
    quick_sort(arr, lo, j-1, manager);
    quick_sort(arr, j+1, hi, manager);
}

void quick_sort(GraphicsView &window, SwapManager &manager)
{
    auto list = window.items();
    QVector<int> arr;

    for (auto item : list)
    {
        arr.push_back(static_cast<QGraphicsRectWidget*>(item)->m_number);
    }

    int len = arr.size();

    quick_sort(arr, 0, len - 1, manager);
}

// ================================================ !QUICK SORT ========================================================


// ================================================================== !SORT FUNCTIONS =============================================================================


SortWidget::SortWidget(QWidget *par, int numberOfSortElements)
    : QWidget(par), numberOfSortElements(numberOfSortElements), minSortItemQty(5), maxSortItemQty(50)
{
    installEventFilter(new SortWidgetEventHandler(this));
    srand(QTime(0,0,0).secsTo(QTime::currentTime()));
    QList<QGraphicsRectWidget*> wgtList;
    QGraphicsRectWidget* pwgt;
    for (int i = 0; i < numberOfSortElements; ++i)
    {
        pwgt = new QGraphicsRectWidget(Q_NULLPTR, rand() % 100);
        pwgt->setZValue(i);
        wgtList.push_back(pwgt);
    }

    pscene = new QGraphicsScene(0, 0, 300, 300, this);
    pscene->setBackgroundBrush(QColor(0, 51, 80));
    for (auto wgt : wgtList)
        pscene->addItem(wgt);

    pwindow = new GraphicsView(pscene, this);

    pwindow->setFrameStyle(0);
    pwindow->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    pwindow->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pwindow->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Pointer to map of strings and function pointers. Those function pointers are to be used when filling action pairs.
    auto sortFunctions = new QMap<QString, void (*)(GraphicsView&, SwapManager&)>();
    sortFunctions->insert("Insertion Sort", ins_sort);
    sortFunctions->insert("Shell Sort", shell_sort);
    sortFunctions->insert("Quick Sort", quick_sort);
    sortFunctions->insert("Bubble Sort", bubble_sort);
    sortFunctions->insert("Shaker Sort", shaker_sort);

    pmanager = new SwapManager(*pwindow, sortFunctions, 1500, this);
    pmanager->setTimerInterval(1500);

    pcmdStartStop = new QPushButton("&Start", this);
    pcmdStartStop->setCheckable(true);
    pcmdNextStep = new QPushButton("Step &forward", this);
    pcmdPrevStep = new QPushButton("Step &back", this);

    // Perform a single animation forward
    QObject::connect(pcmdNextStep, SIGNAL(clicked(bool)), pmanager, SLOT(slotStepForward()));
    // Perform a single animation back
    QObject::connect(pcmdPrevStep, SIGNAL(clicked(bool)), pmanager, SLOT(slotStepBack()));
    // Handle animation sequence
    QObject::connect(pcmdStartStop, SIGNAL(toggled(bool)), pmanager, SLOT(slotHandleToggleStartStop(bool)));

    // Manage number of sort items
    pNoItemSpin = new QSpinBox(this);
    pNoItemSpin->setRange(minSortItemQty, maxSortItemQty);
    pNoItemSpin->setAutoFillBackground(true);
    pNoItemSpin->setValue(numberOfSortElements);
    {
        QObject::connect(pNoItemSpin, SIGNAL(valueChanged(int)), this, SLOT(setNoSortItems(int)));
        // When number of sort items is changed, we drop the  sort sequence
        QObject::connect(pNoItemSpin, SIGNAL(valueChanged(int)), pmanager, SLOT(slotDropToDefault()));
    }

    // ListWidget for sorting type
    pSortTypeList = new QListWidget;
    pSortTypeList->setSelectionMode(QAbstractItemView::SingleSelection);
    pSortTypeList->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
    QListWidgetItem *plistItem = 0;
    // Fill it with names of sortings
    for (auto key : sortFunctions->keys())
    {
        plistItem = new QListWidgetItem(key, pSortTypeList);
        plistItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    }
    pSortTypeList->setMaximumHeight(pSortTypeList->sizeHintForRow(0) * 4);
    {
        // When option is chosen the sequence of swaps is generated and the previous sequence is deleted
        QObject::connect(pSortTypeList, SIGNAL(currentTextChanged(QString)), pmanager, SLOT(slotDropToDefault()));
        QObject::connect(pSortTypeList, SIGNAL(currentTextChanged(QString)), pmanager, SLOT(slotSetCurrentSortType(QString)));
    }

    // When button is pressed and some animation on items is done it triggers slot that restores order of items
    pcmdRestoreOrder = new QPushButton("&Restore Order", this);
    QObject::connect(pcmdRestoreOrder, SIGNAL(clicked(bool)), pmanager, SLOT(slotRestoreInitialOrder()));

    // Slider for speed adjustment
    pspeedSlider = new QSlider(Qt::Horizontal, pwindow);
    pspeedSlider->setRange(1, 7);
    pspeedSlider->setPageStep(1);
    pspeedSlider->setValue(5);
    pspeedSlider->setTickInterval(1);
    pspeedSlider->setTickPosition(QSlider::TicksBelow);
    QObject::connect(pspeedSlider, SIGNAL(valueChanged(int)) ,pmanager, SLOT(slotHandleSpeedSlider(int)));

    pcmdRandomize = new QPushButton("Randomize", this);
    {
        QObject::connect(pcmdRandomize, SIGNAL(clicked(bool)), this, SLOT(slotFillSortItemsWithRand()));
        QObject::connect(pcmdRandomize, SIGNAL(clicked(bool)), pmanager, SLOT(slotDropToDefault()));
    }

    prangeSetter = new RangeSetter(0, 100, pwindow->items().size(), this);
    {
        QObject::connect(prangeSetter, SIGNAL(rangeUpdated(int,int)), this, SLOT(slotFillSortItemsInRange(int, int)));
        QObject::connect(this, SIGNAL(noOfSortItemsChanged(int)), prangeSetter, SLOT(slotUpdateMinimumDifference(int)));
    }

    pcmdFillInRange = new QPushButton("In range", this);
    {
        QObject::connect(pcmdFillInRange, SIGNAL(clicked(bool)), prangeSetter, SLOT(slotUpdateRange()));
        QObject::connect(pcmdFillInRange, SIGNAL(clicked(bool)), pmanager, SLOT(slotDropToDefault()));
    }

    // ======================================== STATE MACHINE ==============================
    pmachine = new QStateMachine(this);

    // State which greets the user
    QState *pnullState = new QState;
    pnullState->assignProperty(pcmdRestoreOrder,	"enabled", false);
    pnullState->assignProperty(pcmdPrevStep,		"enabled", false);
    pnullState->assignProperty(pcmdNextStep,		"enabled", false);
    pnullState->assignProperty(pcmdStartStop,		"enabled", false);
    pnullState->assignProperty(pspeedSlider,		"enabled", true);
    pnullState->assignProperty(pSortTypeList,		"enabled", true);
    pnullState->assignProperty(pspeedSlider,		"enabled", true);
    pnullState->assignProperty(pNoItemSpin,			"enabled", true);
    pnullState->assignProperty(pcmdFillInRange,		"enabled", false);
    pnullState->assignProperty(pcmdRandomize,		"enabled", false);

    // State in which user can apply advanced settings to sorting sequence
    QState *pinitState = new QState;
    pinitState->assignProperty(pcmdRestoreOrder,	"enabled", false);
    pinitState->assignProperty(pcmdPrevStep,		"enabled", false);
    pinitState->assignProperty(pcmdNextStep,		"enabled", true);
    pinitState->assignProperty(pcmdStartStop,		"enabled", true);
    pinitState->assignProperty(pcmdStartStop,		"checked", false);
    pinitState->assignProperty(pspeedSlider,		"enabled", true);
    pinitState->assignProperty(pSortTypeList,		"enabled", true);
    pinitState->assignProperty(pNoItemSpin,			"enabled", true);
    pinitState->assignProperty(pcmdFillInRange,		"enabled", true);
    pinitState->assignProperty(pcmdRandomize,		"enabled", true);

    // State in which there's no more forward animation to perform
    QState *pSortFinishedState = new QState;
    pSortFinishedState->assignProperty(pcmdRestoreOrder,	"enabled", true );
    pSortFinishedState->assignProperty(pcmdPrevStep,		"enabled", true );
    pSortFinishedState->assignProperty(pcmdNextStep,		"enabled", false);
    pSortFinishedState->assignProperty(pcmdStartStop,		"enabled", false);
    pSortFinishedState->assignProperty(pcmdStartStop,		"checked", false);
    pSortFinishedState->assignProperty(pcmdStartStop,		"text", "&Start");
    pSortFinishedState->assignProperty(pspeedSlider,		"enabled", true );
    pSortFinishedState->assignProperty(pSortTypeList,		"enabled", true );
    pSortFinishedState->assignProperty(pNoItemSpin,			"enabled", true);
    pSortFinishedState->assignProperty(pcmdFillInRange,		"enabled", true);
    pSortFinishedState->assignProperty(pcmdRandomize,		"enabled", true);

    // State in which animation is running automatically
    QState *pAutoAnimationState = new QState;
    pAutoAnimationState->assignProperty(pcmdRestoreOrder,	"enabled", false);
    pAutoAnimationState->assignProperty(pcmdPrevStep,		"enabled", false);
    pAutoAnimationState->assignProperty(pcmdNextStep,		"enabled", false);
    pAutoAnimationState->assignProperty(pcmdStartStop,		"enabled", true );
    pAutoAnimationState->assignProperty(pcmdStartStop,		"checked", true );
    pAutoAnimationState->assignProperty(pcmdStartStop,		"text",  "&Stop");
    pAutoAnimationState->assignProperty(pspeedSlider,		"enabled", true );
    pAutoAnimationState->assignProperty(pSortTypeList,		"enabled", false);
    pAutoAnimationState->assignProperty(pNoItemSpin,		"enabled", false);
    pAutoAnimationState->assignProperty(pcmdFillInRange,	"enabled", false);
    pAutoAnimationState->assignProperty(pcmdRandomize,		"enabled", false);

    // Means that u have iterator that is not pointing either to the beginning of anim sequence or end of a sequence
    QState *pManualAnimationModeState = new QState;
    pManualAnimationModeState->assignProperty(pcmdRestoreOrder,	"enabled", true );
    pManualAnimationModeState->assignProperty(pcmdPrevStep,		"enabled", true );
    pManualAnimationModeState->assignProperty(pcmdNextStep,		"enabled", true );
    pManualAnimationModeState->assignProperty(pcmdStartStop,	"enabled", true );
    pManualAnimationModeState->assignProperty(pcmdStartStop,	"checked", false);
    pManualAnimationModeState->assignProperty(pcmdStartStop,	"text", "&Start");
    pManualAnimationModeState->assignProperty(pspeedSlider,		"enabled", true );
    pManualAnimationModeState->assignProperty(pSortTypeList,	"enabled", true );
    pManualAnimationModeState->assignProperty(pNoItemSpin,		"enabled", true );
    pManualAnimationModeState->assignProperty(pcmdFillInRange,	"enabled", true);
    pManualAnimationModeState->assignProperty(pcmdRandomize,	"enabled", true);

        // ================================== TRANSITIONS ====================


        pnullState					->addTransition(pSortTypeList,		SIGNAL(currentTextChanged(QString)),	pinitState);
        pManualAnimationModeState	->addTransition(pmanager,			SIGNAL(indexReachedBegin()),			pinitState);
        pManualAnimationModeState	->addTransition(this,				SIGNAL(noOfSortItemsChanged(int)),		pinitState);
        pManualAnimationModeState	->addTransition(pmanager,			SIGNAL(droppedToDefault()),				pinitState);
        pSortFinishedState			->addTransition(pmanager,			SIGNAL(indexReachedBegin()),			pinitState);
        pSortFinishedState			->addTransition(pSortTypeList,		SIGNAL(currentTextChanged(QString)),	pinitState);
        pSortFinishedState			->addTransition(pmanager,			SIGNAL(droppedToDefault()),				pinitState);

        pinitState					->addTransition(pmanager,			SIGNAL(indexReachedEnd()),				pSortFinishedState);
        pAutoAnimationState			->addTransition(pmanager,			SIGNAL(indexReachedEnd()),				pSortFinishedState);
        pManualAnimationModeState	->addTransition(pmanager,			SIGNAL(indexReachedEnd()),				pSortFinishedState);

        pinitState					->addTransition(pmanager,			SIGNAL(actionTimerStarted()),			pAutoAnimationState);
        pManualAnimationModeState	->addTransition(pmanager,			SIGNAL(actionTimerStarted()),			pAutoAnimationState);

        pAutoAnimationState			->addTransition(pmanager,			SIGNAL(actionTimerStoppedInMiddle()),	pManualAnimationModeState);
        pinitState					->addTransition(pcmdNextStep,		SIGNAL(clicked(bool)),					pManualAnimationModeState);
        pSortFinishedState			->addTransition(pcmdPrevStep,		SIGNAL(clicked(bool)),					pManualAnimationModeState);


        // ================================== !TRANSITIONS ====================

    pmachine->addState(pnullState);
    pmachine->addState(pinitState);
    pmachine->addState(pSortFinishedState);
    pmachine->addState(pAutoAnimationState);
    pmachine->addState(pManualAnimationModeState);

    pmachine->setInitialState(pnullState);
    pmachine->start();
    // ======================================== !STATE MACHINE ==============================

    // Layout setup
    QVBoxLayout *pmainVLayout = new QVBoxLayout;
        QHBoxLayout *phbxTopLayout = new QHBoxLayout;
            QVBoxLayout *pvRegLayout = new QVBoxLayout;
            QVBoxLayout *pvOrderRegLayout = new QVBoxLayout;
        QHBoxLayout *phbxLayout = new QHBoxLayout;

    QHBoxLayout *phbxSpeedLayout = new QHBoxLayout;
    phbxSpeedLayout->addWidget(new QLabel("slow"), 1, Qt::AlignRight);
    phbxSpeedLayout->addWidget(pspeedSlider, 10);
    phbxSpeedLayout->addWidget(new QLabel("fast"), 1, Qt::AlignLeft);

    QHBoxLayout *phbxNoItemsLayout = new QHBoxLayout;
    phbxNoItemsLayout->addWidget(new QLabel("Size of array"), 1);
    phbxNoItemsLayout->addWidget(pNoItemSpin, 10);

    pvOrderRegLayout->addWidget(pcmdRestoreOrder);
    pvOrderRegLayout->addWidget(prangeSetter, 0, Qt::AlignCenter);
    pvOrderRegLayout->addWidget(pcmdFillInRange);
    pvOrderRegLayout->addWidget(pcmdRandomize);

    pvRegLayout->addLayout(phbxNoItemsLayout);
    pvRegLayout->addWidget(pSortTypeList);
    pvRegLayout->addLayout(phbxSpeedLayout);

    phbxTopLayout->addLayout(pvRegLayout);
    phbxTopLayout->addLayout(pvOrderRegLayout);

    pmainVLayout->addLayout(phbxTopLayout);
    pmainVLayout->addWidget(pwindow);

    phbxLayout->addWidget(pcmdPrevStep);
    phbxLayout->addWidget(pcmdStartStop);
    phbxLayout->addWidget(pcmdNextStep);

    pmainVLayout->addLayout(phbxLayout);

    this->setLayout(pmainVLayout);
    setControlsVisible(false);
}

bool SortWidget::addSortItem()
{
    if (numberOfSortElements > minSortItemQty || numberOfSortElements < maxSortItemQty)
    {
        QGraphicsRectWidget *wgt = new QGraphicsRectWidget(Q_NULLPTR, rand() % 100);
        ++numberOfSortElements;

        // Add to the end of elements chain
        wgt->setZValue(pscene->items().last()->zValue() - 1);
        pscene->addItem(wgt);
        pmanager->slotPutItemsInStr8Line();
        return true;
    }
    return false;
}

bool SortWidget::deleteSortItem(int index)
{
    if (index && index < numberOfSortElements)
    {
        pscene->removeItem(pscene->items().last());
        pmanager->slotPutItemsInStr8Line();
        --numberOfSortElements;
        return true;
    }
    else return false;
}
void SortWidget::setNoSortItems(int qty)
{
    if (qty == numberOfSortElements) return;
    if (qty > numberOfSortElements)
        while (numberOfSortElements != qty)
            addSortItem();
    if (qty < numberOfSortElements)
        while (numberOfSortElements != qty)
            deleteSortItem(numberOfSortElements - 1);

    emit noOfSortItemsChanged(pwindow->items().size());
}

void SortWidget::setControlsVisible(bool visible)
{
    for (auto child : this->children())
    {
        if (child->metaObject()->className() != pwindow->metaObject()->className())
        {
            child->setProperty("visible", visible);
        }
    }
    resizeEvent(0);

}

void SortWidget::resizeEvent(QResizeEvent*)
{
    if (!pmanager->isAnimationActive())
        pmanager->slotPutItemsInStr8Line();
}

void SortWidget::slotFillSortItemsInRange(int a, int b)
{
    int n = pwindow->items().size();
    qreal val = (qreal)(b - a) / (qreal)(n - 1);
    auto i = (qreal)a;
    for (auto item : pwindow->items())
    {
        static_cast<QGraphicsRectWidget*>(item)->m_number =  qRound(i);
        i += val;
        item->update();
    }
}

void SortWidget::slotFillSortItemsWithRand(int max)
{

    for (auto item : pwindow->items())
    {
        static_cast<QGraphicsRectWidget*>(item)->m_number = rand() % max;
        item->update();
    }
}
