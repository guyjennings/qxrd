#include "qxrddebug.h"
#include "qxrdimageplot.h"
#include "qxrdrasterdata.h"
#include "qxrdcenterfinder.h"
#include "qxrdprocessor.h"
#include "qxrdapplication.h"
#include "qcepallocator.h"
#include "qxrdacqcommon.h"
#include <qwt_plot_rescaler.h>
#include <qwt_plot_marker.h>
#include <qwt_legend.h>
#include <qwt_legend_label.h>
#include <qwt_plot_spectrogram.h>
#include <qwt_scale_widget.h>
#include <qwt_symbol.h>
#include <QTime>
#include <QMenu>
#include <QContextMenuEvent>
#include "qxrdroimodel.h"
#include "qxrdroi.h"
#include "qxrdroieditordialog.h"
#include "qwt_plot_piecewise_curve.h"
#include "qxrdplotwidgetdialog.h"

QxrdImagePlot::QxrdImagePlot(QWidget *parent)
  : QcepPlot(parent),
    m_ObjectNamer(this, "imageGraph"),
    m_ImagePlotSettings(),
    m_Data(NULL),
    m_Mask(NULL),
//    m_Overflow(NULL),
//    m_DataRaster(NULL),
//    m_MaskRaster(NULL),
    m_Processor(),
//    m_CenterMarker(NULL),
    m_FirstTime(true),
    m_ContextMenuEnabled(true),

    m_ROIModel(),
    m_ROISelection(NULL),
    m_ROICurves()
{
}

void QxrdImagePlot::initialize(QxrdImagePlotSettingsWPtr settings, QcepObjectWPtr parent)
{
  QcepPlot::initialize(settings);

  m_Parent = parent;

  m_ImagePlotSettings = settings;

  m_Legend -> setFrameStyle(QFrame::Box|QFrame::Sunken);
  m_Legend -> setDefaultItemMode(QwtLegendData::Checkable);

//  m_CenterMarker = new QwtPlotMarker();
//  m_CenterMarker -> setLineStyle(QwtPlotMarker::Cross);
//  m_CenterMarker -> attach(this);
}

void QxrdImagePlot::printMessage(QString msg, QDateTime dt) const
{
  QcepObjectPtr parent(m_Parent);

  if (parent) {
    parent->printMessage(msg, dt);
  }
}

void QxrdImagePlot::setProcessor(QxrdProcessorWPtr proc)
{
  m_Processor = proc;

  QxrdProcessorPtr dp(m_Processor);

  if (dp) {
    QxrdCenterFinderPtr cf(dp->centerFinder());

    if (cf) {
//      onCenterChanged(cf->get_Center());

      connect(cf->prop_MarkedPoints(), &QxrdPowderPointVectorProperty::valueChanged,
              this, &QxrdImagePlot::onMarkedPointsChanged);

      onMarkedPointsChanged();
    }
  }
}

QxrdProcessorWPtr QxrdImagePlot::processor() const
{
  return m_Processor;
}

void QxrdImagePlot::setData(QcepImageDataBaseWPtr data)
{
  m_Data = data;

  QcepImageDataBasePtr d(m_Data);

  if (d) {
    setTitle(d->get_FileName());
  }
}

void QxrdImagePlot::setMask(QcepMaskDataWPtr mask)
{
  m_Mask = mask;
}

void QxrdImagePlot::autoScale()
{
  QcepPlot::autoScale();
}

void QxrdImagePlot::setAutoRange()
{
  if (g_Application) {
    g_Application->criticalMessage("QxrdImagePlot::setAutoRange To do...");
  }
}

void QxrdImagePlot::toggleShowROI()
{
  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set) {
    changeROIShown(!set->get_DisplayROI());
  }
}

void QxrdImagePlot::changeROIShown(bool shown)
{
  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set) {
    set->set_DisplayROI(shown);

    updateROIDisplay();
  }
}

//void QxrdImagePlot::onCenterXChanged(double cx)
//{
//  m_CenterMarker -> setXValue(cx);
//  replot();
//}

//void QxrdImagePlot::onCenterYChanged(double cy)
//{
//  m_CenterMarker -> setYValue(cy);
//  replot();
//}

//void QxrdImagePlot::onCenterChanged(QPointF c)
//{
//  m_CenterMarker -> setValue(c);
//  replot();
//}

//const QxrdRasterData* QxrdImagePlot::raster() const
//{
//  return m_DataRaster;
//}

//QxrdRasterData* QxrdImagePlot::raster()
//{
//  return m_DataRaster;
//}

