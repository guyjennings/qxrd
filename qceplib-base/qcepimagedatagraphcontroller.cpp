#include "qcepimagedatagraphcontroller.h"
#include <stdio.h>
#include "qcepdataobjectgraphwindow.h"
#include "qcepimagedatagraphcontrols.h"

QcepImageDataGraphController::QcepImageDataGraphController(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectWPtr object)
  : QcepDataObjectGraphController(window, mode, object)
{

}

QcepImageDataGraphController::~QcepImageDataGraphController()
{
  delete(m_Widget);
}

void QcepImageDataGraphController::activate()
{
  m_Widget = new QcepImageDataGraphControls(m_Window, m_Mode, m_Object);

  m_Window->m_Splitter->addWidget(m_Widget);

  QcepDataObjectGraphController::activate();
}

void QcepImageDataGraphController::updateDisplay()
{
  QcepDataObjectGraphController::updateDisplay();
}
