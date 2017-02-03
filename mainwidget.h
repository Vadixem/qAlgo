#pragma once
#include <QWidget>
#include "sortwidget.h"

class MainWidget : public QWidget
{
public:
  MainWidget(QWidget *par = 0) : QWidget(par)
  {
  }

  void setChildrenContolsVisible(bool visible = true)
  {
      for (auto child : this->children())
      {
          if (QString(child->metaObject()->className()) == QString("SortWidget"))
          {
              static_cast<SortWidget*>(child)->setControlsVisible(visible);
          }
      }
  }
};
