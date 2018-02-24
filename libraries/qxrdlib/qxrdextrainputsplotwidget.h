#ifndef QXRDEXTRAINPUTSPLOTWIDGET_H
#define QXRDEXTRAINPUTSPLOTWIDGET_H

#include "qxrdplotwidget.h"
#include "qxrdextrainputsplotwidgetsettings-ptr.h"

class QxrdExtraInputsPlotWidget : public QxrdPlotWidget
{
  Q_OBJECT

public:
  explicit QxrdExtraInputsPlotWidget(QWidget *parent);

  //TODO: change to QcepObjectWPtr
  void initialize(QxrdExtraInputsPlotWidgetSettingsWPtr settings);

//public slots:
//  void setNChannels(int nch);
//  void plotChannel(int ch, int i0, int i1, QcepDoubleVector x, QcepDoubleVector y);

//private:
//  QVector<QwtPlotCurve*> m_RawData;
//  QVector<QwtPlotCurve*> m_Selections;
//  QVector<QColor>        m_Colors;
};

#endif // QXRDEXTRAINPUTSPLOTWIDGET_H
