#include "qxrdimagedatagraphcontroller.h"

QxrdImageDataGraphController::QxrdImageDataGraphController(QxrdDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object)
  : QxrdDataObjectGraphController(window, mode, object)
{

}

QxrdImageDataGraphController::~QxrdImageDataGraphController()
{
}

void QxrdImageDataGraphController::activate()
{
  QxrdDataObjectGraphController::activate();
}
