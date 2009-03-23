#include "qxrdimageplot.h"
#include "qxrdrasterdata.h"
#include "qxrdplotzoomer.h"
#include "qxrdplottracker.h"

#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_rescaler.h>
#include <qwt_legend.h>
#include <qwt_plot_spectrogram.h>
#include <qwt_scale_widget.h>

QxrdImagePlot::QxrdImagePlot(QWidget *parent)
  : QwtPlot(parent),
    m_Tracker(NULL),
    m_Zoomer(NULL),
    m_Panner(NULL),
    m_Rescaler(NULL),
    m_Legend(NULL),
    m_Spectrogram(NULL),
    m_ColorMap(Qt::black, Qt::white),
    m_MinDisplayed(-10),
    m_MaxDisplayed(110),
    m_Interpolate(1),
    m_MaintainAspect(1)
{
  setCanvasBackground(QColor(Qt::white));

  m_Tracker = new QxrdPlotTracker(canvas(), this);
  m_Tracker -> setEnabled(true);
  m_Tracker -> setSelectionFlags(QwtPicker::PointSelection);
//   m_Tracker -> setAxisEnabled(QwtPlot::yRight, false);

  m_Zoomer = new QxrdPlotZoomer(canvas(), this);
  m_Zoomer -> setSelectionFlags(QwtPicker::DragSelection | QwtPicker::CornerToCorner);
  m_Zoomer -> setTrackerMode(QwtPicker::AlwaysOn);
  m_Zoomer -> setRubberBand(QwtPicker::RectRubberBand);

  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect2,
                              Qt::LeftButton, Qt::ControlModifier | Qt::ShiftModifier);
  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect3,
                              Qt::LeftButton, Qt::ControlModifier);

  m_Zoomer -> setEnabled(true);
//   m_Zoomer -> setAxisEnabled(QwtPlot::yRight, false);

  m_Panner = new QwtPlotPanner(canvas());
  m_Panner -> setEnabled(true);
  m_Panner -> setMouseButton(Qt::MidButton);
  m_Panner -> setAxisEnabled(QwtPlot::yRight, false);

  m_Magnifier = new QwtPlotMagnifier(canvas());
  m_Magnifier -> setEnabled(true);
  m_Magnifier -> setMouseButton(Qt::NoButton);
  m_Magnifier -> setAxisEnabled(QwtPlot::yRight, false);

  m_Rescaler = new QwtPlotRescaler(canvas(), QwtPlot::xBottom, QwtPlotRescaler::Expanding);
  m_Rescaler -> setEnabled(true);
//   m_Rescaler -> setAxisEnabled(QwtPlot::yRight, false);

  m_Legend = new QwtLegend;
  m_Legend -> setFrameStyle(QFrame::Box|QFrame::Sunken);
  m_Legend -> setItemMode(QwtLegend::CheckableItem);

  insertLegend(m_Legend, QwtPlot::BottomLegend);

  m_Spectrogram = new QwtPlotSpectrogram();
  m_Spectrogram -> attach(this);

  setDisplayedRange(0,100);
  setGrayscale();
}

void QxrdImagePlot::autoScale()
{
  setAxisAutoScale(QwtPlot::xBottom);
  setAxisAutoScale(QwtPlot::yLeft);
  
  replot();

  m_Zoomer -> setZoomBase();
}

void QxrdImagePlot::setAutoRange()
{
  printf("To do...\n");
}

void QxrdImagePlot::set005Range()
{
  setDisplayedRange(0, 5);
}

void QxrdImagePlot::set010Range()
{
  setDisplayedRange(0, 10);
}

void QxrdImagePlot::set100Range()
{
  setDisplayedRange(0, 100);
}

void QxrdImagePlot::setDisplayedRange(double min, double max)
{
  if (min != m_MinDisplayed) {
//    printf("QxrdImagePlot::setDisplayedRange emit minimum_changed(%g)\n", min);

    emit minimum_changed(min);
    m_MinDisplayed = min;
  }

  if (max != m_MaxDisplayed) {
//    printf("QxrdImagePlot::setDisplayedRange emit maximum_changed(%g)\n", max);

    emit maximum_changed(max);
    m_MaxDisplayed = max;
  }

  recalculateDisplayedRange();
}