QwtText QxrdImagePlot::trackerTextF(const QPointF &pos)
{
  QxrdProcessorPtr    processor = this->processor();
  QxrdCenterFinderPtr centerFinder;

  if (processor) {
    centerFinder = processor->centerFinder();
  }

  QString res = tr("%1, %2").arg(pos.x()).arg(pos.y());

  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set) {
    set->set_XMouse(pos.x());
    set->set_YMouse(pos.y());
  }

  QcepImageDataBasePtr data(m_Data);

  if (data) {
    double val = data->getImageData(pos.x(),pos.y());
    res += tr(", %1").arg(val);

    if (set) {
      set->set_ValMouse(val);
    }
  }

  QcepMaskDataPtr mask(m_Mask);

  if (mask) {
    double maskv = mask->getImageData((int) pos.x(), (int) pos.y());
    res += tr(", %1").arg(maskv);

    if (set) {
      set->set_MaskMouse(maskv);
    }
  }

  if (centerFinder) {
    double tth = centerFinder->getTTH(pos);
    res += tr("\nTTH %1").arg(tth);

    if (set) {
      set->set_TTHMouse(tth);
    }

    double q = centerFinder->getQ(pos);
    res += tr(", Q %1").arg(q);

    if (set) {
      set->set_QMouse(q);
    }

    double r = centerFinder->getR(pos);
    res += tr(", R %1").arg(r);

    if (set) {
      set->set_RMouse(r);
    }

    double chi = centerFinder->getChi(pos);
    res += tr(", Chi %1").arg(chi);

//    if (ras && m_PowderPointPicker -> isEnabled()) {
//      QPointF rpt = ras->optimizePeakPosition(pos);
//      res += tr("\nPtx %1, Pty %2").arg(rpt.x()).arg(rpt.y());
//    }
  }

  return res;
}

