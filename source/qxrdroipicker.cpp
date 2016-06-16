#include "qxrdroipicker.h"
#include "qwt_picker_machine.h"
#include "qxrdimageplot.h"
#include "qxrddataprocessor.h"
#include "qxrdroicoordinateslistmodel.h"
#include "qxrdroicoordinates.h"
#include "qxrdroishape.h"
#include <QMessageBox>

QxrdROIPicker::QxrdROIPicker(QWidget *canvas,
                             QxrdImagePlot *plot,
                             bool useSelectedOnly,
                             bool canSelectEdges,
                             bool usePolygonsOnly,
                             int  triggeredAppend,
                             double  startingDistance,
                             double  secondaryDistance) :
  QxrdImagePlotMeasurer(canvas, plot),
  m_UseSelectedROIsOnly(useSelectedOnly),
  m_CanSelectEdges(canSelectEdges),
  m_UsePolygonsOnly(usePolygonsOnly),
  m_TriggeredAppend(triggeredAppend),
  m_StartingDistance(startingDistance),
  m_SecondaryDistance(secondaryDistance),
  m_SelectedROI(-1),
  m_SelectedShape(-1),
  m_SelectedPoint(-1),
  m_SelectedEdge(-1),
  m_SelectedPoints(),
  m_RubberBand()
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

void QxrdROIPicker::drawRubberBand(QPainter *painter) const
{
  if ( !isActive() || rubberBand() == NoRubberBand ||
      rubberBandPen().style() == Qt::NoPen )
  {
      return;
  }

  painter->drawPolyline(m_RubberBand);
}

QRegion QxrdROIPicker::rubberBandMask() const
{
  QRegion mask;

  if ( !isActive() || rubberBand() == NoRubberBand ||
      rubberBandPen().style() == Qt::NoPen )
  {
      return mask;
  }

  const int pw = rubberBandPen().width();
  const int off = 2*pw;

  const QRect r = m_RubberBand.boundingRect();

  mask += r.adjusted(-off, -off, off, off);

  return mask;
}

void QxrdROIPicker::begin()
{
//  printMessage("QxrdROIPicker::begin");

  m_SelectedPoints = QPolygon();
  m_SelectedROI    = -1;
  m_SelectedShape  = -1; // Inner or outer
  m_SelectedPoint  = -1; // Point number if a vertex
  m_SelectedEdge   = -1; // Edge number if an edge

  QwtPicker::begin();
}

