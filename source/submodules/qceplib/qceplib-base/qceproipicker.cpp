#include "qceproipicker.h"
#include "qwt_picker_machine.h"
#include "qcepimageplot.h"
#include "qceproimodel.h"
#include "qceproi.h"
#include "qceproishape.h"
#include <QMessageBox>
#include "qcepdebug.h"

QcepROIPicker::QcepROIPicker(QcepImagePlot *plot,
                             bool useSelectedOnly,
                             bool canSelectEdges,
                             bool usePolygonsOnly,
                             int  triggeredAppend,
                             double  startingDistance,
                             double  secondaryDistance) :
  QcepImagePlotMeasurer(plot),
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

  setTrackerPen(QPen(Qt::red));
  setRubberBandPen(QPen(Qt::red));
}

void QcepROIPicker::printMessage(QString msg, QDateTime dt) const
{
  const QcepImagePlot* imgPlot = imagePlot();

  if (imgPlot) {
    imgPlot->printMessage(msg, dt);
  }
}

void QcepROIPicker::drawRubberBand(QPainter *painter) const
{
  if ( !isActive() || rubberBand() == NoRubberBand ||
      rubberBandPen().style() == Qt::NoPen )
  {
      return;
  }

  painter->drawPolyline(m_RubberBand);
}

QRegion QcepROIPicker::rubberBandMask() const
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

void QcepROIPicker::begin()
{
//  printMessage("QcepROIPicker::begin");

  m_SelectedPoints = QPolygon();
  m_SelectedROI    = -1;
  m_SelectedShape  = -1; // Inner or outer
  m_SelectedPoint  = -1; // Point number if a vertex
  m_SelectedEdge   = -1; // Edge number if an edge

  QwtPicker::begin();
}

void QcepROIPicker::append(const QPoint &pt)
{
//  printMessage(tr("QcepROIPicker::append(%1,%2)").arg(pt.x()).arg(pt.y()));

  QcepImagePlot *imgPlot = imagePlot();

  if (imgPlot) {
    QcepROIModelPtr      roiModel  = imgPlot->roiModel();
    QItemSelectionModel *roiSelect = imgPlot->roiSelection();

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
            QcepROIPtr roi = roiModel->roi(i);

            if (roi) {
              if ((!m_UsePolygonsOnly) || roi->get_RoiInnerType() == QcepROIShape::PolygonShape) {
                nPolys += 1;
                QcepROIShapePtr innerShape = roi->inner();

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
                      minDistROIShape = QcepROI::InnerShape;
                      minDistROIPoint = j;
                    }
                  }
                }
              }

              if ((!m_UsePolygonsOnly) || roi->get_RoiOuterType() == QcepROIShape::PolygonShape) {
                nPolys += 1;
                QcepROIShapePtr outerShape = roi->outer();

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
                      minDistROIShape = QcepROI::OuterShape;
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
              QcepROIPtr roi = roiModel->roi(i);

              if (roi) {
                if ((!m_UsePolygonsOnly) || roi->get_RoiInnerType() == QcepROIShape::PolygonShape) {
                  nPolys += 1;
                  QcepROIShapePtr innerShape = roi->inner();

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
                          minDistROIShape = QcepROI::InnerShape;
                          minDistROIEdge = j-1;
                        }
                      }
                    }
                  }
                }

                if ((!m_UsePolygonsOnly) || roi->get_RoiOuterType() == QcepROIShape::PolygonShape) {
                  nPolys += 1;
                  QcepROIShapePtr outerShape = roi->outer();

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
                          minDistROIShape = QcepROI::OuterShape;
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
          if (qcepDebug(DEBUG_ROI)) {
            printMessage(tr("Selected ROI %1:%2, pt %3, edge %4, dist %5")
                         .arg(minDistROI).arg(minDistROIShape)
                         .arg(minDistROIPoint).arg(minDistROIEdge)
                         .arg(minDistance));
          }

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

void QcepROIPicker::move(const QPoint &pt)
{
//  printMessage(tr("QcepROIPicker::move(%1,%2)").arg(pt.x()).arg(pt.y()));

  int idx = m_SelectedPoints.count() - 1;

  if (idx >= 0) {
    m_SelectedPoints[idx] = pt;
  }

  QwtPlotPicker::move(pt);
}

bool QcepROIPicker::end(bool ok)
{
//  printMessage(tr("QcepROIPicker::end(%1)").arg(ok));

//  for (int i=0; i<m_SelectedPoints.count(); i++) {
//    QPoint p = m_SelectedPoints[i];

//    printMessage(tr(" %1: [%2,%3]").arg(i).arg(p.x()).arg(p.y()));
//  }

  return QwtPlotPicker::end(ok);
}

QcepROICreator::QcepROICreator(QcepImagePlot *plot) :
  QcepROIPicker(plot,
                UseAllROIs,
                CanSelectEdges,
                UseAnyShape,
                SecondAppend,
                qInf(),
                10.0),
  m_CreatedType(-1)
{
  setCreatedType(QcepROI::NewRectROI);
  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerDragLineMachine());
  setRubberBand(QwtPicker::PolygonRubberBand);
}

