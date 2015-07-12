#include "qcepimageslicegraphcontroller.h"
#include <stdio.h>
#include "qcepdataobjectgraphwindow.h"
#include "qcepimageslicegraphcontrols.h"

QcepImageSliceGraphController::QcepImageSliceGraphController(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object)
  : QcepDataObjectGraphController(window, mode, object)
{

}

QcepImageSliceGraphController::~QcepImageSliceGraphController()
{
  printf("Deactivate QcepImageSliceGraphController\n");

  m_Window->m_GridLayout->removeWidget(m_Widget);

  delete(m_Widget);
}

void QcepImageSliceGraphController::activate()
{
  printf("Activate QcepImageSliceGraphController\n");

  m_Widget = new QcepImageSliceGraphControls(m_Window, m_Mode, m_Object);

  m_Window->m_GridLayout->addWidget(m_Widget,
                                    m_Window->controllerRow(),
                                    m_Window->controllerColumn());

  QcepDataObjectGraphController::activate();
}

void QcepImageSliceGraphController::updateDisplay()
{
  printf("QcepImageSliceGraphController::updateDisplay()\n");

  QcepDataObjectGraphController::updateDisplay();
}
