#include "qxrdimageslicegraphcontroller.h"
#include <stdio.h>

QxrdImageSliceGraphController::QxrdImageSliceGraphController(QxrdDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object)
  : QxrdDataObjectGraphController(window, mode, object)
{

}

QxrdImageSliceGraphController::~QxrdImageSliceGraphController()
{
  printf("Deactivate QxrdImageSliceGraphController\n");
}

void QxrdImageSliceGraphController::activate()
{
  printf("Activate QxrdImageSliceGraphController\n");

  QxrdDataObjectGraphController::activate();
}
