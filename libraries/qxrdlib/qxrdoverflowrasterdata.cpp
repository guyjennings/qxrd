#include "qxrddebug.h"
#include "qxrdoverflowrasterdata.h"
#include "qxrdapplication.h"

QxrdOverflowRasterData::QxrdOverflowRasterData(QcepImageDataBaseWPtr data,
                                               double level)
  : QwtRasterData(),
    m_ImageData(data),
    m_OverflowLevel(level)
{
  if (g_Application && qcepDebug(DEBUG_IMAGES)) {
    g_Application->printMessage(QObject::tr("QxrdMaskRasterData::QxrdMaskRasterData(%1) [%2]")
                                .HEXARG(data.data()).HEXARG(this));
  }

  setIntervals();
}

void QxrdOverflowRasterData::setImage(QcepImageDataBaseWPtr img)
{
  m_ImageData = img;

  setIntervals();
}

void QxrdOverflowRasterData::setIntervals()
{
  QcepImageDataBasePtr data(m_ImageData);

  if (data) {
    m_NRows = data->get_Height();
    m_NCols = data->get_Width();
  } else {
    m_NRows = 0;
    m_NCols = 0;
  }

  setInterval(Qt::XAxis, QwtInterval(0, m_NCols));
  setInterval(Qt::YAxis, QwtInterval(0, m_NRows));
  setInterval(Qt::ZAxis, QwtInterval(0.0, 3.0));
}

double QxrdOverflowRasterData::value(double x, double y) const
{
  return 0;
}

int QxrdOverflowRasterData::width() const
{
  return m_NCols;
}

int QxrdOverflowRasterData::height() const
{
  return m_NRows;
}

