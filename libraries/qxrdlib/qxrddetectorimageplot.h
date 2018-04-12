#ifndef QXRDDETECTORIMAGEPLOT_H
#define QXRDDETECTORIMAGEPLOT_H

#include "qxrdlib_global.h"
#include "qcepimageplot.h"
#include "qceproipicker-ptr.h"

class QXRD_EXPORT QxrdDetectorImagePlot : public QcepImagePlot
{
  Q_OBJECT

public:
  QxrdDetectorImagePlot(QWidget *parent=0);
  //TODO: change to QcepObjectWPtr...
  void initialize(QcepImagePlotSettingsWPtr settings, QcepObjectWPtr parent);

  void contextMenuEvent(QContextMenuEvent *event);

  void enableROICreate();
  void enableROISelect();
  void enableROIAddNode();
  void enableROIRemoveNode();
  void enableROIRotate();
  void enableROIResize();

  void classifyROIPoint(double x, double y);

  static int newROITypeCount();
  static QString newROITypeName(int i);

//signals:
//  void changeROICreateType(int newType);

protected:
  void disablePickers();

signals:

public slots:

private:
  QcepROICreator    *m_ROICreator;
  QcepROISelector   *m_ROISelector;
  QcepROIAddNode    *m_ROIAddNode;
  QcepROIRemoveNode *m_ROIRemoveNode;
  QcepROIRotator    *m_ROIRotator;
  QcepROIResizer    *m_ROIResizer;
};

#endif // QXRDDETECTORIMAGEPLOT_H