//TODO: remove
void QxrdImagePlot::contextMenuEvent(QContextMenuEvent * event)
{
  if (m_ContextMenuEnabled) {
    QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

    if (set) {
      QMenu plotMenu(NULL, NULL);

      QAction *auSc = plotMenu.addAction("Autoscale");
      QAction *prGr = plotMenu.addAction("Print Graph...");

      plotMenu.addSeparator();

      QxrdProcessorPtr dp(m_Processor);

      if (dp) {
        QxrdCenterFinderPtr cf(dp->centerFinder());

        if (cf) {
          QwtScaleMap xMap = canvasMap(QwtPlot::xBottom);
          QwtScaleMap yMap = canvasMap(QwtPlot::yLeft);

          QWidget *canv = canvas();

          QPoint evlocal = canv->mapFromParent(event->pos());

          double x = xMap.invTransform(evlocal.x());
          double y = yMap.invTransform(evlocal.y());

          QxrdPowderPoint nearest = cf->nearestPowderPoint(x, y);

          QAction *fitCircle        = plotMenu.addAction(tr("Fit Circle Center from Points on Ring %1").arg(nearest.n1()));
          QAction *fitEllipse       = plotMenu.addAction(tr("Fit Ellipse from Points on Ring %1").arg(nearest.n1()));
          QAction *fitEllipses      = plotMenu.addAction(tr("Fit Ellipses to all powder rings"));

          QAction *adjustEnergy     = plotMenu.addAction(tr("Adjust Energy to match Calibrant Ring %1").arg(nearest.n1()));
          QAction *adjustDistance   = plotMenu.addAction(tr("Adjust Detector Distance to match Calibrant Ring %1").arg(nearest.n1()));

          QAction *adjustFit        = plotMenu.addAction(tr("Fit to nearby peak when adding powder points?"));
          adjustFit->setCheckable(true); adjustFit->setChecked(cf->get_FitPowderPointPosition());
          QAction *addPoint         = plotMenu.addAction(tr("Add point at (%1,%2)").arg(x).arg(y));
          QAction *delPoint         = plotMenu.addAction(tr("Delete point at (%1,%2)").arg(nearest.x()).arg(nearest.y()));
          QAction *delRing          = plotMenu.addAction(tr("Delete Ring %1").arg(nearest.n1()));
          QAction *deleteAllPoints  = plotMenu.addAction(tr("Delete all Rings"));
          QAction *disableRing      = plotMenu.addAction(tr("Disable Ring %1").arg(nearest.n1()));
          QAction *enableRing       = plotMenu.addAction(tr("Enable Ring %1").arg(nearest.n1()));
          QAction *normalizeRings   = plotMenu.addAction(tr("Normalize Powder Rings"));
          QAction *fitPeakNear      = plotMenu.addAction(tr("Fit Diffracted Peak near (%1,%2) [%3,%4]").arg(x).arg(y).arg(event->x()).arg(event->y()));
          QAction *fitRingNear      = plotMenu.addAction(tr("Fit Point on Diffracted Ring near (%1,%2) [%3,%4]").arg(x).arg(y).arg(event->x()).arg(event->y()));
          QAction *traceRingClockwise = plotMenu.addAction(tr("Trace Diffracted Ring starting at (%1,%2) [%3,%4]").arg(x).arg(y).arg(event->x()).arg(event->y()));
          QAction *missingRing      = plotMenu.addAction(tr("Missing Diffracted Ring near (%1,%2)").arg(x).arg(y));
          //          QAction *traceRingParallel = plotMenu.addAction(tr("Trace Diffracted Ring starting at (%1,%2) [%3,%4] in parallel").arg(x).arg(y).arg(event->x()).arg(event->y()));
          QAction *zapPixel         = plotMenu.addAction(tr("Zap (replace with avg of neighboring values) pixel [%1,%2]").arg((int)x).arg(int(y)));

          QAction *action = plotMenu.exec(event->globalPos());

          if (action == auSc) {
            autoScale();
          } else if (action == prGr) {
            printGraph();
          } else if (action == fitCircle) {
            cf->fitPowderCircle(nearest.n1());
          } else if (action == fitEllipse) {
            cf->fitPowderEllipse(nearest.n1());
          } else if (action == fitEllipses) {
            cf->fitPowderEllipses();
          } else if (action == adjustEnergy) {
            cf->adjustEnergy(nearest.n1());
          } else if (action == adjustDistance) {
            cf->adjustDistance(nearest.n1());
          } else if (action == adjustFit) {
            cf->toggle_FitPowderPointPosition();
          } else if (action == addPoint) {
            cf->appendPowderPoint(x,y);
          } else if (action == delPoint) {
            cf->deletePowderPointNear(x,y);
          } else if (action == delRing) {
            cf->deletePowderRing(nearest.n1());
          } else if (action == deleteAllPoints) {
            cf->deletePowderPoints();
          } else if (action == enableRing) {
            cf->enablePowderRing(nearest.n1());
          } else if (action == disableRing) {
            cf->disablePowderRing(nearest.n1());
          } else if (action == normalizeRings) {
            cf->normalizePowderRings();
          } else if (action == fitPeakNear) {
            QMetaObject::invokeMethod(cf.data(), "fitPeakNear",
                                      Q_ARG(double,x),
                                      Q_ARG(double,y));
          } else if (action == fitRingNear) {
            QMetaObject::invokeMethod(cf.data(), "fitRingNear",
                                      Q_ARG(double,x),
                                      Q_ARG(double,y));
          } else if (action == traceRingClockwise) {
            QMetaObject::invokeMethod(cf.data(), "traceRingNear",
                                      Q_ARG(double,x),
                                      Q_ARG(double,y),
                                      Q_ARG(double,25.0));
          } else if (action == missingRing) {
            cf->missingRingNear(x,y);
//            QMetaObject::invokeMethod((cf.data(), "missingRingNear",
//                                       Q_ARG(double,x),
//                                       Q_ARG(double,y));
//          } else if (action == traceRingParallel) {
//            QMetaObject::invokeMethod(cf.data(), "traceRingNearParallel",
//                                      Q_ARG(double,x),
//                                      Q_ARG(double,y),
//                                      Q_ARG(double,25.0));
          } else if (action == zapPixel) {
            this->zapPixel(qRound(x),qRound(y));
          }
        }
      }
    }

    event->accept();
  } else {
    event->accept();
  }
}

void QxrdImagePlot::zapPixel(int x, int y)
{
  double sum = 0;
  int    npx = 0;

  QcepImageDataBasePtr data(m_Data);

  //TODO: move into QxrdImagePlotWidget
  if (data) {
    for (int ix = x-1; ix <= x+1; ix++) {
      for (int iy = y-1; iy <= y+1; iy++) {
        sum += data->getImageData(ix, iy);
        npx += 1;
      }
    }

    sum -= data->getImageData(x,y);
    npx -= 1;

    data->setImageData(x,y, sum/npx);

    replot();
  }
}

void QxrdImagePlot::onMarkedPointsChanged()
{
  displayPowderMarkers();

  replot();
}

