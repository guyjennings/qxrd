#ifndef QXRDEXTRAOUTPUTSPLOTWIDGET_H
#define QXRDEXTRAOUTPUTSPLOTWIDGET_H

#include "qxrdplotwidget.h"
#include "qxrdextraoutputsplotwidgetsettings-ptr.h"
#include "qxrdsynchronizedoutputchannel-ptr.h"

class QxrdExtraOutputsPlotWidget : public QxrdPlotWidget
{
  Q_OBJECT

public:
  explicit QxrdExtraOutputsPlotWidget(QWidget *parent);

  //TODO: change to QcepObjectWPtr
  void initialize(QxrdExtraOutputsPlotWidgetSettingsWPtr settings);

  void clear();
  void plotChannel(QxrdSynchronizedOutputChannelWPtr chan);
  void replot();

private:
  QVector<QwtPlotCurve*> m_Curves;
  QVector<QColor>        m_Colors;
};

#endif // QXRDEXTRAOUTPUTSPLOTWIDGET_H
