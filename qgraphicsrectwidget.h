#ifndef QGRAPHICSRECTWIDGET_H
#define QGRAPHICSRECTWIDGET_H
#include <QGraphicsWidget>
#include <QPainter>
#include <QObject>
#include <QDebug>

/**
 * @brief The QGraphicsRectWidget class represents a simple element of array to be swapped
 */
class QGraphicsRectWidget : public QGraphicsWidget
{
    Q_OBJECT
public:
    int m_number;

    QGraphicsRectWidget(QGraphicsItem *parent = 0, int m_number = 1)
        : QGraphicsWidget(parent), m_number(m_number)
    {

    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,
               QWidget *) Q_DECL_OVERRIDE
    {
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setBrush(QBrush(QColor(51, 153, 255)));
        painter->drawEllipse(rect());
        QFont font = painter->font();
        font.setFamily("Comic Sans MS");
        font.setPointSize(rect().height()/2.5);
        painter->setFont(font);
        painter->drawText(rect(), Qt::AlignCenter, QString("%1").arg(m_number) );

    }

};

#endif // QGRAPHICSRECTWIDGET_H
