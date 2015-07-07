#include "qxrdimagehistogramgraphcontroller.h"
#include <stdio.h>

QxrdImageHistogramGraphController::QxrdImageHistogramGraphController(QxrdDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object)
  : QxrdDataObjectGraphController(window, mode, object)
{

}

QxrdImageHistogramGraphController::~QxrdImageHistogramGraphController()
{
  printf("Deactivate QxrdImageHistogramGraphController\n");
}

void QxrdImageHistogramGraphController::activate()
{
  printf("Activate QxrdImageHistogramGraphController\n");

  QxrdDataObjectGraphController::activate();
}
