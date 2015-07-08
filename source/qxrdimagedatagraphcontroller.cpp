#include "qxrdimagedatagraphcontroller.h"
#include <stdio.h>
#include "qxrddataobjectgraphwindow.h"
#include "qxrdimagedatagraphcontrols.h"

QxrdImageDataGraphController::QxrdImageDataGraphController(QxrdDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object)
  : QxrdDataObjectGraphController(window, mode, object)
{

}

QxrdImageDataGraphController::~QxrdImageDataGraphController()
{
  printf("Deactivate QxrdImageDataGraphController\n");

  m_Window->m_GridLayout->removeWidget(m_Widget);

  delete(m_Widget);
}

void QxrdImageDataGraphController::activate()
{
  printf("Activate QxrdImageDataGraphController\n");

  m_Widget = new QxrdImageDataGraphControls(m_Window, m_Mode, m_Object);

  m_Window->m_GridLayout->addWidget(m_Widget,
                                    m_Window->controllerRow(),
                                    m_Window->controllerColumn());

  QxrdDataObjectGraphController::activate();
}

void QxrdImageDataGraphController::updateDisplay()
{
  QxrdDataObjectGraphController::updateDisplay();
}
