#include "qxrdscatterplotgraphcontrols.h"
#include "ui_qxrdscatterplotgraphcontrols.h"

QxrdScatterPlotGraphControls::QxrdScatterPlotGraphControls(QxrdDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object) :
  QxrdGraphControlsWidget(window, mode, object)
{
  setupUi(this);
}

QxrdScatterPlotGraphControls::~QxrdScatterPlotGraphControls()
{
}

void QxrdScatterPlotGraphControls::changeEvent(QEvent *e)
{
  QWidget::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}
