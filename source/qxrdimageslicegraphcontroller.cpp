#include "qxrdimageslicegraphcontroller.h"

QxrdImageSliceGraphController::QxrdImageSliceGraphController(QxrdDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object)
  : QxrdDataObjectGraphController(window, mode, object)
{

}

QxrdImageSliceGraphController::~QxrdImageSliceGraphController()
{
}

void QxrdImageSliceGraphController::activate()
{
  QxrdDataObjectGraphController::activate();
}
