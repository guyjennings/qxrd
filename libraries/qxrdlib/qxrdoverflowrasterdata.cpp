#include "qxrddebug.h"
#include "qxrdoverflowrasterdata.h"
#include "qxrdapplication.h"
#include "qxrdimageplotwidgetsettings.h"

QxrdOverflowRasterData::QxrdOverflowRasterData(QcepImageDataBaseWPtr           img,
                                               QxrdImagePlotWidgetSettingsWPtr set)
  : QwtRasterData(),
    m_ImageData(img),
    m_Settings(set)
{
  if (g_Application && qcepDebug(DEBUG_IMAGES)) {
    g_Application->printMessage(QObject::tr("QxrdMaskRasterData::QxrdMaskRasterData(%1,%2) [%3]")
                                .HEXARG(img.data()).HEXARG(set.data()).HEXARG(this));
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

//TODO: implement this...
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

