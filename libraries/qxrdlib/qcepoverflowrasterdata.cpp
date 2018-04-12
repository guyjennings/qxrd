#include "qxrddebug.h"
#include "qcepoverflowrasterdata.h"
#include "qxrdapplication.h"
#include "qcepimageplotwidgetsettings.h"

QcepOverflowRasterData::QcepOverflowRasterData(QcepImageDataBaseWPtr           img,
                                               QcepImagePlotWidgetSettingsWPtr set)
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

void QcepOverflowRasterData::setImage(QcepImageDataBaseWPtr img)
{
  m_ImageData = img;

  setIntervals();
}

void QcepOverflowRasterData::setIntervals()
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
double QcepOverflowRasterData::value(double x, double y) const
{
  return 0;
}

int QcepOverflowRasterData::width() const
{
  return m_NCols;
}

int QcepOverflowRasterData::height() const
{
  return m_NRows;
}

