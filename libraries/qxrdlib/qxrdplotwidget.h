#ifndef QXRDPLOTWIDGET_H
#define QXRDPLOTWIDGET_H

#include <QWidget>
#include "ui_qxrdplotwidget.h"
#include "qxrdplotcommand-ptr.h"

class QXRD_EXPORT QxrdPlotWidget : public QWidget, public Ui::QxrdPlotWidget
{
  Q_OBJECT

public:
  explicit QxrdPlotWidget(QWidget *parent = 0);
  ~QxrdPlotWidget();

  void addPlotCommand(QxrdPlotCommandPtr cmd);
  void addPlotCommandSpacer();

private slots:
  void contextMenu(const QPoint &pos);

private:
  QVector<QxrdPlotCommandPtr> m_PlotCommands;
};

#endif // QXRDPLOTWIDGET_H
