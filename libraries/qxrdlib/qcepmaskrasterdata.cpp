#include "qxrddebug.h"
#include "qcepmaskrasterdata.h"

#include "qceprasterdata.h"
#include "qxrdapplication.h"
#include "qcepimageplotwidgetsettings.h"

QcepMaskRasterData::QcepMaskRasterData(QcepMaskDataWPtr                mask,
                                       QcepImagePlotWidgetSettingsWPtr set)
  : QwtRasterData(),
    m_MaskData(mask),
    m_Settings(set)
{
  if (g_Application && qcepDebug(DEBUG_IMAGES)) {
    g_Application->printMessage(QObject::tr("QxrdMaskRasterData::QxrdMaskRasterData(%1,%2) [%3]")
                                .HEXARG(mask.data()).HEXARG(set.data()).HEXARG(this));
  }

  setIntervals();
}

void QcepMaskRasterData::setIntervals()
{
  QcepMaskDataPtr mask(m_MaskData);

  if (mask) {
    m_NRows = mask->get_Height();
    m_NCols = mask->get_Width();
  } else {
    m_NRows = 0;
    m_NCols = 0;
  }

  setInterval(Qt::XAxis, QwtInterval(0, m_NCols));
  setInterval(Qt::YAxis, QwtInterval(0, m_NRows));
  setInterval(Qt::ZAxis, QwtInterval(0.0, 3.0));
}

void QcepMaskRasterData::setMask(QcepMaskDataWPtr mask)
{
  m_MaskData = mask;

  setIntervals();
}

double QcepMaskRasterData::value(double x, double y) const
{
  QcepMaskDataPtr mask(m_MaskData);

  if (mask) {
    return mask->maskValue(((int) qRound(x)), ((int) qRound(y)));
  } else {
    return 1;
  }
}

int QcepMaskRasterData::width() const
{
  return m_NCols;
}

int QcepMaskRasterData::height() const
{
  return m_NRows;
}
