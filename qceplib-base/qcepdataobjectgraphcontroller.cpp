#include "qcepdataobjectgraphcontroller.h"

QcepDataObjectGraphController::QcepDataObjectGraphController(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object) :
  m_Window(window),
  m_Mode(mode),
  m_Object(object),
  m_Widget(NULL)
{

}

QcepDataObjectGraphController::~QcepDataObjectGraphController()
{
}

void QcepDataObjectGraphController::activate()
{
}

void QcepDataObjectGraphController::updateDisplay()
{
}
