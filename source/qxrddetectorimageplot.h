#ifndef QXRDDETECTORIMAGEPLOT_H
#define QXRDDETECTORIMAGEPLOT_H

#include "qxrdimageplot.h"
#include "qxrdroipicker-ptr.h"

class QxrdDetectorImagePlot : public QxrdImagePlot
{
  Q_OBJECT

public:
  QxrdDetectorImagePlot(QWidget *parent=0);
  void init(QxrdImagePlotSettingsWPtr settings, QcepObjectWPtr parent);

  void contextMenuEvent(QContextMenuEvent *event);

  void enableROISelect();
  void enableROIAddNode();
  void enableROIRemoveNode();
  void enableROIRotate();
  void enableROIResize();

  void classifyROIPoint(double x, double y);

protected:
  void disablePickers();

signals:

public slots:

private:
  QxrdROISelector   *m_ROISelector;
  QxrdROIAddNode    *m_ROIAddNode;
  QxrdROIRemoveNode *m_ROIRemoveNode;
  QxrdROIRotator    *m_ROIRotator;
  QxrdROIResizer    *m_ROIResizer;
};

#endif // QXRDDETECTORIMAGEPLOT_H
