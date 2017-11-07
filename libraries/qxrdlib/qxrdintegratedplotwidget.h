#ifndef QXRDINTEGRATEDPLOTWIDGET_H
#define QXRDINTEGRATEDPLOTWIDGET_H

#include <QWidget>

namespace Ui {
class QxrdIntegratedPlotWidget;
}

class QxrdIntegratedPlotWidget : public QWidget
{
  Q_OBJECT

public:
  explicit QxrdIntegratedPlotWidget(QWidget *parent = 0);
  ~QxrdIntegratedPlotWidget();

protected:
  void changeEvent(QEvent *e);

private:
  Ui::QxrdIntegratedPlotWidget *ui;
};

#endif // QXRDINTEGRATEDPLOTWIDGET_H
