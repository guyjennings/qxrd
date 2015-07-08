#include "qxrdscatterplotgraphcontroller.h"
#include <stdio.h>
#include "qxrddataobjectgraphwindow.h"
#include "qxrdscatterplotgraphcontrols.h"

QxrdScatterPlotGraphController::QxrdScatterPlotGraphController(QxrdDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object)
  : QxrdDataObjectGraphController(window, mode, object)
{

}

QxrdScatterPlotGraphController::~QxrdScatterPlotGraphController()
{
  printf("Deactivate QxrdScatterPlotGraphController\n");

  m_Window->m_GridLayout->removeWidget(m_Widget);

  delete(m_Widget);
}

void QxrdScatterPlotGraphController::activate()
{
  printf("Activate QxrdScatterPlotGraphController\n");

  m_Widget = new QxrdScatterPlotGraphControls(m_Window, m_Mode, m_Object);

  m_Window->m_GridLayout->addWidget(m_Widget,
                                    m_Window->controllerRow(),
                                    m_Window->controllerColumn());

  QxrdDataObjectGraphController::activate();
}

void QxrdScatterPlotGraphController::updateDisplay()
{
  printf("QxrdScatterPlotGraphController::updateDisplay()\n");

  QxrdDataObjectGraphController::updateDisplay();
}
