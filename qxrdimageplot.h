#ifndef QXRDIMAGEPLOT_H
#define QXRDIMAGEPLOT_H

#include <qwt_plot.h>
#include <qwt_color_map.h>

class QwtPlotZoomer;
class QwtLegend;
class QwtPlotSpectrogram;

class QxrdRasterData;

class QxrdImagePlot : public QwtPlot
{
  Q_OBJECT;

 public:
  QxrdImagePlot(QWidget *parent);

 public slots:
  void autoScale();

 public:
  void setImage(QxrdRasterData data);

 private:
  QwtPlotZoomer       *m_Zoomer;
  QwtLegend           *m_Legend;
  QwtPlotSpectrogram  *m_Spectrogram;
  QxrdRasterData      *m_Raster;
  QwtLinearColorMap    m_ColorMap;
};

#endif
