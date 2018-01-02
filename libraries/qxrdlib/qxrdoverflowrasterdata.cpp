#include "qxrddebug.h"
#include "qxrdoverflowrasterdata.h"
#include "qxrdapplication.h"

QxrdOverflowRasterData::QxrdOverflowRasterData(QcepImageDataBasePtr data,
                                               int interp,
                                               double level)
  : QwtRasterData(),
    m_Data(data),
    m_NRows((data ? data->get_Height(): 0)),
    m_NCols((data ? data->get_Width() : 0)),
    m_OverflowLevel(level),
    m_Interpolate(interp)
{
  if (g_Application && qcepDebug(DEBUG_IMAGES)) {
    g_Application->printMessage(QObject::tr("QxrdMaskRasterData::QxrdMaskRasterData(%1,%2) [%3]")
                                .HEXARG(data.data()).arg(interp).HEXARG(this));
  }

  setInterval(Qt::XAxis, QwtInterval(0, (data?data->get_Width():0)));
  setInterval(Qt::YAxis, QwtInterval(0, (data?data->get_Height():0)));
  setInterval(Qt::ZAxis, range());
}

double QxrdOverflowRasterData::value(double x, double y) const
{
  return 0;
}

QwtInterval QxrdOverflowRasterData::range() const
{
  return QwtInterval(0.0, 3.0);
}

int QxrdOverflowRasterData::width() const
{
  return m_NCols;
}

int QxrdOverflowRasterData::height() const
{
  return m_NRows;
}