void QcepROICreator::setCreatedType(int newType)
{
  m_CreatedType = newType;
  m_CreatedTypeID = QcepROI::roiTypeID(QcepROI::NoBounds,
                                                  QcepROI::NoBounds);

  switch (newType) {
  case QcepROI::NewRectROI:
    m_CreatedTypeID = QcepROI::roiTypeID(QcepROI::NoBounds,
                                                    QcepROI::RectangleBounds);
    break;
  case QcepROI::NewEllipseROI:
    m_CreatedTypeID = QcepROI::roiTypeID(QcepROI::NoBounds,
                                                    QcepROI::EllipseBounds);
    break;
  case QcepROI::NewRectDonutROI:
    m_CreatedTypeID = QcepROI::roiTypeID(QcepROI::RectangleBounds,
                                                    QcepROI::RectangleBounds);
    break;
  case QcepROI::NewEllipseDonutROI:
    m_CreatedTypeID = QcepROI::roiTypeID(QcepROI::EllipseBounds,
                                                    QcepROI::EllipseBounds);
    break;
  }
}

void QcepROICreator::begin()
{
  QcepROIPicker::begin();

  m_NewROI = QcepROI::newROICoordinates(m_CreatedTypeID);
}

void QcepROICreator::move(const QPoint &pt)
{
  QcepROIPicker::move(pt);

  QcepImagePlot *imgPlot = imagePlot();

  if (imgPlot) {
    if (m_SelectedPoints.count() == 2) {
      QPointF p1 = invTransform(m_SelectedPoints.value(0));
      QPointF p2 = invTransform(m_SelectedPoints.value(1));

      double sx = qAbs(p2.x() - p1.x())/12.5;
      double sy = qAbs(p2.y() - p1.y())/12.5;

      m_NewROI->setCenter(p1);


//      QPointF ptf1 = invTransform(m_SelectedPoints.value(0));
//      QPointF ptf2 = invTransform(m_SelectedPoints.value(1));

//      QPointF p1 = roi->invTransform(ptf1);
//      QPointF p2 = roi->invTransform(ptf2);

      QPolygonF fb = m_NewROI->scaledInnerOutline(sx, sy);

      m_RubberBand = QPolygon();

      for (int i=0; i<fb.count(); i++) {
        m_RubberBand.append(transform(fb.value(i)));
      }
    }
  }
}

void QcepROICreator::append(const QPoint &pt)
{
  printMessage(tr("QcepROICreator::append(QPoint(%1,%2))").arg(pt.x()).arg(pt.y()));

  m_SelectedPoints.append(pt);

  QwtPlotPicker::append(pt);
}

bool QcepROICreator::end(bool ok)
{
  QcepImagePlot *imgPlot = imagePlot();

  if (imgPlot) {
    if (m_SelectedPoints.count() == 2) {
      QcepROIModelPtr roiMod = imgPlot->roiModel();

      QPointF p1 = invTransform(m_SelectedPoints.value(0));
      QPointF p2 = invTransform(m_SelectedPoints.value(1));

      double sx = qAbs(p2.x() - p1.x())/12.5;
      double sy = qAbs(p2.y() - p1.y())/12.5;

      m_NewROI->scaleROI(sx, sy);

      roiMod->append(m_NewROI);
    }
  }

  return QwtPlotPicker::end(ok);
}

QcepROISelector::QcepROISelector(QcepImagePlot *plot) :
  QcepROIPicker(plot,
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

//  connect(this, (void (QcepPlotMeasurer::*)( const QVector<QPointF> &)) &QcepROIPicker::selected,
//          m_Plot, &QcepImagePlot::roiMouseSelected);
}

