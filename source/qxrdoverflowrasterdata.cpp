#include "qxrddebug.h"
#include "qxrdoverflowrasterdata.h"
#include "qxrdimagedata.h"
#include "qxrdapplication.h"

QxrdOverflowRasterData::QxrdOverflowRasterData(QxrdDoubleImageDataPtr image, double level)
  : QwtRasterData(),
    m_Image(image),
    m_OverflowLevel(level)
{
  if (g_Application && qcepDebug(DEBUG_IMAGES)) {
    g_Application->printMessage(QObject::tr("QxrdOverflowRasterData::QxrdMaskRasterData(%1) [%2]")
                                .HEXARG(image.data()).HEXARG(this));
  }

  setInterval(Qt::XAxis, QwtInterval(0, (image?image->get_Width():0)));
  setInterval(Qt::YAxis, QwtInterval(0, (image?image->get_Height():0)));
  setInterval(Qt::ZAxis, QwtInterval(0,3));

  if (image) {
    if (image->get_SummedExposures()>1) {
      m_OverflowLevel = level*image->get_SummedExposures();
    }
  }
}

double QxrdOverflowRasterData::value(double x, double y) const
{
  double res = 0;

  if (m_Image) {
    double val = m_Image->value((int) qRound(x), (int) qRound(y));

    if (val == val && val > m_OverflowLevel) {
      res = 1;
    }
  }

  return res;
}
