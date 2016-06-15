#include "qxrdroipicker.h"
#include "qwt_picker_machine.h"
#include "qxrdimageplot.h"
#include "qxrddataprocessor.h"
#include "qxrdroicoordinateslistmodel.h"
#include "qxrdroicoordinates.h"
#include "qxrdroishape.h"
#include <QMessageBox>

QxrdROIPicker::QxrdROIPicker(QWidget *canvas, QxrdImagePlot *plot) :
  QxrdImagePlotMeasurer(canvas, plot)
{
  setEnabled(false);
}

void QxrdROIPicker::printMessage(QString msg, QDateTime dt) const
{
  const QxrdImagePlot* imgPlot = imagePlot();

  if (imgPlot) {
    imgPlot->printMessage(msg, dt);
  }
}

QxrdROISelector::QxrdROISelector(QWidget *canvas, QxrdImagePlot *plot) :
  QxrdROIPicker(canvas, plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerDragLineMachine());
  setRubberBand(QwtPicker::PolygonRubberBand);

  connect(this, (void (QcepPlotMeasurer::*)( const QVector<QPointF> &)) &QxrdROIPicker::selected,
          m_Plot, &QxrdImagePlot::roiMouseSelected);
}

QxrdROIAddNode::QxrdROIAddNode(QWidget *canvas, QxrdImagePlot *plot) :
  QxrdROIPicker(canvas, plot),
  m_SelectedPoints()
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerDragLineMachine());
  setRubberBand(QwtPicker::PolygonRubberBand);

  connect(this, (void (QcepPlotMeasurer::*)( const QVector<QPointF> &)) &QxrdROIPicker::selected,
          m_Plot, &QxrdImagePlot::roiMouseAdded);
}

QPolygon QxrdROIAddNode::adjustedPoints(const QPolygon &poly) const
{
//  printMessage("QxrdROIAddNode::adjustedPoints(");

//  for (int i=0; i<poly.count(); i++) {
//    QPoint pt = poly.value(i);
//    printMessage(tr(" %1: [%2,%3]").arg(i).arg(pt.x()).arg(pt.y()));
//  }

//  printMessage(")");

  return QxrdROIPicker::adjustedPoints(poly);
}

void QxrdROIAddNode::begin()
{
//  printMessage("QxrdROIAddNode::begin");

  m_SelectedPoints = QPolygon();
  m_SelectedROI    = -1;
  m_SelectedShape  = -1; // Inner or outer
  m_SelectedPoint  = -1; // Point number if a vertex
  m_SelectedEdge   = -1; // Edge number if an edge

  QwtPicker::begin();
}

