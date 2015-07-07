#include "qxrdimagedatagraphcontroller.h"
#include <stdio.h>

QxrdImageDataGraphController::QxrdImageDataGraphController(QxrdDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object)
  : QxrdDataObjectGraphController(window, mode, object)
{

}

QxrdImageDataGraphController::~QxrdImageDataGraphController()
{
  printf("Deactivate QxrdImageDataGraphController\n");
}

void QxrdImageDataGraphController::activate()
{
  printf("Activate QxrdImageDataGraphController\n");

  QxrdDataObjectGraphController::activate();
}
