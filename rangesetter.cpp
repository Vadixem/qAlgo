#include "rangesetter.h"
#include <QHBoxLayout>

RangeSetter::RangeSetter(int min, int max, int minDiff, QWidget *par)
    : absoluteMinimum(min), absoluteMaximum(max), minimumDifference(minDiff), QWidget(par)
{
    pstartSpin = new QSpinBox(this);
    pstartSpin->setMinimum(min);
    pstartSpin->setMaximum(max);
    pstartSpin->setValue(max);

    pendSpin = new QSpinBox(this);
    pendSpin->setMaximum(min);
    pendSpin->setMaximum(max);
    pendSpin->setValue(min);

    pcmdSwap = new QPushButton("<-Swap->", this);
    connect(pcmdSwap, SIGNAL(clicked(bool)), this, SLOT(slotSwapValues()));

    // Layout setup
    QHBoxLayout *phbxLayout = new QHBoxLayout;
    phbxLayout->addWidget(pstartSpin);
    phbxLayout->addWidget(pcmdSwap);
    phbxLayout->addWidget(pendSpin);

    this->setLayout(phbxLayout);
}

void RangeSetter::slotUpdateRange()
{

    if (getDifference() >= minimumDifference)
    {
        emit rangeUpdated(pstartSpin->value(), pendSpin->value());
        return;
    }

    int startVal = pstartSpin->value();
    int endVal = pendSpin->value();

    if ((startVal == endVal) && (startVal == absoluteMinimum))
    {
        emit rangeUpdated(startVal = absoluteMinimum, endVal = absoluteMinimum + minimumDifference);
    }
    else
    {
        emit rangeUpdated(startVal = absoluteMaximum - minimumDifference, endVal = absoluteMaximum);
    }
    pstartSpin->setValue(startVal);
    pendSpin->setValue(endVal);
}

void RangeSetter::slotUpdateMinimumDifference(int diff)
{
    if (!diff)
        minimumDifference = 100;
    else
        minimumDifference = diff;
}

int RangeSetter::getDifference() const
{
    return abs(pstartSpin->value() - pendSpin->value());
}

void RangeSetter::slotSwapValues()
{
    auto temp = pstartSpin->value();
    pstartSpin->setValue(pendSpin->value());
    pendSpin->setValue(temp);
}