void QxrdROIAddNode::append(const QPoint &pt)
{
//  printMessage(tr("QxrdROIAddNode::append(%1,%2)").arg(pt.x()).arg(pt.y()));

  QxrdImagePlot *imgPlot = imagePlot();

  if (imgPlot) {
    QxrdROICoordinatesListModelPtr roiModel  = imgPlot->roiModel();
    QItemSelectionModel           *roiSelect = imgPlot->roiSelection();

    if (roiModel && roiSelect) {
      int nRois = roiModel->rowCount(QModelIndex());
      int nSel  = 0;
      int nPolys = 0;
      double minDistance = 100;
      int    minDistROI  = -1;     // The ROI with the minimum distance
      int    minDistROIShape = -1; // Inner or outer...
      int    minDistROIPoint = -1; // Which point...
      int    minDistROIEdge  = -1; // Which edge...

      if (m_SelectedPoints.count() == 1) { // First point is added twice - trigger on second append...
        for (int i=0; i<nRois; i++) {
          if (roiSelect->rowIntersectsSelection(i, QModelIndex())) {
            nSel += 1;
            QxrdROICoordinatesPtr roi = roiModel->roi(i);

            if (roi) {
              if (roi->get_RoiInnerType() == QxrdROIShape::PolygonShape) {
                nPolys += 1;
                QxrdROIShapePtr innerShape = roi->inner();

                if (innerShape) {
                  for (int j=0; j<innerShape->markerCount(); j++) {
                    QPointF ptf = roi->transform(innerShape->markerPoint(j));
                    QPoint  ptfi = transform(ptf);
                    int     dx   = ptfi.x() - pt.x();
                    int     dy   = ptfi.y() - pt.y();

                    double  dist = sqrt(dx*dx + dy*dy);

                    if (dist < minDistance) {
                      minDistance = dist;
                      minDistROI  = i;
                      minDistROIShape = QxrdROICoordinates::InnerShape;
                      minDistROIPoint = j;
                    }
                  }
                }
              }

              if (roi->get_RoiOuterType() == QxrdROIShape::PolygonShape) {
                nPolys += 1;
                QxrdROIShapePtr outerShape = roi->outer();

                if (outerShape) {
                  for (int j=0; j<outerShape->markerCount(); j++) {
                    QPointF ptf = roi->transform(outerShape->markerPoint(j));
                    QPoint  ptfi = transform(ptf);
                    int     dx   = ptfi.x() - pt.x();
                    int     dy   = ptfi.y() - pt.y();

                    double  dist = sqrt(dx*dx + dy*dy);

                    if (dist < minDistance) {
                      minDistance = dist;
                      minDistROI  = i;
                      minDistROIShape = QxrdROICoordinates::OuterShape;
                      minDistROIPoint = j;
                    }
                  }
                }
              }

              if (minDistance > 10) { // Now look at polygon edges...
                if (roi) {
                  if (roi->get_RoiInnerType() == QxrdROIShape::PolygonShape) {
                    nPolys += 1;
                    QxrdROIShapePtr innerShape = roi->inner();

                    if (innerShape) {
                      for (int j=1; j<innerShape->markerCount(); j++) {
                        QLineF  lnf(roi->transform(innerShape->markerPoint(j-1)),
                                    roi->transform(innerShape->markerPoint(j)));

                        for (double t=0.1; t<1; t += 0.1) {
                          QPointF ptf = lnf.pointAt(t);
                          QPoint  ptfi = transform(ptf);
                          int     dx   = ptfi.x() - pt.x();
                          int     dy   = ptfi.y() - pt.y();

                          double  dist = sqrt(dx*dx + dy*dy);

                          if (dist < minDistance) {
                            minDistance = dist;
                            minDistROI  = i;
                            minDistROIShape = QxrdROICoordinates::InnerShape;
                            minDistROIEdge = j-1;
                          }
                        }
                      }
                    }
                  }

                  if (roi->get_RoiOuterType() == QxrdROIShape::PolygonShape) {
                    nPolys += 1;
                    QxrdROIShapePtr outerShape = roi->outer();

                    if (outerShape) {
                      for (int j=1; j<outerShape->markerCount(); j++) {
                        QLineF lnf(roi->transform(outerShape->markerPoint(j-1)),
                                   roi->transform(outerShape->markerPoint(j)));

                        for (double t=0.1; t<1; t += 0.1) {
                          QPointF ptf = lnf.pointAt(t);
                          QPoint  ptfi = transform(ptf);
                          int     dx   = ptfi.x() - pt.x();
                          int     dy   = ptfi.y() - pt.y();

                          double  dist = sqrt(dx*dx + dy*dy);

                          if (dist < minDistance) {
                            minDistance = dist;
                            minDistROI  = i;
                            minDistROIShape = QxrdROICoordinates::OuterShape;
                            minDistROIEdge = j-1;
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }

        if (minDistance > 20) {
          if (nSel == 0) {
            QMessageBox::warning(imgPlot, "No Selected ROIs", "No ROIs were selected");
          } else if (nPolys == 0) {
            QMessageBox::warning(imgPlot, "No Polygonal ROIs", "No selected ROI had a polygon shape");
          } else {
            QMessageBox::warning(imgPlot, "Not Close", "No ROI corners near click point");
          }

          reset();
          return;
        } else {
          printMessage(tr("Selected ROI %1:%2, pt %3, edge %4, dist %5")
                       .arg(minDistROI).arg(minDistROIShape)
                       .arg(minDistROIPoint).arg(minDistROIEdge)
                       .arg(minDistance));
        }
      } else {
      }
    }

    m_SelectedPoints.append(pt);
  }

  QwtPlotPicker::append(pt);
}

void QxrdROIAddNode::move(const QPoint &pt)
{
//  printMessage(tr("QxrdROIAddNode::move(%1,%2)").arg(pt.x()).arg(pt.y()));

  int idx = m_SelectedPoints.count() - 1;

  if (idx >= 0) {
    m_SelectedPoints[idx] = pt;
  }

  QwtPlotPicker::move(pt);
}

bool QxrdROIAddNode::end(bool ok)
{
//  printMessage(tr("QxrdROIAddNode::end(%1)").arg(ok));

//  for (int i=0; i<m_SelectedPoints.count(); i++) {
//    QPoint p = m_SelectedPoints[i];

//    printMessage(tr(" %1: [%2,%3]").arg(i).arg(p.x()).arg(p.y()));
//  }

  return QwtPlotPicker::end(ok);
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
  setRubberBand(QwtPicker::PolygonRubberBand);

  connect(this, (void (QcepPlotMeasurer::*)( const QVector<QPointF> &)) &QxrdROIPicker::selected,
          m_Plot, &QxrdImagePlot::roiMouseRotated);
}

QxrdROIResizer::QxrdROIResizer(QWidget *canvas, QxrdImagePlot *plot) :
  QxrdROIPicker(canvas, plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerDragLineMachine());
  setRubberBand(QwtPicker::PolygonRubberBand);

  connect(this, (void (QcepPlotMeasurer::*)( const QVector<QPointF> &)) &QxrdROIPicker::selected,
          m_Plot, &QxrdImagePlot::roiMouseResized);
}
