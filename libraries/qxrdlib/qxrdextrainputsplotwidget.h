#ifndef QXRDEXTRAINPUTSPLOTWIDGET_H
#define QXRDEXTRAINPUTSPLOTWIDGET_H

#include "qcepplotwidget.h"
#include "qxrdextrainputsplotwidgetsettings-ptr.h"
#include "qxrdsynchronizedinputchannel-ptr.h"

class QxrdExtraInputsPlotWidget : public QcepPlotWidget
{
  Q_OBJECT

public:
  explicit QxrdExtraInputsPlotWidget(QWidget *parent);

  //TODO: change to QcepObjectWPtr
  void initialize(QxrdExtraInputsPlotWidgetSettingsWPtr settings);

//public slots:
//  void setNChannels(int nch);
  void clear();
  void plotChannel(QxrdSynchronizedInputChannelWPtr chan);
  void replot();

private:
  QVector<QwtPlotCurve*> m_Curves;
//  QVector<QwtPlotCurve*> m_Selections;
  QVector<QColor>        m_Colors;
};

#endif // QXRDEXTRAINPUTSPLOTWIDGET_H
