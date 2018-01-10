#include "qxrdplotslicer.h"
#include "qxrdrasterdata.h"
#include "qxrdimageplot.h"
#include "qwt_picker_machine.h"

QxrdPlotSlicer::QxrdPlotSlicer(QWidget *canvas, QxrdImagePlot *plot)
  : QxrdImagePlotMeasurer(canvas, plot)/*,
    m_Plot(plot)*/
{
  qRegisterMetaType< QVector<QPointF> >("QVector<QPointF>");

  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerPolygonMachine());
  setRubberBand(QwtPicker::PolygonRubberBand);
}
