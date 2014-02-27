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
