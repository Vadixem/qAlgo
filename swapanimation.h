#ifndef SWAPANIMATION_H
#define SWAPANIMATION_H
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QPointer>

class SwapAnimation : public QPropertyAnimation
{
   QPointer<QObject> other;
   qreal offset;
   QPoint propertyOf(QObject *obj) {
      return obj->property(propertyName().constData()).toPoint();
   }
   void updateState(State newState, State oldState) override
   {
      if (newState == Running && oldState == Stopped)
      {
         auto start = propertyOf(targetObject());
         auto end = propertyOf(other);
         auto step1 = fabs(offset);

         auto step2 = QLineF(start,end).length();
         auto steps = 2.0*step1 + step2;
         setStartValue(start);
         setKeyValueAt(step1/steps, QPoint(start.x(), start.y() + offset));
         setKeyValueAt((step1+step2)/steps, QPoint(end.x(), end.y() + offset));
         setEndValue(end);
      }
      QPropertyAnimation::updateState(newState, oldState);
   }
public:
   SwapAnimation(QObject *first, QObject *second, int duration, qreal offset) : other(second), offset(offset) {
      setTargetObject(first);
      setPropertyName("pos");
      setDuration(duration);
   }
};


QParallelAnimationGroup* getSwapAnimation(QObject *obj1, QObject *obj2, int duration ,double swapHeightRatio = 0.75)
{
   auto swapHeight = obj1->property("geometry").toRectF().height() * swapHeightRatio;
   auto par = new QParallelAnimationGroup;
   par->addAnimation(new SwapAnimation(obj2, obj1, duration, -swapHeight));
   par->addAnimation(new SwapAnimation(obj1, obj2, duration,  swapHeight));
   return par;
}
#endif // SWAPANIMATION_H
