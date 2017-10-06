#include "qxrddebug.h"
#include "qxrdmaskrasterdata.h"

#include "qxrdrasterdata.h"
#include "qxrdapplication.h"

QxrdMaskRasterData::QxrdMaskRasterData(QxrdMaskDataPtr mask)
  : QwtRasterData(),
    m_Mask(mask)
{
  if (g_Application && qcepDebug(DEBUG_IMAGES)) {
    g_Application->printMessage(QObject::tr("QxrdMaskRasterData::QxrdMaskRasterData(%1) [%2]")
                                .HEXARG(mask.data()).HEXARG(this));
  }

  setInterval(Qt::XAxis, QwtInterval(0, (mask?mask->get_Width():0)));
  setInterval(Qt::YAxis, QwtInterval(0, (mask?mask->get_Height():0)));
  setInterval(Qt::ZAxis, QwtInterval(0,3));
}

double QxrdMaskRasterData::value(double x, double y) const
{
  if (m_Mask) {
    return m_Mask->maskValue(((int) qRound(x)), ((int) qRound(y)));
  } else {
    return 1;
  }
}
