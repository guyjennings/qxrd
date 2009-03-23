#ifndef QXRDIMAGEPLOT_H
#define QXRDIMAGEPLOT_H

#include <QObject>

#include <qwt_plot.h>
#include <qwt_color_map.h>

class QxrdPlotZoomer;
class QxrdPlotTracker;
class QwtPlotPanner;
class QwtPlotMagnifier;
class QwtLegend;
class QwtPlotSpectrogram;
class QwtPlotRescaler;

#include "qxrdrasterdata.h"

class QxrdImagePlot : public QwtPlot
{
  Q_OBJECT;

 public:
  QxrdImagePlot(QWidget *parent);

 public slots:
  void autoScale();

  void set005Range();
  void set010Range();
  void set100Range();
  void setDisplayedRange(double min, double max);
  void recalculateDisplayedRange();
  void setAutoRange();

  void setGrayscale();
  void setInverseGrayscale();
  void setEarthTones();
  void setSpectrum();
  void setFire();
  void setIce();

  void on_minimum_changed(double min);
  void on_maximum_changed(double max);
  void on_interpolate_changed(int interp);
  void on_maintain_aspect_changed(int interp);

 signals:
  void minimum_changed(double min);
  void maximum_changed(double max);\

 public:
  void setImage(QxrdRasterData data);
  QxrdRasterData* raster();
  void replotImage();

 private:
  void changedColorMap();
  void setTrackerPen(const QPen &pen);

 private:
  QxrdPlotTracker     *m_Tracker;
  QxrdPlotZoomer      *m_Zoomer;
  QwtPlotPanner       *m_Panner;
  QwtPlotMagnifier    *m_Magnifier;
  QwtPlotRescaler     *m_Rescaler;
  QwtLegend           *m_Legend;
  QwtPlotSpectrogram  *m_Spectrogram;
  QxrdRasterData       m_Raster;
  QwtLinearColorMap    m_ColorMap;
  double               m_MinDisplayed;
  double               m_MaxDisplayed;
  int                  m_Interpolate;
  int                  m_MaintainAspect;
};

#endif
