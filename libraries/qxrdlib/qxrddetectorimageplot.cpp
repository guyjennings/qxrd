#include "qxrddetectorimageplot.h"
#include <QMenu>
#include <QContextMenuEvent>
#include "qceproipicker.h"
#include "qceproi.h"
#include "qceproimodel.h"
#include <QItemSelectionModel>

//TODO: reimplement using QxrdPlotButtonCommand
QxrdDetectorImagePlot::QxrdDetectorImagePlot(QWidget *parent)
  : QcepImagePlot(parent),
    m_ROICreator(NULL),
    m_ROISelector(NULL),
    m_ROIAddNode(NULL),
    m_ROIRemoveNode(NULL),
    m_ROIRotator(NULL),
    m_ROIResizer(NULL)
{
}

void QxrdDetectorImagePlot::initialize(QcepImagePlotSettingsWPtr settings, QcepObjectWPtr parent)
{
  QcepImagePlot::initialize(settings, parent);

  m_ROICreator    = new QcepROICreator(this);
  m_ROISelector   = new QcepROISelector(this);
  m_ROIAddNode    = new QcepROIAddNode(this);
  m_ROIRemoveNode = new QcepROIRemoveNode(this);
  m_ROIRotator    = new QcepROIRotator(this);
  m_ROIResizer    = new QcepROIResizer(this);
}

void QxrdDetectorImagePlot::disablePickers()
{
  if (m_ROICreator) {
    m_ROICreator    -> setEnabled(false);
  }

  if (m_ROISelector) {
    m_ROISelector   -> setEnabled(false);
  }

  if (m_ROIAddNode) {
    m_ROIAddNode    -> setEnabled(false);
  }

  if (m_ROIRemoveNode) {
    m_ROIRemoveNode -> setEnabled(false);
  }

  if (m_ROIRotator) {
    m_ROIRotator -> setEnabled(false);
  }

  if (m_ROIResizer) {
    m_ROIResizer -> setEnabled(false);
  }
}

void QxrdDetectorImagePlot::enableROICreate()
{
  disablePickers();

  if (m_ROICreator) {
    m_ROICreator -> setEnabled(true);

    QMenu          *popUp = new QMenu(this);

    for (int i=0; i<newROITypeCount(); i++) {
      popUp -> addAction(newROITypeName(i),
                         [=]() { m_ROICreator -> setCreatedType(i);});
    }

    popUp->exec(QCursor::pos());
  }
}

void QxrdDetectorImagePlot::enableROISelect()
{
  disablePickers();

  if (m_ROISelector) {
    m_ROISelector -> setEnabled(true);
  }
}

void QxrdDetectorImagePlot::enableROIAddNode()
{
  disablePickers();

  if (m_ROIAddNode) {
    m_ROIAddNode -> setEnabled(true);
  }
}

void QxrdDetectorImagePlot::enableROIRemoveNode()
{
  disablePickers();

  if (m_ROIRemoveNode) {
    m_ROIRemoveNode -> setEnabled(true);
  }
}

void QxrdDetectorImagePlot::enableROIRotate()
{
  disablePickers();

  if (m_ROIRotator) {
    m_ROIRotator -> setEnabled(true);
  }
}

void QxrdDetectorImagePlot::enableROIResize()
{
  disablePickers();

  if (m_ROIResizer) {
    m_ROIResizer -> setEnabled(true);
  }
}

void QxrdDetectorImagePlot::classifyROIPoint(double x, double y)
{
  QcepROIModelPtr      roiMod = roiModel();
  QItemSelectionModel *roiSel = roiSelection();
  QPointF pt(x,y);

  if (roiMod && roiSel) {
    int nRois = roiMod->rowCount(QModelIndex());

    for (int i=0; i<nRois; i++) {
      QcepROIPtr roi = roiMod->roi(i);

      bool isSelected = roiSel -> rowIntersectsSelection(i, QModelIndex());
      bool inInner    = roi -> pointInInner(pt);
      bool inOuter    = roi -> pointInOuter(pt);

      printMessage(tr("ROI %1, Sel %2, inInner %3, inOuter %4")
                   .arg(i).arg(isSelected).arg(inInner).arg(inOuter));
    }
  }
}

void QxrdDetectorImagePlot::contextMenuEvent(QContextMenuEvent *event)
{
  QcepImagePlotSettingsPtr set(m_ImagePlotSettings);

  if (set) {
    QwtScaleMap xMap = canvasMap(QwtPlot::xBottom);
    QwtScaleMap yMap = canvasMap(QwtPlot::yLeft);

    QWidget *canv = canvas();

    QPoint evlocal = canv->mapFromParent(event->pos());

    double x = xMap.invTransform(evlocal.x());
    double y = yMap.invTransform(evlocal.y());

    QMenu plotMenu;

    QAction *auSc   = plotMenu.addAction("Autoscale");
    QAction *prGr   = plotMenu.addAction("Print Graph...");

//    QAction *lgSc   = plotMenu.addAction("Log Intensity Scale?");
//    lgSc->setCheckable(true);
//    lgSc->setChecked(set->get_DisplayLog());

//    QAction *shMk   = plotMenu.addAction("Show Mask?");
//    shMk->setCheckable(true);
//    shMk->setChecked(set->get_MaskShown());

    QAction *shRoi  = plotMenu.addAction("Show ROI Outlines?");
    shRoi->setCheckable(true);
    shRoi->setChecked(set->get_DisplayROI());

    QAction *editRoi  = plotMenu.addAction(tr("Edit selected ROI"));
    QAction *mvRoi    = plotMenu.addAction(tr("Move selected ROI centers to (%1,%2)").arg(x).arg(y));
    QAction *classify = plotMenu.addAction(tr("Classify ROI Point at (%1,%2").arg(x).arg(y));

    plotMenu.addSeparator();

    QAction *zap    = plotMenu.addAction(tr("Zap (replace with avg of neighboring values) pixel [%1,%2]").arg((int)x).arg(int(y)));

    QAction *action = plotMenu.exec(event->globalPos());

    if (action == auSc) {
      autoScale();
    } else if (action == prGr) {
      printGraph();
//    } else if (action == lgSc) {
//      toggleLogDisplay();
//    } else if (action == shMk) {
//      toggleShowMask();
    } else if (action == shRoi) {
      toggleShowROI();
    } else if (action == editRoi) {
      editSelectedROI(x,y);
    } else if (action == mvRoi) {
      moveSelectedROICenter(x,y);
    } else if (action == classify) {
      classifyROIPoint(x,y);
    } else if (action == zap) {
      zapPixel(qRound(x), qRound(y));
    }
  }
}

int QxrdDetectorImagePlot::newROITypeCount()
{
  return QcepROI::LastNewROIType;
}

QString QxrdDetectorImagePlot::newROITypeName(int i)
{
  QString res = "Unknown ROI Type";

  switch (i) {
  case QcepROI::NewRectROI:
    res = "Rectangular ROI";
    break;

  case QcepROI::NewEllipseROI:
    res = "Elliptical ROI";
    break;

  case QcepROI::NewRectDonutROI:
    res = "Rectangle in Rectangle ROI";
    break;

  case QcepROI::NewEllipseDonutROI:
    res = "Ellipse in Ellipse ROI";
    break;
  }

  return res;
}
