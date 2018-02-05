#ifndef QXRDCENTERFINDERPLOT_H
#define QXRDCENTERFINDERPLOT_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepobjectnamer.h"
#include "qcepplot.h"
#include "qcepimagedata.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata.h"
#include "qcepmaskdata-ptr.h"

#include "qxrdprocessor-ptr.h"
#include "qxrdcenterfinder-ptr.h"
#include "qxrdwindow-ptr.h"
#include "qxrdplotcurvevector-ptr.h"

//TODO: is class still used?
class QXRD_EXPORT QxrdCenterFinderPlot : public QcepPlot
{
  Q_OBJECT

public:
  QxrdCenterFinderPlot(QWidget *parent=0);
  //TODO: change to QObjectWPtr...
  void initialize(QcepPlotSettingsWPtr settings);

  void setWindow(QxrdWindow *win);

public slots:
  void onParameterChanged();
  void onCenterXChanged(double cx);
  void onCenterYChanged(double cy);
  void onCenterChanged(double cx, double cy);
  void onCenterChanged(QPointF c);
  void onProcessedImageAvailable(QcepDoubleImageDataPtr image);
  void onMaskedImageAvailable(QcepDoubleImageDataPtr image, QcepMaskDataPtr mask);

signals:
  void newCenterFinderCurves(QxrdPlotCurveVectorPtr curves);

private slots:
  void updateCenterFinderPlot();
  void onNewCenterFinderCurves(QxrdPlotCurveVectorPtr curves);

private:
  QcepObjectNamer        m_ObjectNamer;
  QxrdWindow            *m_Window;
  QxrdProcessorWPtr      m_Processor;
  QxrdCenterFinderWPtr   m_CenterFinder;
  bool                   m_FirstTime;
  QcepDoubleImageDataPtr m_Image;
  QcepMaskDataPtr        m_Mask;
  QxrdPlotCurveVectorPtr m_Curves;
};

#endif // QXRDCENTERFINDERPLOT_H
