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
  delete(m_Widget);
}

void QcepImageSliceGraphController::activate()
{
  m_Widget = new QcepImageSliceGraphControls(m_Window, m_Mode, m_Object);

  m_Window->m_Splitter->addWidget(m_Widget);

  QcepDataObjectGraphController::activate();
}

void QcepImageSliceGraphController::updateDisplay()
{
  QcepDataObjectGraphController::updateDisplay();
}
