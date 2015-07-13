#include "qcepscatterplotgraphcontroller.h"
#include <stdio.h>
#include "qcepdataobjectgraphwindow.h"
#include "qcepscatterplotgraphcontrols.h"

QcepScatterPlotGraphController::QcepScatterPlotGraphController(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object)
  : QcepDataObjectGraphController(window, mode, object)
{

}

QcepScatterPlotGraphController::~QcepScatterPlotGraphController()
{
  delete(m_Widget);
}

void QcepScatterPlotGraphController::activate()
{
  m_Widget = new QcepScatterPlotGraphControls(m_Window, m_Mode, m_Object);

  m_Window->m_Splitter->addWidget(m_Widget);

  QcepDataObjectGraphController::activate();
}

void QcepScatterPlotGraphController::updateDisplay()
{
  printf("QcepScatterPlotGraphController::updateDisplay()\n");

  QcepDataObjectGraphController::updateDisplay();
}
