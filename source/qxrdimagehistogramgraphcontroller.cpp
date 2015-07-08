#include "qxrdimagehistogramgraphcontroller.h"
#include <stdio.h>
#include "qxrddataobjectgraphwindow.h"
#include "qxrdimagehistogramgraphcontrols.h"

QxrdImageHistogramGraphController::QxrdImageHistogramGraphController(QxrdDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object)
  : QxrdDataObjectGraphController(window, mode, object)
{

}

QxrdImageHistogramGraphController::~QxrdImageHistogramGraphController()
{
  printf("Deactivate QxrdImageHistogramGraphController\n");

  m_Window->m_GridLayout->removeWidget(m_Widget);

  delete(m_Widget);
}

void QxrdImageHistogramGraphController::activate()
{
  printf("Activate QxrdImageHistogramGraphController\n");

  m_Widget = new QxrdImageHistogramGraphControls(m_Window, m_Mode, m_Object);

  m_Window->m_GridLayout->addWidget(m_Widget,
                                    m_Window->controllerRow(),
                                    m_Window->controllerColumn());

  QxrdDataObjectGraphController::activate();
}

void QxrdImageHistogramGraphController::updateDisplay()
{
  printf("QxrdImageHistogramGraphController::updateDisplay()\n");

  QxrdDataObjectGraphController::updateDisplay();
}
