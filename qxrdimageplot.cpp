#include "qxrdimageplot.h"
#include "qxrdrasterdata.h"
#include "qxrdplotzoomer.h"

#include <qwt_plot_zoomer.h>
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

  m_Zoomer = new QxrdPlotZoomer(canvas(), this);
  m_Zoomer -> setSelectionFlags(QwtPicker::DragSelection | QwtPicker::CornerToCorner);
  m_Zoomer -> setTrackerMode(QwtPicker::AlwaysOn);
  m_Zoomer -> setRubberBand(QwtPicker::RectRubberBand);

  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect2,
			      Qt::LeftButton, Qt::ControlModifier | Qt::ShiftModifier);
  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect3,
			      Qt::LeftButton, Qt::ControlModifier);

  m_Zoomer -> setEnabled(true);

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
