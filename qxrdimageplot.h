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

  void setGrayscale();
  void setInverseGrayscale();
  void setEarthTones();
  void setSpectrum();
  void setFire();

 public:
  void setImage(QxrdRasterData data);
  QxrdRasterData* raster();

 private:
  void changedColorMap();

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
};

#endif
