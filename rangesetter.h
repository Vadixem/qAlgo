#ifndef RANGESETTER_H
#define RANGESETTER_H
#include <QSpinBox>
#include <QObject>
#include <QPushButton>

/**
 * @brief The RangeSetter class consists of two spin boxes for setting range for selected sorting items
 */
class RangeSetter : public QWidget
{
    Q_OBJECT
public:
    RangeSetter(int min = 0, int max = 100, int minDiff = 100, QWidget *par = 0);
    QSpinBox *pstartSpin, *pendSpin;
    QPushButton	*pcmdSwap;

    int getDifference() const;

signals:
    void rangeUpdated(int, int);

public slots:
    void slotUpdateMinimumDifference(int);
    void slotUpdateRange();
    void slotSwapValues();

private:
    int absoluteMinimum;
    int absoluteMaximum;
    int minimumDifference;

};

#endif // RANGESETTER_H
