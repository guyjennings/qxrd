#ifndef QXRDIMAGEPLOTWIDGET_H
#define QXRDIMAGEPLOTWIDGET_H

#include <QWidget>

namespace Ui {
class QxrdImagePlotWidget;
}

class QxrdImagePlotWidget : public QWidget
{
  Q_OBJECT

public:
  explicit QxrdImagePlotWidget(QWidget *parent = 0);
  ~QxrdImagePlotWidget();

protected:
  void changeEvent(QEvent *e);

private:
  Ui::QxrdImagePlotWidget *ui;
};

#endif // QXRDIMAGEPLOTWIDGET_H
