#ifndef QXRDDISTORTIONCORRECTIONPLOT_H
#define QXRDDISTORTIONCORRECTIONPLOT_H

#include "qcepmacros.h"

#include "qxrdimageplot.h"

#include "qxrdobjectnamer.h"
#include "qxrddataprocessor-ptr.h"
#include "qxrddataprocessor.h"
#include "qxrddistortioncorrection-ptr.h"
#include "qxrdwindow-ptr.h"

class QxrdDistortionCorrectionPlot : public QxrdPlot
{
  Q_OBJECT
public:
  explicit QxrdDistortionCorrectionPlot(QWidget *parent = 0);
  void init(QxrdPlotSettingsWPtr settings);
  void setWindow(QxrdWindow *win);

signals:

public slots:

private:
  QxrdObjectNamer               m_ObjectNamer;
  QxrdWindow                   *m_Window;
  QxrdDataProcessorWPtr         m_DataProcessor;
  QxrdDistortionCorrectionWPtr  m_DistortionCorrection;
  bool                          m_FirstTime;
};

#endif // QXRDDISTORTIONCORRECTIONPLOT_H