void QcepROISelector::move(const QPoint &pt)
{
  QcepROIPicker::move(pt);

  QcepImagePlot* imgPlot = imagePlot();

  if (imgPlot) {
    QcepROIModelPtr      roiMod = imgPlot->roiModel();
    QItemSelectionModel *selMod = imgPlot->roiSelection();

    if (roiMod &&
        selMod &&
        m_SelectedROI >= 0 &&
        m_SelectedPoints.count() == 2) {
      QcepROIPtr roi = roiMod->roi(m_SelectedROI);

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

bool QcepROISelector::end(bool ok)
{
  if (ok) {
    if (qcepDebug(DEBUG_ROI)) {
      printMessage(tr("Select ROI %1").arg(m_SelectedROI));
    }

    QcepImagePlot* imgPlot = imagePlot();

    if (imgPlot) {
      QcepROIModelPtr      roi = imgPlot->roiModel();
      QItemSelectionModel *sel = imgPlot->roiSelection();

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

  return QcepROIPicker::end(ok);
}

QcepROIAddNode::QcepROIAddNode(QcepImagePlot *plot) :
  QcepROIPicker(plot,
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

//  connect(this, (void (QcepPlotMeasurer::*)( const QVector<QPointF> &)) &QcepROIPicker::selected,
//          m_Plot, &QcepImagePlot::roiMouseAdded);
}

void QcepROIAddNode::move(const QPoint &pt)
{
  QcepROIPicker::move(pt);

  QcepImagePlot* imgPlot = imagePlot();

  if (imgPlot) {
    QcepROIModelPtr      roiMod = imgPlot->roiModel();
    QItemSelectionModel *selMod = imgPlot->roiSelection();

    if (roiMod &&
        selMod &&
        m_SelectedROI >= 0 &&
        m_SelectedPoints.count() == 2) {
      QcepROIPtr roi = roiMod->roi(m_SelectedROI);

      if (roi) {
        QPolygonF fb;
        if (m_SelectedShape == QcepROI::InnerShape) {
          fb = roi->innerOutline();
        } else {
          fb = roi->outerOutline();
        }

        if (m_SelectedEdge == -1) { // Moving an existing point...
          fb[m_SelectedPoint] = invTransform(m_SelectedPoints.value(1));
        } else {
          fb.insert(m_SelectedEdge+1, invTransform(m_SelectedPoints.value(1)));
        }

        fb.last() = fb.first();

        m_RubberBand = QPolygon();

        for (int i=0; i<fb.count(); i++) {
          m_RubberBand.append(transform(fb.value(i)));
        }
      }
    }
  }
}

bool QcepROIAddNode::end(bool ok)
{
  if (ok) {
    if (qcepDebug(DEBUG_ROI)) {
      printMessage(tr("Add node to ROI %1:%2").arg(m_SelectedROI).arg(m_SelectedShape));
    }

    QcepImagePlot* imgPlot = imagePlot();

    if (imgPlot) {
      QcepROIModelPtr      roiMod = imgPlot->roiModel();
      QItemSelectionModel *selMod = imgPlot->roiSelection();

      if (roiMod &&
          selMod &&
          m_SelectedROI >= 0 &&
          m_SelectedPoints.count() == 2) {
        QPointF ptf = invTransform(m_SelectedPoints.value(1));

        if (m_SelectedEdge == -1) {
          roiMod->changeROIPoint(m_SelectedROI, m_SelectedShape, m_SelectedPoint, ptf);
        } else {
          roiMod->insertROIPoint(m_SelectedROI, m_SelectedShape, m_SelectedEdge, ptf);
        }
      }
    }
  }

  return QcepROIPicker::end(ok);
}

QcepROIRemoveNode::QcepROIRemoveNode(QcepImagePlot *plot) :
  QcepROIPicker(plot,
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

//  connect(this, (void (QcepPlotMeasurer::*)( const QPointF &)) &QcepROIPicker::selected,
//          m_Plot, &QcepImagePlot::roiMouseRemoved);
}

bool QcepROIRemoveNode::end(bool ok)
{
  if (ok) {
    if (qcepDebug(DEBUG_ROI)) {
      printMessage(tr("Remove node %1 of ROI %2").arg(m_SelectedPoint).arg(m_SelectedROI));
    }

    QcepImagePlot* imgPlot = imagePlot();

    if (imgPlot) {
      QcepROIModelPtr      roiMod = imgPlot->roiModel();
      QItemSelectionModel *selMod = imgPlot->roiSelection();

      if (roiMod &&
          selMod &&
          m_SelectedROI >= 0 &&
          m_SelectedShape >= 0 &&
          m_SelectedPoint >= 0) {
        roiMod->deleteROIPoint(m_SelectedROI, m_SelectedShape, m_SelectedPoint);
      }
    }
  }

  return QcepROIPicker::end(ok);
}

QcepROIRotator::QcepROIRotator(QcepImagePlot *plot) :
  QcepROIPicker(plot,
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

//  connect(this, (void (QcepPlotMeasurer::*)( const QVector<QPointF> &)) &QcepROIPicker::selected,
//          m_Plot, &QcepImagePlot::roiMouseRotated);
}

void QcepROIRotator::move(const QPoint &pt)
{
  QcepROIPicker::move(pt);

  QcepImagePlot* imgPlot = imagePlot();

  if (imgPlot) {
    QcepROIModelPtr      roiMod = imgPlot->roiModel();
    QItemSelectionModel *selMod = imgPlot->roiSelection();

    if (roiMod &&
        selMod &&
        m_SelectedROI >= 0 &&
        m_SelectedPoints.count() == 2) {
      QcepROIPtr roi = roiMod->roi(m_SelectedROI);

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

bool QcepROIRotator::end(bool ok)
{
  if (ok) {
    if (qcepDebug(DEBUG_ROI)) {
      printMessage(tr("Rotate ROI %1").arg(m_SelectedROI));
    }

    QcepImagePlot* imgPlot = imagePlot();

    if (imgPlot) {
      QcepROIModelPtr      roiMod = imgPlot->roiModel();
      QItemSelectionModel *selMod = imgPlot->roiSelection();

      if (roiMod &&
          selMod &&
          m_SelectedROI >= 0 &&
          m_SelectedPoints.count() == 2) {
        QcepROIPtr roi = roiMod->roi(m_SelectedROI);

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

  return QcepROIPicker::end(ok);
}

QcepROIResizer::QcepROIResizer(QcepImagePlot *plot) :
  QcepROIPicker(plot,
                UseSelectedROIs,
                CanSelectEdges,
                UseAnyShape,
                SecondAppend,
                qInf(),
                10.0),
  m_ScaledX(1.0),
  m_ScaledY(1.0)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerDragLineMachine());
  setRubberBand(QwtPicker::PolygonRubberBand);

//  connect(this, (void (QcepPlotMeasurer::*)( const QVector<QPointF> &)) &QcepROIPicker::selected,
//          m_Plot, &QcepImagePlot::roiMouseResized);
}

void QcepROIResizer::move(const QPoint &pt)
{
  QcepROIPicker::move(pt);

  QcepImagePlot* imgPlot = imagePlot();

  if (imgPlot) {
    QcepROIModelPtr      roiMod = imgPlot->roiModel();
    QItemSelectionModel *selMod = imgPlot->roiSelection();

    if (roiMod &&
        selMod &&
        m_SelectedROI >= 0 &&
        m_SelectedPoints.count() == 2) {
      QcepROIPtr roi = roiMod->roi(m_SelectedROI);

      if (roi) {
        QPointF ptf1 = invTransform(m_SelectedPoints.value(0));
        QPointF ptf2 = invTransform(m_SelectedPoints.value(1));

        QPointF p1 = roi->invTransform(ptf1);
        QPointF p2 = roi->invTransform(ptf2);

        double kx = 1;
        double ky = 1;

        double rxy = fabs(p1.x()/p1.y());
        double ryx = 1/rxy;

        if (rxy > 0.25) {
          kx = p2.x()/p1.x();
        }

        if (ryx > 0.25) {
          ky = p2.y()/p1.y();
        }

        QPolygonF fb;
        if (m_SelectedShape == QcepROI::InnerShape) {
          fb = roi->scaledInnerOutline(kx, ky);
        } else {
          fb = roi->scaledOuterOutline(kx, ky);
        }

        m_RubberBand = QPolygon();

        for (int i=0; i<fb.count(); i++) {
          m_RubberBand.append(transform(fb.value(i)));
        }

        m_ScaledX = kx;
        m_ScaledY = ky;
      }
    }
  }
}

bool QcepROIResizer::end(bool ok)
{
  if (ok) {
    if (qcepDebug(DEBUG_ROI)) {
      printMessage(tr("Resize ROI %1").arg(m_SelectedROI));
    }

    QcepImagePlot* imgPlot = imagePlot();

    if (imgPlot) {
      QcepROIModelPtr      roiMod = imgPlot->roiModel();
      QItemSelectionModel *selMod = imgPlot->roiSelection();

      if (roiMod &&
          selMod &&
          m_SelectedROI >= 0 &&
          m_SelectedPoints.count() == 2) {

        roiMod->scaleROI(m_SelectedROI, m_SelectedShape, m_ScaledX, m_ScaledY);
      }
    }
  }

  return QcepROIPicker::end(ok);
}
