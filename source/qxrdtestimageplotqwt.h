#ifndef QXRDTESTIMAGEPLOTQWT_H
#define QXRDTESTIMAGEPLOTQWT_H

#include "qxrdimageplot.h"
#include "qxrdtestimageplotqwthelper-ptr.h"
#include "qxrdtestimageplotqwthelper.h"
#include "qxrdtestthread.h"

class QxrdTestImagePlotQwt : public QxrdImagePlot
{
  Q_OBJECT

public:
  QxrdTestImagePlotQwt(QWidget *parent=0);

  QxrdTestImagePlotQwtHelperPtr helper();

public slots:
  void onNewPlotSpectrogramAvailable(QwtPlotSpectrogramPtr img);

private:
  QSharedPointer<QxrdTestThread<QxrdTestImagePlotQwtHelper> > m_HelperThread;
  QxrdTestImagePlotQwtHelperPtr                               m_Helper;
};

#endif // QXRDTESTIMAGEPLOTQWT_H