void QxrdImagePlot::displayPowderMarkers()
{
  clearPowderMarkers();

  QxrdProcessorPtr dp(m_Processor);

  if (dp) {
    QxrdCenterFinderPtr cf(dp->centerFinder());

    if (cf) {
      int nrgs = cf->countPowderRings();
      int npts = cf->countPowderRingPoints();

      for (int r=0; r<nrgs; r++) {
        QVector<double> x,y;

        for (int i=0; i<npts; i++) {
          QxrdPowderPoint pt = cf->powderRingPoint(i);

          if (pt.n1() == r) {
            x.append(pt.x());
            y.append(pt.y());
          }
        }

        if (x.count() > 0) {
          QwtPlotCurve *pc = new QwtPlotCurve(tr("Ring %1").arg(r));

          setPlotCurveStyle(r, pc);
          pc -> setSamples(x, y);
          pc -> setStyle(QwtPlotCurve::NoCurve);
          pc -> setLegendAttribute(QwtPlotCurve::LegendShowSymbol, true);
          pc -> attach(this);

          m_PowderPointCurves.append(pc);
        }
      }

      replot();
    }
  }
}

void QxrdImagePlot::clearPowderMarkers()
{
  foreach(QwtPlotCurve *curve, m_PowderPointCurves) {
    curve->detach();
    delete curve;
  }

  m_PowderPointCurves.clear();
}

void QxrdImagePlot::enableROIDisplay(bool enable)
{
  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set) {
    set->set_DisplayROI(enable);
  }

  updateROIDisplay();
}

void QxrdImagePlot::setROIModel(QxrdROIModelWPtr model)
{
  m_ROIModel = model;

  updateROIDisplay();

  QxrdROIModelPtr roiModel(m_ROIModel);

  if (roiModel) {
    connect(roiModel.data(), &QAbstractItemModel::modelReset,    this, &QxrdImagePlot::updateROIDisplay);
    connect(roiModel.data(), &QAbstractItemModel::dataChanged,   this, &QxrdImagePlot::roiDataChanged);
    connect(roiModel.data(), &QAbstractItemModel::rowsInserted,  this, &QxrdImagePlot::roiRowsInserted);
    connect(roiModel.data(), &QAbstractItemModel::rowsMoved,     this, &QxrdImagePlot::roiRowsMoved);
    connect(roiModel.data(), &QAbstractItemModel::rowsRemoved,   this, &QxrdImagePlot::roiRowsRemoved);
  }
}

QxrdROIModelWPtr QxrdImagePlot::roiModel()
{
  return m_ROIModel;
}

void QxrdImagePlot::setROISelection(QItemSelectionModel *select)
{
  m_ROISelection = select;

  updateROIDisplay();

  if (m_ROISelection) {
    connect(m_ROISelection, &QItemSelectionModel::selectionChanged,
            this, &QxrdImagePlot::updateROISelection);
  }
}

QItemSelectionModel* QxrdImagePlot::roiSelection()
{
  return m_ROISelection;
}

void QxrdImagePlot::clearROIDisplay()
{
  foreach (QwtPlotCurve *curve, m_ROICurves) {
    curve->detach();
    delete curve;
  }

  m_ROICurves.clear();
}

void QxrdImagePlot::updateROIDisplay()
{
  clearROIDisplay();

  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set && set->get_DisplayROI() && m_ROIModel && m_ROISelection) {
    QxrdROIModelPtr roiModel(m_ROIModel);

    if (roiModel) {
      int nROI = roiModel->rowCount(QModelIndex());

      for (int i=0; i<nROI; i++) {
        QxrdROIPtr roi = roiModel->roi(i);

        if (roi) {
          QVector<QPointF> pts = roi->markerCoords();

          QwtPlotPiecewiseCurve *pc = new QwtPlotPiecewiseCurve(this, tr("ROI %1").arg(i));

          setPlotCurveStyle(i, pc);

          bool on = m_ROISelection->rowIntersectsSelection(i, QModelIndex());

          if (pc) {
            QPen pen = pc->pen();
            const QwtSymbol *oldsym = pc->symbol();
            QwtSymbol *sym = NULL;

            if (oldsym) {
              sym = new QwtSymbol(oldsym->style(), oldsym->brush(), oldsym->pen(), oldsym->size());
            }

            if (on) {
              pen.setWidth(3);
              if (sym) {
                sym->setSize(9,9);
              }
            } else {
              pen.setWidth(1);
              if (sym) {
                sym->setSize(5,5);
              }
            }
            pc->setPen(pen);
            if (sym) {
              pc->setSymbol(sym);
            }
          }

          pc->setSamples(pts);

          pc->attach(this);

          m_ROICurves.append(pc);
        }
      }
    }
  }

  replot();
}

