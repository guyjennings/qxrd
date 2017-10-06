#include "qxrddebug.h"
#include "qxrdrasterdata.h"
#include "qxrdapplication.h"
#include <QString>

QxrdRasterData::QxrdRasterData(QxrdDoubleImageDataPtr img, int interp, QxrdMaskDataPtr mask, QwtInterval range)
  : QwtRasterData(/*QRectF(0,0,(img?img->get_Width():0),(img?img->get_Height():0))*/),
    m_Data(img),
    m_Mask(mask),
    m_NRows((img ? img->get_Height(): 0)),
    m_NCols((img ? img->get_Width() : 0)),
    m_Range(range),
    m_Interpolate(interp)
{
  if (qcepDebug(DEBUG_IMAGES)) {
    if (g_Application) {
      g_Application->printMessage(QObject::tr("QxrdRasterData::QxrdRasterData(%1,%2,%3) [%4]")
                        .HEXARG(img.data()).arg(interp).HEXARG(mask.data()).HEXARG(this));
    }
  }

  setInterval(Qt::XAxis, QwtInterval(0.0, m_NCols));
  setInterval(Qt::YAxis, QwtInterval(0.0, m_NRows));
  setInterval(Qt::ZAxis, range);
}

double QxrdRasterData::value(double x, double y) const
{
  if (m_Data) {
    if (x < 0 || x > m_NCols) return 0;
    if (y < 0 || y > m_NRows) return 0;

    if (m_Interpolate) {
      return m_Data->value(x, y);
    } else {
      return m_Data->value(((int) qRound(x)) , ((int) qRound(y)));
    }
  } else {
    return 0;
  }
}

QwtInterval QxrdRasterData::range() const
{
  return m_Range;
}

void QxrdRasterData::setDisplayedRange(double min, double max)
{
  m_Range = QwtInterval(min, max);

  setInterval(Qt::ZAxis, m_Range);
}
