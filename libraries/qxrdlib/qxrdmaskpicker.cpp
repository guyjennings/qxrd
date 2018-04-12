#include "qxrdmaskpicker.h"
#include "qxrdimageplot.h"
#include "qceprasterdata.h"
#include "qwt_picker_machine.h"

QxrdMaskPicker::QxrdMaskPicker(QxrdImagePlot *plot)
  : QxrdImagePlotMeasurer(plot),
    m_Plot(plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
}

QxrdRectangularMaskPicker::QxrdRectangularMaskPicker(QxrdImagePlot *plot)
  : QxrdMaskPicker(plot)
{
  qRegisterMetaType<QRectF>("QRectF");

  setStateMachine(new QwtPickerDragRectMachine());
  setRubberBand(QwtPicker::RectRubberBand);
}

QxrdCircularMaskPicker::QxrdCircularMaskPicker(QxrdImagePlot *plot)
  : QxrdMaskPicker(plot)
{
  qRegisterMetaType<QRectF>("QRectF");

  setStateMachine(new QwtPickerDragRectMachine());
  setRubberBand(QwtPicker::EllipseRubberBand);
}

QPolygon QxrdCircularMaskPicker::adjustedPoints(const QPolygon &points) const
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

QxrdPolygonalMaskPicker::QxrdPolygonalMaskPicker(QxrdImagePlot *plot)
  : QxrdMaskPicker(plot)
{
  qRegisterMetaType< QVector<QPointF> >("QVector<QPointF>");

  setStateMachine(new QwtPickerPolygonMachine());
  setRubberBand(QwtPicker::PolygonRubberBand);
}
