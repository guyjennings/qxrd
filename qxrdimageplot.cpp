#include "qxrdimageplot.h"
#include "qxrdrasterdata.h"
#include "qxrdplotzoomer.h"
#include "qxrdplottracker.h"

#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_legend.h>
#include <qwt_plot_spectrogram.h>
#include <qwt_scale_widget.h>

QxrdImagePlot::QxrdImagePlot(QWidget *parent)
  : QwtPlot(parent),
    m_Zoomer(NULL),
    m_Legend(NULL),
    m_Spectrogram(NULL),
    m_ColorMap(Qt::black, Qt::white)
{
  setCanvasBackground(QColor(Qt::white));

  m_Tracker = new QxrdPlotTracker(canvas(), this);
  m_Tracker -> setEnabled(true);
  m_Tracker -> setSelectionFlags(QwtPicker::PointSelection);

  m_Zoomer = new QxrdPlotZoomer(canvas(), this);
  m_Zoomer -> setSelectionFlags(QwtPicker::DragSelection | QwtPicker::CornerToCorner);
  m_Zoomer -> setTrackerMode(QwtPicker::AlwaysOn);
  m_Zoomer -> setRubberBand(QwtPicker::RectRubberBand);

  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect2,
                              Qt::LeftButton, Qt::ControlModifier | Qt::ShiftModifier);
  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect3,
                              Qt::LeftButton, Qt::ControlModifier);

  m_Zoomer -> setEnabled(true);

  m_Panner = new QwtPlotPanner(canvas());
  m_Panner -> setEnabled(true);
  m_Panner -> setMouseButton(Qt::MidButton);

  m_Magnifier = new QwtPlotMagnifier(canvas());
  m_Magnifier -> setEnabled(true);
  m_Magnifier -> setMouseButton(Qt::NoButton);

  m_Legend = new QwtLegend;
  m_Legend -> setFrameStyle(QFrame::Box|QFrame::Sunken);
  m_Legend -> setItemMode(QwtLegend::CheckableItem);

  insertLegend(m_Legend, QwtPlot::BottomLegend);

  m_Spectrogram = new QwtPlotSpectrogram();
  m_Spectrogram -> attach(this);
}

void QxrdImagePlot::autoScale()
{
  setAxisAutoScale(QwtPlot::xBottom);
  setAxisAutoScale(QwtPlot::yLeft);
  
  replot();

  m_Zoomer -> setZoomBase();
}

void QxrdImagePlot::set005Range()
{
  double mn = m_Raster.minValue();
  double mx = m_Raster.maxValue();

  setDisplayedRange(mn, mn+(mx-mn)*0.05);
}

void QxrdImagePlot::set010Range()
{
  double mn = m_Raster.minValue();
  double mx = m_Raster.maxValue();

  setDisplayedRange(mn, mn+(mx-mn)*0.1);
}

void QxrdImagePlot::set100Range()
{
  double mn = m_Raster.minValue();
  double mx = m_Raster.maxValue();

  setDisplayedRange(mn, mn+(mx-mn)*1.0);
}

void QxrdImagePlot::setDisplayedRange(double min, double max)
{
  setTitle(tr("Displayed Range %1 to %2").arg(min).arg(max));

  m_Raster.setDisplayedRange(min, max);
  m_Spectrogram -> setData(m_Raster);

  m_Spectrogram -> invalidateCache();
  m_Spectrogram -> itemChanged();

  QwtScaleWidget *rightAxis = axisWidget(QwtPlot::yRight);
  setAxisScale(QwtPlot::yRight, min, max);
  rightAxis -> setColorBarEnabled(true);
  rightAxis -> setColorMap(m_Spectrogram->data().range(),
                           m_Spectrogram->colorMap());

  replot();
}

void QxrdImagePlot::setGrayscale()
{
  setTitle("Grayscale");

  m_ColorMap.setColorInterval(Qt::black, Qt::white);

  changedColorMap();

  m_Zoomer -> setTrackerPen(QPen(Qt::red));
}

void QxrdImagePlot::setInverseGrayscale()
{
  m_ColorMap.setColorInterval(Qt::white, Qt::black);

  changedColorMap();

  m_Zoomer -> setTrackerPen(QPen(Qt::red));
}

void QxrdImagePlot::setEarthTones()
{
  m_ColorMap.setColorInterval(Qt::black, Qt::white);
  m_ColorMap.addColorStop(0.15, Qt::blue);
  m_ColorMap.addColorStop(0.25, Qt::gray);
  m_ColorMap.addColorStop(0.35, Qt::green);
  m_ColorMap.addColorStop(0.5, Qt::darkYellow);
  m_ColorMap.addColorStop(0.85, Qt::darkMagenta);

  changedColorMap();

  m_Zoomer -> setTrackerPen(QPen(Qt::red));
}

void QxrdImagePlot::setSpectrum()
{
  m_ColorMap.setColorInterval(Qt::magenta, Qt::red);
  m_ColorMap.addColorStop(0.2, Qt::blue);
  m_ColorMap.addColorStop(0.4, Qt::cyan);
  m_ColorMap.addColorStop(0.6, Qt::green);
  m_ColorMap.addColorStop(0.8, Qt::yellow);

  changedColorMap();

  m_Zoomer -> setTrackerPen(QPen(Qt::black));
}

void QxrdImagePlot::setFire()
{
    setTitle("Fire");

  m_ColorMap.setColorInterval(Qt::black, Qt::white);
  m_ColorMap.addColorStop(0.25, Qt::red);
  m_ColorMap.addColorStop(0.75, Qt::yellow);

  changedColorMap();

  m_Zoomer -> setTrackerPen(QPen(Qt::blue));
}

void QxrdImagePlot::changedColorMap()
{
  m_Spectrogram -> setColorMap(m_ColorMap);
  m_Spectrogram -> invalidateCache();
  m_Spectrogram -> itemChanged();

  QwtScaleWidget *rightAxis = axisWidget(QwtPlot::yRight);
  rightAxis -> setColorBarEnabled(true);
  rightAxis -> setColorMap(m_Spectrogram->data().range(),
                           m_Spectrogram->colorMap());

  replot();
}

void QxrdImagePlot::setImage(QxrdRasterData data)
{
  m_Raster = data;

  m_Spectrogram -> setData(data);
  m_Spectrogram -> setColorMap(m_ColorMap);
  m_Spectrogram -> invalidateCache();
  m_Spectrogram -> itemChanged();

  QwtScaleWidget *rightAxis = axisWidget(QwtPlot::yRight);
  rightAxis -> setColorBarEnabled(true);
  rightAxis -> setColorMap(m_Spectrogram->data().range(),
                           m_Spectrogram->colorMap());
}

QxrdRasterData* QxrdImagePlot::raster()
{
  return &m_Raster;
}