void QxrdImagePlot::roiDataChanged(const QModelIndex & /*topLeft*/,
                                   const QModelIndex & bottomRight,
                                   const QVector<int> & /*roles*/)
{
//  int t = topLeft.row();
//  int l = topLeft.column();
//  int b = bottomRight.row();
  int r = bottomRight.column();

//  printMessage(tr("roiDataChanged t:%1 l:%2 b:%3 r:%4 n:%5")
//               .arg(t).arg(l).arg(b).arg(r).arg(roles.count()));

  if (r >= QxrdROIModel::OuterTypeCol) {
    updateROIDisplay();
  }
}

void QxrdImagePlot::roiRowsInserted(const QModelIndex & /*parent*/, int /*first*/, int /*last*/)
{
  updateROIDisplay();
}

void QxrdImagePlot::roiRowsMoved(const QModelIndex & /*parent*/, int /*start*/, int /*end*/,
                                 const QModelIndex & /*destination*/, int /*row*/)
{
  updateROIDisplay();
}

void QxrdImagePlot::roiRowsRemoved(const QModelIndex & /*parent*/, int /*first*/, int /*last*/)
{
  updateROIDisplay();
}

void QxrdImagePlot::onLegendChecked(const QVariant &itemInfo, bool on, int index)
{
  QwtPlotItem *item = infoToItem(itemInfo);

  if (item) {
    QwtPlotCurve *pc = dynamic_cast<QwtPlotCurve*>(item);

    int i = m_ROICurves.indexOf(pc);

    if (i >= 0) {
      QxrdROIModelPtr roiModel(m_ROIModel);

      if (m_ROISelection && roiModel) {
        if (on) {
          m_ROISelection->select(roiModel->index(i,0),
                                 QItemSelectionModel::Select | QItemSelectionModel::Rows);
        } else {
          m_ROISelection->select(roiModel->index(i,0),
                                 QItemSelectionModel::Deselect | QItemSelectionModel::Rows);
        }
      }
    }
  }

  QcepPlot::onLegendChecked(itemInfo, on, index);
}

void QxrdImagePlot::selectROIItem(int n, bool selected)
{
  QwtPlotCurve *pc = m_ROICurves.value(n);

  if (pc) {
    QPen pen = pc->pen();
    const QwtSymbol *oldsym = pc->symbol();
    QwtSymbol *sym = NULL;

    if (oldsym) {
     sym = new QwtSymbol(oldsym->style(), oldsym->brush(), oldsym->pen(), oldsym->size());
    }

    if (selected) {
      pen.setWidth(3);
      if (sym) {
        sym->setSize(9,9);
      }
    } else {
      pen.setWidth(1);
      if (sym) {
        sym->setSize(5,5);
      }
    }
    pc->setPen(pen);
    if (sym) {
      pc->setSymbol(sym);
    }
  }
}

void QxrdImagePlot::selectROILabel(int i, bool on)
{
  const QVariant itemInfo = itemToInfo(m_ROICurves.value(i));

  QwtLegendLabel *legendLabel =
      qobject_cast<QwtLegendLabel*>(
        m_Legend->legendWidget(itemInfo));

  if (legendLabel) {
    legendLabel->setChecked(on);
  }
}

void QxrdImagePlot::updateROISelection(
    const QItemSelection & /*selected*/,
    const QItemSelection & /*deselected*/)
{
  QxrdROIModelPtr roiModel(m_ROIModel);

  if (roiModel && m_ROISelection) {
    int n = roiModel->roiCount();

    for (int i=0; i<n; i++) {
      bool sel = m_ROISelection->rowIntersectsSelection(i, QModelIndex());

      selectROIItem(i, sel);
      selectROILabel(i, sel);
    }

    replot();
  }
}

void QxrdImagePlot::moveSelectedROICenter(double x, double y)
{
  QxrdROIModelPtr roiModel(m_ROIModel);

  if (roiModel && m_ROISelection) {
    int n = roiModel->roiCount();

    for (int i=0; i<n; i++) {
      if (m_ROISelection->rowIntersectsSelection(i,QModelIndex())) {
        roiModel->moveROICenter(i, x, y);
      }
    }
  }
}

