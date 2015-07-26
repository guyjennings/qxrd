#include "qcepscatterplotgraphcontrols.h"
#include "ui_qcepscatterplotgraphcontrols.h"

QcepScatterPlotGraphControls::QcepScatterPlotGraphControls(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectWPtr object) :
  QcepGraphControlsWidget(window, mode, object)
{
  setupUi(this);
}

QcepScatterPlotGraphControls::~QcepScatterPlotGraphControls()
{
}

void QcepScatterPlotGraphControls::changeEvent(QEvent *e)
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
