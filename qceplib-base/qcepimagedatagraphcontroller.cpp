#include "qcepimagedatagraphcontroller.h"
#include <stdio.h>
#include "qcepdataobjectgraphwindow.h"
#include "qcepimagedatagraphcontrols.h"

QcepImageDataGraphController::QcepImageDataGraphController(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object)
  : QcepDataObjectGraphController(window, mode, object)
{

}

QcepImageDataGraphController::~QcepImageDataGraphController()
{
  printf("Deactivate QcepImageDataGraphController\n");

  m_Window->m_GridLayout->removeWidget(m_Widget);

  delete(m_Widget);
}

void QcepImageDataGraphController::activate()
{
  printf("Activate QcepImageDataGraphController\n");

  m_Widget = new QcepImageDataGraphControls(m_Window, m_Mode, m_Object);

  m_Window->m_GridLayout->addWidget(m_Widget,
                                    m_Window->controllerRow(),
                                    m_Window->controllerColumn());

  QcepDataObjectGraphController::activate();
}

void QcepImageDataGraphController::updateDisplay()
{
  QcepDataObjectGraphController::updateDisplay();
}
