#include "qxrdplotslicer.h"
#include "qxrdrasterdata.h"
#include "qxrdimageplot.h"
#include "qwt_picker_machine.h"

QxrdPlotSlicer::QxrdPlotSlicer(QxrdImagePlot *plot)
  : QxrdImagePlotMeasurer(plot)
{
  qRegisterMetaType< QVector<QPointF> >("QVector<QPointF>");

  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerPolygonMachine());
  setRubberBand(QwtPicker::PolygonRubberBand);
}
