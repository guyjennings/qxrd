#include "qxrdimagehistogramgraphcontroller.h"

QxrdImageHistogramGraphController::QxrdImageHistogramGraphController(QxrdDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object)
  : QxrdDataObjectGraphController(window, mode, object)
{

}

QxrdImageHistogramGraphController::~QxrdImageHistogramGraphController()
{
}

void QxrdImageHistogramGraphController::activate()
{
  QxrdDataObjectGraphController::activate();
}
