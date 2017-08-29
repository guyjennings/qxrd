#ifndef QXRDDETECTORIMAGEPLOT_H
#define QXRDDETECTORIMAGEPLOT_H

#include "qxrdlib_global.h"
#include "qxrdimageplot.h"
#include "qxrdroipicker-ptr.h"

class QXRD_EXPORT QxrdDetectorImagePlot : public QxrdImagePlot
{
  Q_OBJECT

public:
  QxrdDetectorImagePlot(QWidget *parent=0);
  void init(QxrdImagePlotSettingsWPtr settings, QcepObjectWPtr parent);

  void contextMenuEvent(QContextMenuEvent *event);

  void enableROICreate();
  void enableROISelect();
  void enableROIAddNode();
  void enableROIRemoveNode();
  void enableROIRotate();
  void enableROIResize();

  void classifyROIPoint(double x, double y);

  typedef enum {
    NewRectROI,
    NewEllipseROI,
    NewRectDonutROI,
    NewEllipseDonutROI,
    LastNewROIType
  } NewROIType;

  static int newROITypeCount();
  static QString newROITypeName(int i);

signals:
  void changeROICreateType(int newType);

protected:
  void disablePickers();

signals:

public slots:

private:
  QxrdROICreator    *m_ROICreator;
  QxrdROISelector   *m_ROISelector;
  QxrdROIAddNode    *m_ROIAddNode;
  QxrdROIRemoveNode *m_ROIRemoveNode;
  QxrdROIRotator    *m_ROIRotator;
  QxrdROIResizer    *m_ROIResizer;
};

#endif // QXRDDETECTORIMAGEPLOT_H