void QxrdImagePlot::editSelectedROI(double x, double y)
{
  QxrdROIModelPtr roiModel(m_ROIModel);
  QPointF pt(x,y);

  if (roiModel) {
    int nearest = -1;
    double nearestDistance = qInf();

    int n = roiModel->roiCount();

    for (int i=0; i<n; i++) {
      QxrdROIPtr roi = roiModel->roi(i);

      if (roi) {
        double dist = roi->nearestDistance(pt);

        if (dist < nearestDistance) {
          nearestDistance = dist;
          nearest = i;
        }
      }
    }

    if (nearest >= 0) {
      QxrdROIPtr roi = roiModel->roi(nearest);

      QxrdROIEditorDialog editor(roi, this);

      editor.setWindowTitle(tr("Edit ROI %1").arg(nearest));

      if (editor.exec() == QDialog::Accepted) {
        roiModel->setRoi(nearest, editor.roi());
      }
    }
  }
}

//void QxrdImagePlot::roiMouseSelected(const QVector<QPointF> &p)
//{
//  QxrdROIModelPtr roiModel(m_ROIModel);

//  if (roiModel && p.count() == 2) {
//    QPointF del = p.value(1) - p.value(0);

//    if ((del.x() == 0) && (del.y() == 0)) {
//      int roiId, inOut, roiType, roiPtIndex;

//      if (roiModel->identifyROIPointByMouse(p.value(0),  scaledDelta(5,5), roiId, inOut, roiType, roiPtIndex)) {
//        printf("Select ROI %d:%d\n", roiId, inOut);
//      }
//    } else {
//      // Move click...

//      if (m_ROISelection) {
//        int n = roiModel->roiCount();

//        for (int i=0; i<n; i++) {
//          if (m_ROISelection->rowIntersectsSelection(i,QModelIndex())) {
//            roiModel->moveROIRelative(i, del.x(), del.y());
//          }
//        }
//      }
//    }
//  }
//}

//void QxrdImagePlot::roiMouseAdded(const QVector<QPointF> &p)
//{
//  QxrdROIModelPtr roiModel(m_ROIModel);

//  if (roiModel && p.count() == 2) {
//    int roiId, inOut, roiType, roiPtIndex;

//    if (roiModel->identifyROIPointByMouse(p.value(0),  scaledDelta(5,5), roiId, inOut, roiType, roiPtIndex)) {
//      printf("Add ROI Pt %d:%d Pt: %d\n", roiId, inOut, roiPtIndex);
//    }
//  }
//}

//void QxrdImagePlot::roiMouseRemoved(const QPointF &pt)
//{
//  QxrdROIModelPtr roiModel(m_ROIModel);

//  if (roiModel) {
//    int roiId, inOut, roiType, roiPtIndex;

//    if (roiModel->identifyROIPointByMouse(pt,  scaledDelta(5,5), roiId, inOut, roiType, roiPtIndex)) {
//      printf("Move ROI %d:%d Pt: %d\n", roiId, inOut, roiPtIndex);
//    }
//  }
//}

//void QxrdImagePlot::roiMouseRotated(const QVector<QPointF> &p)
//{
//  if (p.count() == 2) {
//    QxrdROIModelPtr roiModel(m_ROIModel);

//    if (roiModel && m_ROISelection) {
//      int n = roiModel->roiCount();

//      for (int i=0; i<n; i++) {
//        if (m_ROISelection->rowIntersectsSelection(i,QModelIndex())) {
//          roiModel->rotateROIByMouse(i, p.value(0), p.value(1));
//        }
//      }
//    }
//  }
//}

//void QxrdImagePlot::roiMouseResized(const QVector<QPointF> &p)
//{
//  QxrdROIModelPtr roiModel(m_ROIModel);

//  if (roiModel && p.count() == 2) {
//    int roiId, inOut, roiType, roiPtIndex;

//    if (roiModel->identifyROIPointByMouse(p.value(0),  scaledDelta(5,5), roiId, inOut, roiType, roiPtIndex)) {
//      printf("Resize ROI %d:%d\n", roiId, inOut);
//    }
//  }
//}

//QPointF QxrdImagePlot::scaledDelta(double dx, double dy)
//{
//  QwtScaleMap xMap = canvasMap(QwtPlot::xBottom);
//  QwtScaleMap yMap = canvasMap(QwtPlot::yLeft);

//  double tdx = xMap.transform(dx) - xMap.transform(0);
//  double tdy = yMap.transform(dy) - yMap.transform(0);

//  return QPointF(tdx, tdy);
//}
