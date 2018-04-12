#include "qcepplotslicer.h"
#include "qceprasterdata.h"
#include "qcepimageplot.h"
#include "qwt_picker_machine.h"

QcepPlotSlicer::QcepPlotSlicer(QcepImagePlot *plot)
  : QcepImagePlotMeasurer(plot)
{
  qRegisterMetaType< QVector<QPointF> >("QVector<QPointF>");

  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerPolygonMachine());
  setRubberBand(QwtPicker::PolygonRubberBand);
}
