#ifndef QXRDCENTERINGPLOTWIDGET_H
#define QXRDCENTERINGPLOTWIDGET_H

#include <QWidget>

namespace Ui {
class QxrdCenteringPlotWidget;
}

class QxrdCenteringPlotWidget : public QWidget
{
  Q_OBJECT

public:
  explicit QxrdCenteringPlotWidget(QWidget *parent = 0);
  ~QxrdCenteringPlotWidget();

protected:
  void changeEvent(QEvent *e);

private:
  Ui::QxrdCenteringPlotWidget *ui;
};

#endif // QXRDCENTERINGPLOTWIDGET_H
