/******************************************************************
*
*  $Id: qxrdcenterfinderplot.cpp,v 1.7 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#include "qxrdcenterfinderplot.h"
#include "qxrdimagedata.h"

#include <QPen>
#include <qwt_plot_curve.h>
#include <qwt_plot_picker.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>
#include <QMetaMethod>

QxrdCenterFinderPlot::QxrdCenterFinderPlot(QWidget *parent)
  : QxrdPlot(parent),
    m_Tracker(NULL),
    m_Panner(NULL),
    m_Zoomer(NULL),
    m_Magnifier(NULL),
    m_Legend(NULL),
    SOURCE_IDENT("$Id: qxrdcenterfinderplot.cpp,v 1.7 2009/06/27 22:50:32 jennings Exp $")
{
  setCanvasBackground(QColor(Qt::white));

  m_Tracker = new QwtPlotPicker(canvas());
  m_Tracker -> setEnabled(true);
  m_Tracker -> setSelectionFlags(QwtPicker::PointSelection);
//   m_Tracker -> setAxisEnabled(QwtPlot::yRight, false);

  m_Zoomer = new QwtPlotZoomer(canvas());
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

  m_Legend = new QwtLegend();
  m_Legend -> setItemMode(QwtLegend::CheckableItem);

  insertLegend(m_Legend, QwtPlot::RightLegend);

  autoScale();
}

void QxrdCenterFinderPlot::autoScale()
{
  m_Zoomer -> zoom(0);

  setAxisAutoScale(QwtPlot::yLeft);
  setAxisAutoScale(QwtPlot::xBottom);

  replot();

  m_Zoomer -> setZoomBase();
}

void QxrdCenterFinderPlot::onCenterChanged(QxrdImageData *img, double cx, double cy)
{
  int width =img->get_Width();
  int height=img->get_Height();

  int len = (int) sqrt(pow(width,2)+pow(height,2));

  clear();

  QPen pen;

  for (double ang=0; ang<2*M_PI; ang+=M_PI/12) {
    double x = cx, y = cy;
    double dx = cos(ang);
    double dy = sin(ang);
    int nn=0;

    m_XData.resize(len);
    m_YData.resize(len);

    for (int n = 0; n<=len; n++) {
      int ix = (int)round(x);
      int iy = (int)round(y);

      if (ix >= 0 && iy >= 0 && ix < width && iy < height) {
        double v = img->value(ix,iy);
        bool mv = img->maskValue(ix,iy);

        if (mv) {
          m_XData[nn] = n;
          m_YData[nn] = v;

          nn++;
        }
      }

      x += dx;
      y += dy;
    }

    m_XData.resize(nn);
    m_YData.resize(nn);

    double angdeg = 180*(ang)/M_PI;

    QwtPlotCurve *pc = new QwtPlotCurve(QString("%1").arg(180*(ang)/M_PI));

    pc->setData(m_XData, m_YData);
//    pc->setStyle(QwtPlotCurve::Dots);
    pen.setColor(QColor::fromHsv((int) angdeg, 255, 255));

    QwtSymbol s;
    s.setStyle(QwtSymbol::Rect);
    s.setSize(3,3);
    s.setPen(pen);

    pc->setSymbol(s);
    pc->setPen(pen);
    pc->attach(this);
  }

  QString title = QString("Center:%1:").arg(img->get_Title());
  title += QString("(%1,%2):").arg(cx).arg(cy);

  setTitle(title);

  setAxisTitle(xBottom, "r");

  replot();
}

void QxrdCenterFinderPlot::doZoomIn()
{
  m_Zoomer -> zoom(1);
}

void QxrdCenterFinderPlot::doZoomOut()
{
  m_Zoomer -> zoom(-1);
}

void QxrdCenterFinderPlot::doZoomAll()
{
  m_Zoomer -> zoom(0);

  autoScale();
}

/******************************************************************
*
*  $Log: qxrdcenterfinderplot.cpp,v $
*  Revision 1.7  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

