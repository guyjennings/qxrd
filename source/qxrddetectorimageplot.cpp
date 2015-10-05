#include "qxrddetectorimageplot.h"
#include <QMenu>
#include <QContextMenuEvent>

QxrdDetectorImagePlot::QxrdDetectorImagePlot(QWidget *parent)
  : QxrdImagePlot(parent)
{
}

void QxrdDetectorImagePlot::init(QxrdImagePlotSettingsWPtr settings)
{
  QxrdImagePlot::init(settings);
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