void QxrdImagePlot::recalculateDisplayedRange()
{
  double minv = m_Raster.minValue();
  double maxv = m_Raster.maxValue();
  double del = maxv-minv;

  double mindis = minv+del*m_MinDisplayed/100.0;
  double maxdis = minv+del*m_MaxDisplayed/100.0;

  m_Raster.setDisplayedRange(mindis, maxdis);

  QwtScaleWidget *rightAxis = axisWidget(QwtPlot::yRight);
  setAxisScale(QwtPlot::yRight, mindis, maxdis);
  rightAxis -> setColorBarEnabled(true);
  rightAxis -> setColorMap(m_Spectrogram->data().range(),
                           m_Spectrogram->colorMap());

  replotImage();
}

void QxrdImagePlot::replotImage()
{
  m_Spectrogram -> setData(m_Raster);

  m_Spectrogram -> invalidateCache();
  m_Spectrogram -> itemChanged();

  replot();
}

void QxrdImagePlot::on_minimum_changed(double min)
{
//  printf("QxrdImagePlot::on_minimum_changed(%g)\n", min);

  setDisplayedRange(min, m_MaxDisplayed);
}

void QxrdImagePlot::on_maximum_changed(double max)
{
//  printf("QxrdImagePlot::on_maximum_changed(%g)\n", max);

  setDisplayedRange(m_MinDisplayed, max);
}

void QxrdImagePlot::on_interpolate_changed(int interp)
{
//  printf("QxrdImagePlot::on_interpolate_changed(%d)\n", interp);

  m_Raster.setInterpolate(interp);

  replotImage();
}

void QxrdImagePlot::on_maintain_aspect_changed(int interp)
{
//  printf("QxrdImagePlot::on_maintain_aspect_changed(%d)\n", interp);

  m_Rescaler -> setEnabled(interp);
}

void QxrdImagePlot::setTrackerPen(const QPen &pen)
{
   m_Tracker -> setTrackerPen(pen);
   m_Tracker -> setRubberBandPen(pen);
   m_Zoomer -> setTrackerPen(pen);
   m_Zoomer -> setRubberBandPen(pen);
}

void QxrdImagePlot::setGrayscale()
{
  m_ColorMap.setColorInterval(Qt::black, Qt::white);

  changedColorMap();

  setTrackerPen(QPen(Qt::red));
}

void QxrdImagePlot::setInverseGrayscale()
{
  m_ColorMap.setColorInterval(Qt::white, Qt::black);

  changedColorMap();

  setTrackerPen(QPen(Qt::red));
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

  setTrackerPen(QPen(Qt::red));
}

void QxrdImagePlot::setSpectrum()
{
  m_ColorMap.setColorInterval(Qt::magenta, Qt::red);
  m_ColorMap.addColorStop(0.2, Qt::blue);
  m_ColorMap.addColorStop(0.4, Qt::cyan);
  m_ColorMap.addColorStop(0.6, Qt::green);
  m_ColorMap.addColorStop(0.8, Qt::yellow);

  changedColorMap();

  setTrackerPen(QPen(Qt::black));
}

void QxrdImagePlot::setFire()
{
  m_ColorMap.setColorInterval(Qt::black, Qt::white);
  m_ColorMap.addColorStop(0.25, Qt::red);
  m_ColorMap.addColorStop(0.75, Qt::yellow);

  changedColorMap();

  setTrackerPen(QPen(Qt::blue));
}

void QxrdImagePlot::setIce()
{
  m_ColorMap.setColorInterval(Qt::black, Qt::white);
//  m_ColorMap.addColorStop(0.25, Qt::darkblue);
  m_ColorMap.addColorStop(0.5, Qt::blue);
  m_ColorMap.addColorStop(0.75, Qt::cyan);

  changedColorMap();

  setTrackerPen(QPen(Qt::red));
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
  rightAxis -> setColorMap(QwtDoubleInterval(0,1),
                           m_Spectrogram->colorMap());

//   enableAxis(QwtPlot::yRight);

  QwtDoubleInterval range = m_Raster.range();

  setAxisScale(QwtPlot::yRight, m_Raster.minValue(), m_Raster.maxValue());

//   setDisplayedRange(0,100);

  recalculateDisplayedRange();
  replot();
}

QxrdRasterData* QxrdImagePlot::raster()
{
  return &m_Raster;
}