void QxrdROIPicker::append(const QPoint &pt)
{
//  printMessage(tr("QxrdROIPicker::append(%1,%2)").arg(pt.x()).arg(pt.y()));

  QxrdImagePlot *imgPlot = imagePlot();

  if (imgPlot) {
    QxrdROICoordinatesListModelPtr roiModel  = imgPlot->roiModel();
    QItemSelectionModel           *roiSelect = imgPlot->roiSelection();

    if (roiModel && roiSelect) {
      int nRois = roiModel->rowCount(QModelIndex());
      int nSel  = 0;
      int nPolys = 0;
      double minDistance = m_StartingDistance;
      int    minDistROI  = -1;     // The ROI with the minimum distance
      int    minDistROIShape = -1; // Inner or outer...
      int    minDistROIPoint = -1; // Which point...
      int    minDistROIEdge  = -1; // Which edge...

      if (m_SelectedPoints.count() == m_TriggeredAppend) { // First point is added twice - trigger on second append...
        for (int i=0; i<nRois; i++) {
          if ((!m_UseSelectedROIsOnly) || roiSelect->rowIntersectsSelection(i, QModelIndex())) {
            nSel += 1;
            QxrdROICoordinatesPtr roi = roiModel->roi(i);

            if (roi) {
              if ((!m_UsePolygonsOnly) || roi->get_RoiInnerType() == QxrdROIShape::PolygonShape) {
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

              if ((!m_UsePolygonsOnly) || roi->get_RoiOuterType() == QxrdROIShape::PolygonShape) {
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
            }
          }
        }

        if (m_CanSelectEdges && (minDistance > m_SecondaryDistance)) { // Now look at polygon edges...
          for (int i=0; i<nRois; i++) {
            if ((!m_UseSelectedROIsOnly) || roiSelect->rowIntersectsSelection(i, QModelIndex())) {
              nSel += 1;
              QxrdROICoordinatesPtr roi = roiModel->roi(i);

              if (roi) {
                if ((!m_UsePolygonsOnly) || roi->get_RoiInnerType() == QxrdROIShape::PolygonShape) {
                  nPolys += 1;
                  QxrdROIShapePtr innerShape = roi->inner();

                  if (innerShape) {
                    for (int j=1; j<innerShape->markerCount(); j++) {
                      QLineF  lnf(roi->transform(innerShape->markerPoint(j-1)),
                                  roi->transform(innerShape->markerPoint(j)));

                      for (double t=0.1; t<=0.9; t += 0.1) {
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

                if ((!m_UsePolygonsOnly) || roi->get_RoiOuterType() == QxrdROIShape::PolygonShape) {
                  nPolys += 1;
                  QxrdROIShapePtr outerShape = roi->outer();

                  if (outerShape) {
                    for (int j=1; j<outerShape->markerCount(); j++) {
                      QLineF lnf(roi->transform(outerShape->markerPoint(j-1)),
                                 roi->transform(outerShape->markerPoint(j)));

                      for (double t=0.1; t<=0.9; t += 0.1) {
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

        if (minDistance > m_SecondaryDistance*2.0) {
          if (m_UseSelectedROIsOnly && nSel == 0) {
            QMessageBox::warning(imgPlot, "No Selected ROIs", "No ROIs were selected");
          } else if (nPolys == 0) {
            QMessageBox::warning(imgPlot, "No Polygonal ROIs", "No ROI had a polygon shape");
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

          m_SelectedROI   = minDistROI;
          m_SelectedShape = minDistROIShape;
          m_SelectedPoint = minDistROIPoint;
          m_SelectedEdge  = minDistROIEdge;
        }
      } else {
      }
    }

    m_SelectedPoints.append(pt);
  }

  QwtPlotPicker::append(pt);
}

void QxrdROIPicker::move(const QPoint &pt)
{
//  printMessage(tr("QxrdROIPicker::move(%1,%2)").arg(pt.x()).arg(pt.y()));

  int idx = m_SelectedPoints.count() - 1;

  if (idx >= 0) {
    m_SelectedPoints[idx] = pt;
  }

  QwtPlotPicker::move(pt);
}

bool QxrdROIPicker::end(bool ok)
{
//  printMessage(tr("QxrdROIPicker::end(%1)").arg(ok));

//  for (int i=0; i<m_SelectedPoints.count(); i++) {
//    QPoint p = m_SelectedPoints[i];

//    printMessage(tr(" %1: [%2,%3]").arg(i).arg(p.x()).arg(p.y()));
//  }

  return QwtPlotPicker::end(ok);
}

QxrdROISelector::QxrdROISelector(QWidget *canvas, QxrdImagePlot *plot) :
  QxrdROIPicker(canvas, plot,
                UseAllROIs,
                CanSelectEdges,
                UseAnyShape,
                SecondAppend,
                qInf(),
                10.0)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerDragLineMachine());
  setRubberBand(QwtPicker::PolygonRubberBand);

//  connect(this, (void (QcepPlotMeasurer::*)( const QVector<QPointF> &)) &QxrdROIPicker::selected,
//          m_Plot, &QxrdImagePlot::roiMouseSelected);
}

void QxrdROISelector::move(const QPoint &pt)
{
  QxrdROIPicker::move(pt);

  QxrdImagePlot* imgPlot = imagePlot();

  if (imgPlot) {
    QxrdROICoordinatesListModelPtr roiMod = imgPlot->roiModel();
    QItemSelectionModel           *selMod = imgPlot->roiSelection();

    if (roiMod &&
        selMod &&
        m_SelectedROI >= 0 &&
        m_SelectedPoints.count() == 2) {
      QxrdROICoordinatesPtr roi = roiMod->roi(m_SelectedROI);

      if (roi) {
        QPointF del = invTransform(m_SelectedPoints.value(1))
            - invTransform(m_SelectedPoints.value(0));

        QPolygonF fb = roi->dragOutline();

        fb.translate(del);

        m_RubberBand = QPolygon();

        for (int i=0; i<fb.count(); i++) {
          m_RubberBand.append(transform(fb.value(i)));
        }
      }
    }
  }
}

bool QxrdROISelector::end(bool ok)
{
  if (ok) {
    printMessage(tr("Select ROI %1").arg(m_SelectedROI));

    QxrdImagePlot* imgPlot = imagePlot();

    if (imgPlot) {
      QxrdROICoordinatesListModelPtr roi = imgPlot->roiModel();
      QItemSelectionModel           *sel = imgPlot->roiSelection();

      if (roi && sel && m_SelectedPoints.count() == 2) {
        QPointF del = invTransform(m_SelectedPoints.value(1))
            - invTransform(m_SelectedPoints.value(0));

        if ((del.x() == 0) && (del.y() == 0)) {
          sel->select(roi->index(m_SelectedROI, 0),
                      QItemSelectionModel::Toggle | QItemSelectionModel::Rows);
        } else {
          int n = roi->roiCount();

          for (int i=0; i<n; i++) {
            if (sel->rowIntersectsSelection(i, QModelIndex())) {
              roi->moveROIRelative(i, del.x(), del.y());
            }
          }
        }
      }
    }
  }

  return QxrdROIPicker::end(ok);
}

QxrdROIAddNode::QxrdROIAddNode(QWidget *canvas, QxrdImagePlot *plot) :
  QxrdROIPicker(canvas, plot,
                UseSelectedROIs,
                CanSelectEdges,
                UsePolygonsOnly,
                SecondAppend,
                100.0,
                10.0)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerDragLineMachine());
  setRubberBand(QwtPicker::PolygonRubberBand);

//  connect(this, (void (QcepPlotMeasurer::*)( const QVector<QPointF> &)) &QxrdROIPicker::selected,
//          m_Plot, &QxrdImagePlot::roiMouseAdded);
}

void QxrdROIAddNode::move(const QPoint &pt)
{
  QxrdROIPicker::move(pt);
}

bool QxrdROIAddNode::end(bool ok)
{
  if (ok) {
    printMessage(tr("Add node to ROI %1:%2").arg(m_SelectedROI).arg(m_SelectedShape));
  }

  return QxrdROIPicker::end(ok);
}

QxrdROIRemoveNode::QxrdROIRemoveNode(QWidget *canvas, QxrdImagePlot *plot) :
  QxrdROIPicker(canvas, plot,
                UseSelectedROIs,
                SelectPointsOnly,
                UsePolygonsOnly,
                FirstAppend,
                100.0,
                10.0)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerClickPointMachine());
  setRubberBand(QwtPicker::CrossRubberBand);

//  connect(this, (void (QcepPlotMeasurer::*)( const QPointF &)) &QxrdROIPicker::selected,
//          m_Plot, &QxrdImagePlot::roiMouseRemoved);
}

bool QxrdROIRemoveNode::end(bool ok)
{
  if (ok) {
    printMessage(tr("Remove node %1 of ROI %2").arg(m_SelectedPoint).arg(m_SelectedROI));
  }

  return QxrdROIPicker::end(ok);
}

QxrdROIRotator::QxrdROIRotator(QWidget *canvas, QxrdImagePlot *plot) :
  QxrdROIPicker(canvas, plot,
                UseSelectedROIs,
                SelectPointsOnly,
                UseAnyShape,
                SecondAppend,
                qInf(),
                10.0)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerDragLineMachine());
  setRubberBand(QwtPicker::PolygonRubberBand);

//  connect(this, (void (QcepPlotMeasurer::*)( const QVector<QPointF> &)) &QxrdROIPicker::selected,
//          m_Plot, &QxrdImagePlot::roiMouseRotated);
}

void QxrdROIRotator::move(const QPoint &pt)
{
  QxrdROIPicker::move(pt);

  QxrdImagePlot* imgPlot = imagePlot();

  if (imgPlot) {
    QxrdROICoordinatesListModelPtr roiMod = imgPlot->roiModel();
    QItemSelectionModel           *selMod = imgPlot->roiSelection();

    if (roiMod &&
        selMod &&
        m_SelectedROI >= 0 &&
        m_SelectedPoints.count() == 2) {
      QxrdROICoordinatesPtr roi = roiMod->roi(m_SelectedROI);

      if (roi) {
        QPointF ptf1 = invTransform(m_SelectedPoints.value(0));
        QPointF ptf2 = invTransform(m_SelectedPoints.value(1));
        QPointF ptf0 = roi->get_Center();

        QLineF l1(ptf0, ptf1), l2(ptf0, ptf2);
        double a1 = l1.angle();
        double a2 = l2.angle();
        double da = a1 - a2;
        double na = roi->get_Rotation() + da;

        while (na > 180) {
          na -= 360;
        }

        while (na < -180) {
          na += 360;
        }

        QPolygonF fb = roi->rotatedDragOutline(na);

        m_RubberBand = QPolygon();

        for (int i=0; i<fb.count(); i++) {
          m_RubberBand.append(transform(fb.value(i)));
        }
      }
    }
  }
}

bool QxrdROIRotator::end(bool ok)
{
  if (ok) {
    printMessage(tr("Rotate ROI %1").arg(m_SelectedROI));

    QxrdImagePlot* imgPlot = imagePlot();

    if (imgPlot) {
      QxrdROICoordinatesListModelPtr roiMod = imgPlot->roiModel();
      QItemSelectionModel           *selMod = imgPlot->roiSelection();

      if (roiMod &&
          selMod &&
          m_SelectedROI >= 0 &&
          m_SelectedPoints.count() == 2) {
        QxrdROICoordinatesPtr roi = roiMod->roi(m_SelectedROI);

        if (roi) {
          QPointF ptf1 = invTransform(m_SelectedPoints.value(0));
          QPointF ptf2 = invTransform(m_SelectedPoints.value(1));
          QPointF ptf0 = roi->get_Center();

          QLineF l1(ptf0, ptf1), l2(ptf0, ptf2);
          double a1 = l1.angle();
          double a2 = l2.angle();
          double da = a1 - a2;
          double na = roi->get_Rotation() + da;

          while (na > 180) {
            na -= 360;
          }

          while (na < -180) {
            na += 360;
          }

          roiMod->setRotation(m_SelectedROI, na);
        }
      }
    }
  }

  return QxrdROIPicker::end(ok);
}

QxrdROIResizer::QxrdROIResizer(QWidget *canvas, QxrdImagePlot *plot) :
  QxrdROIPicker(canvas, plot,
                UseSelectedROIs,
                SelectPointsOnly,
                UseAnyShape,
                SecondAppend,
                qInf(),
                10.0)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerDragLineMachine());
  setRubberBand(QwtPicker::PolygonRubberBand);

//  connect(this, (void (QcepPlotMeasurer::*)( const QVector<QPointF> &)) &QxrdROIPicker::selected,
//          m_Plot, &QxrdImagePlot::roiMouseResized);
}

void QxrdROIResizer::move(const QPoint &pt)
{
  QxrdROIPicker::move(pt);
}

bool QxrdROIResizer::end(bool ok)
{
  if (ok) {
    printMessage(tr("Resize ROI %1").arg(m_SelectedROI));
  }

  return QxrdROIPicker::end(ok);
}
