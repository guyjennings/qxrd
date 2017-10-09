#ifdef HAVE_DATAVIS

#include "qxrdtestimagewindowdatavis.h"
#include "qcepdebug.h"
#include "qcepimagedata.h"

QxrdTestImageWindowDataVis::QxrdTestImageWindowDataVis(QWidget *parent) :
  QxrdTestImageWindow(parent)
{
  setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose);
}

QxrdTestImageWindowDataVis::~QxrdTestImageWindowDataVis()
{
}

QxrdTestImagePlotDataVisHelperPtr QxrdTestImageWindowDataVis::helper()
{
  return m_Graph->helper();
}

#endif // HAVE_DATAVIS
