#include "qxrdimageplot.h"
#include "qxrdrasterdata.h"

#include <qwt_plot_zoomer.h>
#include <qwt_legend.h>
#include <qwt_plot_spectrogram.h>

QxrdImagePlot::QxrdImagePlot(QWidget *parent)
  : QwtPlot(parent),
    m_Zoomer(NULL),
    m_Legend(NULL),
    m_Spectrogram(NULL),
    m_Raster(NULL),
    m_ColorMap(Qt::black, Qt::white)
{
  setCanvasBackground(QColor(Qt::white));

  m_Zoomer = new QwtPlotZoomer(QwtPlot::xBottom, QwtPlot::yLeft, canvas());
  m_Zoomer -> setSelectionFlags(QwtPicker::DragSelection | QwtPicker::CornerToCorner);
  m_Zoomer -> setTrackerMode(QwtPicker::ActiveOnly);
  m_Zoomer -> setRubberBand(QwtPicker::RectRubberBand);

  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect2,
			      Qt::LeftButton, Qt::ControlModifier | Qt::ShiftModifier);
  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect3,
			      Qt::LeftButton, Qt::ControlModifier);

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
  m_Spectrogram -> setData(data);
  m_Spectrogram -> setColorMap(m_ColorMap);
  m_Spectrogram -> invalidateCache();
  m_Spectrogram -> itemChanged();
}
