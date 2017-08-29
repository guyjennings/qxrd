#include "qxrdmaskpicker.h"
#include "qxrdimageplot.h"
#include "qxrdrasterdata.h"
#include "qwt_picker_machine.h"

QxrdMaskPicker::QxrdMaskPicker(QWidget *canvas, QxrdImagePlot *plot)
  : QxrdImagePlotMeasurer(canvas, plot),
    m_Plot(plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
}

QxrdCircularMaskPicker::QxrdCircularMaskPicker(QWidget *canvas, QxrdImagePlot *plot)
  : QxrdMaskPicker(canvas, plot)
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

QxrdPolygonalMaskPicker::QxrdPolygonalMaskPicker(QWidget *canvas, QxrdImagePlot *plot)
  : QxrdMaskPicker(canvas, plot)
{
  qRegisterMetaType< QVector<QPointF> >("QVector<QPointF>");

  setStateMachine(new QwtPickerPolygonMachine());
  setRubberBand(QwtPicker::PolygonRubberBand);
}

void QxrdPolygonalMaskPicker::append(const QPoint &pt)
{
  if (m_Plot) {
    m_Plot->disableContextMenu();
  }

  QxrdMaskPicker::append(pt);
}

bool QxrdPolygonalMaskPicker::end(bool ok)
{
  if (m_Plot) {
    m_Plot->enableContextMenu();
  }

  return QxrdMaskPicker::end(ok);
}
