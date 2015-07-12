#include "qcepimagehistogramgraphcontroller.h"
#include <stdio.h>
#include "qcepdataobjectgraphwindow.h"
#include "qcepimagehistogramgraphcontrols.h"

QcepImageHistogramGraphController::QcepImageHistogramGraphController(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object)
  : QcepDataObjectGraphController(window, mode, object)
{

}

QcepImageHistogramGraphController::~QcepImageHistogramGraphController()
{
  printf("Deactivate QcepImageHistogramGraphController\n");

  m_Window->m_GridLayout->removeWidget(m_Widget);

  delete(m_Widget);
}

void QcepImageHistogramGraphController::activate()
{
  printf("Activate QcepImageHistogramGraphController\n");

  m_Widget = new QcepImageHistogramGraphControls(m_Window, m_Mode, m_Object);

  m_Window->m_GridLayout->addWidget(m_Widget,
                                    m_Window->controllerRow(),
                                    m_Window->controllerColumn());

  QcepDataObjectGraphController::activate();
}

void QcepImageHistogramGraphController::updateDisplay()
{
  printf("QcepImageHistogramGraphController::updateDisplay()\n");

  QcepDataObjectGraphController::updateDisplay();
}
