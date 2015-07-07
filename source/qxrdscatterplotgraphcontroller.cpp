#include "qxrdscatterplotgraphcontroller.h"
#include <stdio.h>

QxrdScatterPlotGraphController::QxrdScatterPlotGraphController(QxrdDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object)
  : QxrdDataObjectGraphController(window, mode, object)
{

}

QxrdScatterPlotGraphController::~QxrdScatterPlotGraphController()
{
  printf("Deactivate QxrdScatterPlotGraphController\n");
}

void QxrdScatterPlotGraphController::activate()
{
  printf("Activate QxrdScatterPlotGraphController\n");

  QxrdDataObjectGraphController::activate();
}

