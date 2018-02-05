#include "qxrdtestimagewindowqwt.h"
#include "qxrddebug.h"
#include "qcepimagedata.h"

QxrdTestImageWindowQwt::QxrdTestImageWindowQwt(QWidget *parent) :
  QxrdTestImageWindow(parent)
{
  setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);

  m_GraphSettings = QxrdImagePlotSettingsPtr(
        new QxrdImagePlotSettings("settings"));

  m_Graph->initialize(m_GraphSettings, QcepObjectPtr());
}

QxrdTestImageWindowQwt::~QxrdTestImageWindowQwt()
{
}

QxrdTestImagePlotQwtHelperPtr QxrdTestImageWindowQwt::helper()
{
  return m_Graph->helper();
}
