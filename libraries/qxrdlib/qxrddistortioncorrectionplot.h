#ifndef QXRDDISTORTIONCORRECTIONPLOT_H
#define QXRDDISTORTIONCORRECTIONPLOT_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepplot.h"
#include "qcepobjectnamer.h"

#include "qxrdwindow-ptr.h"

class QXRD_EXPORT QxrdDistortionCorrectionPlot : public QcepPlot
{
  Q_OBJECT
public:
  explicit QxrdDistortionCorrectionPlot(QWidget *parent = 0);
  void init(QcepPlotSettingsWPtr settings);
  void setWindow(QxrdWindow *win);

signals:

public slots:

private:
  QcepObjectNamer               m_ObjectNamer;
  QxrdWindow                   *m_Window;
  bool                          m_FirstTime;
};

#endif // QXRDDISTORTIONCORRECTIONPLOT_H