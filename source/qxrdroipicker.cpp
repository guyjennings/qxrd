#include "qxrdroipicker.h"
#include "qwt_picker_machine.h"
#include "qxrdimageplot.h"

QxrdROIPicker::QxrdROIPicker(QWidget *canvas, QxrdImagePlot *plot) :
  QxrdImagePlotMeasurer(canvas, plot)
{
//  connect(this, &QxrdROIPicker::activated, this, &QxrdROIPicker::pickerActivated);
//  connect(this, &QxrdROIPicker::appended,  this, &QxrdROIPicker::appendedPoint);

//  connect(this, (void (QcepPlotMeasurer::*)( const QRectF &)) &QxrdROIPicker::selected,
//          this, &QxrdROIPicker::selectedRect);

//  connect(this, &QxrdROIPicker::moved,
//          this, &QxrdROIPicker::movedPoint);
}

//void QxrdROIPicker::pickerActivated(bool active)
//{
//  printf("QxrdROIPicker::pickerActivated(%d)\n", active);
//}

//void QxrdROIPicker::selectedPoint(const QPointF &pt)
//{
//  printf("QxrdROIPicker::selectedPoint(%g,%g)\n", pt.x(), pt.y());
//}

//void QxrdROIPicker::selectedRect(const QRectF &r)
//{
//  printf("QxrdROIPicker::selectedRect(%g,%g,%g,%g)\n", r.left(), r.top(), r.right(), r.bottom());
//}

//void QxrdROIPicker::selectedPoly(const QVector<QPointF> &r)
//{
//  printf("QxrdROIPicker::selectedPoly(%d points)\n", r.count());
//}

//void QxrdROIPicker::appendedPoint(const QPointF &pt)
//{
//  printf("QxrdROIPicker::appendedPoint(%g,%g)\n", pt.x(), pt.y());
//}

//void QxrdROIPicker::movedPoint(const QPointF &pt)
//{
//  printf("QxrdROIPicker::movedPoint(%g,%g)\n", pt.x(), pt.y());
//}

QxrdROISelector::QxrdROISelector(QWidget *canvas, QxrdImagePlot *plot) :
  QxrdROIPicker(canvas, plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerDragLineMachine());
  setRubberBand(QwtPicker::CrossRubberBand);

  connect(this, (void (QcepPlotMeasurer::*)( const QVector<QPointF> &)) &QxrdROIPicker::selected,
          m_Plot, &QxrdImagePlot::roiMouseSelected);
}

QxrdROIAddNode::QxrdROIAddNode(QWidget *canvas, QxrdImagePlot *plot) :
  QxrdROIPicker(canvas, plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerDragLineMachine());
  setRubberBand(QwtPicker::CrossRubberBand);

  connect(this, (void (QcepPlotMeasurer::*)( const QVector<QPointF> &)) &QxrdROIPicker::selected,
          m_Plot, &QxrdImagePlot::roiMouseAdded);
}

QxrdROIRemoveNode::QxrdROIRemoveNode(QWidget *canvas, QxrdImagePlot *plot) :
  QxrdROIPicker(canvas, plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerClickPointMachine());
  setRubberBand(QwtPicker::CrossRubberBand);

  connect(this, (void (QcepPlotMeasurer::*)( const QPointF &)) &QxrdROIPicker::selected,
          m_Plot, &QxrdImagePlot::roiMouseRemoved);
}

QxrdROIRotator::QxrdROIRotator(QWidget *canvas, QxrdImagePlot *plot) :
  QxrdROIPicker(canvas, plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerDragLineMachine());
  setRubberBand(QwtPicker::CrossRubberBand);

  connect(this, (void (QcepPlotMeasurer::*)( const QVector<QPointF> &)) &QxrdROIPicker::selected,
          m_Plot, &QxrdImagePlot::roiMouseRotated);
}

QxrdROIResizer::QxrdROIResizer(QWidget *canvas, QxrdImagePlot *plot) :
  QxrdROIPicker(canvas, plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerDragLineMachine());
  setRubberBand(QwtPicker::CrossRubberBand);

  connect(this, (void (QcepPlotMeasurer::*)( const QVector<QPointF> &)) &QxrdROIPicker::selected,
          m_Plot, &QxrdImagePlot::roiMouseResized);
}
