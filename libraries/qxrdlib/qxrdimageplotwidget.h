#ifndef QXRDIMAGEPLOTWIDGET_H
#define QXRDIMAGEPLOTWIDGET_H

#include "qxrdplotwidget.h"

class QXRD_EXPORT QxrdImagePlotWidget : public QxrdPlotWidget
{
  Q_OBJECT

public:
  explicit QxrdImagePlotWidget(QWidget *parent = 0);
  ~QxrdImagePlotWidget();
};

#endif // QXRDIMAGEPLOTWIDGET_H
