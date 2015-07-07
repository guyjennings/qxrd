#include "qxrddataobjectgraphcontroller.h"

QxrdDataObjectGraphController::QxrdDataObjectGraphController(QxrdDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object) :
  m_Window(window),
  m_Mode(mode),
  m_Object(object)
{

}

QxrdDataObjectGraphController::~QxrdDataObjectGraphController()
{
}

void QxrdDataObjectGraphController::activate()
{
}
