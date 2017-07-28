#include "qxrdtestimageplotqwt.h"
#include "qxrdtestimageplotqwthelper.h"


QxrdTestImagePlotQwt::QxrdTestImagePlotQwt(QWidget *parent)
  : QxrdImagePlot(parent)
{
  m_HelperThread = QSharedPointer<QxrdTestThread<QxrdTestImagePlotQwtHelper> > (
        new QxrdTestThread<QxrdTestImagePlotQwtHelper>(QcepObjectWPtr()));

  m_HelperThread->start();

  m_Helper = m_HelperThread->object();

  if (m_Helper) {
    connect(m_Helper.data(), &QxrdTestImagePlotQwtHelper::newPlotSpectrogramAvailable,
            this,            &QxrdTestImagePlotQwt::onNewPlotSpectrogramAvailable);
  }
}

QxrdTestImagePlotQwtHelperPtr QxrdTestImagePlotQwt::helper()
{
  return m_Helper;
}

void QxrdTestImagePlotQwt::onNewPlotSpectrogramAvailable(QcepDoubleImageDataPtr img)
{
  onProcessedImageAvailable(img, QcepMaskDataPtr());
}

