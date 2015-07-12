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
  printf("Deactivate QcepScatterPlotGraphController\n");

  m_Window->m_GridLayout->removeWidget(m_Widget);

  delete(m_Widget);
}

void QcepScatterPlotGraphController::activate()
{
  printf("Activate QcepScatterPlotGraphController\n");

  m_Widget = new QcepScatterPlotGraphControls(m_Window, m_Mode, m_Object);

  m_Window->m_GridLayout->addWidget(m_Widget,
                                    m_Window->controllerRow(),
                                    m_Window->controllerColumn());

  QcepDataObjectGraphController::activate();
}

void QcepScatterPlotGraphController::updateDisplay()
{
  printf("QcepScatterPlotGraphController::updateDisplay()\n");

  QcepDataObjectGraphController::updateDisplay();
}
