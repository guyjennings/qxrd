#include "qcepimagehistogramgraphcontroller.h"
#include <stdio.h>
#include "qcepdataobjectgraphwindow.h"
#include "qcepimagehistogramgraphcontrols.h"

QcepImageHistogramGraphController::QcepImageHistogramGraphController(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectWPtr object)
  : QcepDataObjectGraphController(window, mode, object)
{

}

QcepImageHistogramGraphController::~QcepImageHistogramGraphController()
{
  delete(m_Widget);
}

void QcepImageHistogramGraphController::activate()
{
  m_Widget = new QcepImageHistogramGraphControls(m_Window, m_Mode, m_Object);

  m_Window->m_Splitter->addWidget(m_Widget);

  QcepDataObjectGraphController::activate();
}

void QcepImageHistogramGraphController::updateDisplay()
{
  printf("QcepImageHistogramGraphController::updateDisplay()\n");

  QcepDataObjectGraphController::updateDisplay();
}
