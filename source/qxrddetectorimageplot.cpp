#include "qxrddetectorimageplot.h"
#include <QMenu>
#include <QContextMenuEvent>
#include "qxrdroipicker.h"

QxrdDetectorImagePlot::QxrdDetectorImagePlot(QWidget *parent)
  : QxrdImagePlot(parent),
    m_ROISelector(NULL),
    m_ROIAddNode(NULL),
    m_ROIRemoveNode(NULL),
    m_ROIRotator(NULL),
    m_ROIResizer(NULL)
{
}

void QxrdDetectorImagePlot::init(QxrdImagePlotSettingsWPtr settings)
{
  QxrdImagePlot::init(settings);

  m_ROISelector   = new QxrdROISelector(canvas(), this);
  m_ROIAddNode    = new QxrdROIAddNode(canvas(), this);
  m_ROIRemoveNode = new QxrdROIRemoveNode(canvas(), this);
  m_ROIRotator    = new QxrdROIRotator(canvas(), this);
  m_ROIResizer    = new QxrdROIResizer(canvas(), this);
}

void QxrdDetectorImagePlot::disablePickers()
{
  QxrdImagePlot::disablePickers();

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

void QxrdDetectorImagePlot::contextMenuEvent(QContextMenuEvent *event)
{
  QxrdImagePlotSettingsPtr set(m_ImagePlotSettings);

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

    QAction *lgSc   = plotMenu.addAction("Log Intensity Scale?");
    lgSc->setCheckable(true);
    lgSc->setChecked(set->get_DisplayLog());

    QAction *shMk   = plotMenu.addAction("Show Mask?");
    shMk->setCheckable(true);
    shMk->setChecked(set->get_MaskShown());

    QAction *shRoi  = plotMenu.addAction("Show ROI Outlines?");
    shRoi->setCheckable(true);
    shRoi->setChecked(set->get_DisplayROI());
    QAction *mvRoi  = plotMenu.addAction(tr("Move selected ROI centers to (%1,%2)").arg(x).arg(y));
    plotMenu.addSeparator();

    QAction *zap    = plotMenu.addAction(tr("Zap (replace with avg of neighboring values) pixel [%1,%2]").arg((int)x).arg(int(y)));

    QAction *action = plotMenu.exec(event->globalPos());

    if (action == auSc) {
      autoScale();
    } else if (action == prGr) {
      printGraph();
    } else if (action == lgSc) {
      toggleLogDisplay();
    } else if (action == shMk) {
      toggleShowMask();
    } else if (action == shRoi) {
      toggleShowROI();
    } else if (action == mvRoi) {
      moveSelectedROICenter(x,y);
    } else if (action == zap) {
      zapPixel(qRound(x), qRound(y));
    }
  }
}
