#include "qxrdplotslicer.h"
#include "qceprasterdata.h"
#include "qcepimageplot.h"
#include "qwt_picker_machine.h"

QxrdPlotSlicer::QxrdPlotSlicer(QcepImagePlot *plot)
  : QxrdImagePlotMeasurer(plot)
{
  qRegisterMetaType< QVector<QPointF> >("QVector<QPointF>");

  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerPolygonMachine());
  setRubberBand(QwtPicker::PolygonRubberBand);
}
