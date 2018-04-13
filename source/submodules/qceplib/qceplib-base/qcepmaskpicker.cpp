#include "qcepmaskpicker.h"
#include "qcepimageplot.h"
#include "qceprasterdata.h"
#include "qwt_picker_machine.h"

QcepMaskPicker::QcepMaskPicker(QcepImagePlot *plot)
  : QcepImagePlotMeasurer(plot),
    m_Plot(plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
}

QcepRectangularMaskPicker::QcepRectangularMaskPicker(QcepImagePlot *plot)
  : QcepMaskPicker(plot)
{
  qRegisterMetaType<QRectF>("QRectF");

  setStateMachine(new QwtPickerDragRectMachine());
  setRubberBand(QwtPicker::RectRubberBand);
}

QcepCircularMaskPicker::QcepCircularMaskPicker(QcepImagePlot *plot)
  : QcepMaskPicker(plot)
{
  qRegisterMetaType<QRectF>("QRectF");

  setStateMachine(new QwtPickerDragRectMachine());
  setRubberBand(QwtPicker::EllipseRubberBand);
}

QPolygon QcepCircularMaskPicker::adjustedPoints(const QPolygon &points) const
{
  QPolygon adjusted;

  if ( points.size() == 2 ) {
      const int width = qAbs(points[1].x() - points[0].x());
      const int height = qAbs(points[1].y() - points[0].y());

      QRect rect(0, 0, 2 * width, 2 * height);
      rect.moveCenter(points[0]);

      adjusted += rect.topLeft();
      adjusted += rect.bottomRight();
  }

  return adjusted;
}

QcepPolygonalMaskPicker::QcepPolygonalMaskPicker(QcepImagePlot *plot)
  : QcepMaskPicker(plot)
{
  qRegisterMetaType< QVector<QPointF> >("QVector<QPointF>");

  setStateMachine(new QwtPickerPolygonMachine());
  setRubberBand(QwtPicker::PolygonRubberBand);
}
