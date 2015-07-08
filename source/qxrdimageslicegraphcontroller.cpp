#include "qxrdimageslicegraphcontroller.h"
#include <stdio.h>
#include "qxrddataobjectgraphwindow.h"
#include "qxrdimageslicegraphcontrols.h"

QxrdImageSliceGraphController::QxrdImageSliceGraphController(QxrdDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object)
  : QxrdDataObjectGraphController(window, mode, object)
{

}

QxrdImageSliceGraphController::~QxrdImageSliceGraphController()
{
  printf("Deactivate QxrdImageSliceGraphController\n");

  m_Window->m_GridLayout->removeWidget(m_Widget);

  delete(m_Widget);
}

void QxrdImageSliceGraphController::activate()
{
  printf("Activate QxrdImageSliceGraphController\n");

  m_Widget = new QxrdImageSliceGraphControls(m_Window, m_Mode, m_Object);

  m_Window->m_GridLayout->addWidget(m_Widget,
                                    m_Window->controllerRow(),
                                    m_Window->controllerColumn());

  QxrdDataObjectGraphController::activate();
}

void QxrdImageSliceGraphController::updateDisplay()
{
  printf("QxrdImageSliceGraphController::updateDisplay()\n");

  QxrdDataObjectGraphController::updateDisplay();
}
