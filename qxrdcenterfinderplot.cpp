/******************************************************************
*
*  $Id: qxrdcenterfinderplot.cpp,v 1.9 2009/07/25 15:18:19 jennings Exp $
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
#include "qxrddataprocessor.h"
#include "qxrdcenterfinder.h"

QxrdCenterFinderPlot::QxrdCenterFinderPlot(QWidget *parent)
  : QxrdPlot(false, false, parent),
    m_Legend(NULL),
    m_DataProcessor(NULL),
    m_CenterFinder(NULL),
    m_FirstTime(true),
    SOURCE_IDENT("$Id: qxrdcenterfinderplot.cpp,v 1.9 2009/07/25 15:18:19 jennings Exp $")
{
  m_Legend = new QwtLegend();
  m_Legend -> setItemMode(QwtLegend::CheckableItem);

  insertLegend(m_Legend, QwtPlot::RightLegend);

//  autoScale();
}

void QxrdCenterFinderPlot::setDataProcessor(QxrdDataProcessor *proc)
{
  m_DataProcessor = proc;
  m_CenterFinder = m_DataProcessor -> centerFinder();
}

void QxrdCenterFinderPlot::onProcessedImageAvailable(QxrdImageData *image)
{
  onCenterChanged(m_CenterFinder -> get_CenterX(), m_CenterFinder -> get_CenterY());
}

void QxrdCenterFinderPlot::onCenterXChanged(double cx)
{
  onCenterChanged(cx, m_CenterFinder -> get_CenterY());
}

void QxrdCenterFinderPlot::onCenterYChanged(double cy)
{
  onCenterChanged(m_CenterFinder -> get_CenterX(), cy);
}

void QxrdCenterFinderPlot::onCenterChanged(double cx, double cy)
{
  QxrdImageData *img = m_DataProcessor -> data();

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

  if (m_FirstTime) {
    autoScale();
    m_FirstTime = false;
  } else {
    replot();
  }
}

/******************************************************************
*
*  $Log: qxrdcenterfinderplot.cpp,v $
*  Revision 1.9  2009/07/25 15:18:19  jennings
*  Moved graph zooming code into QxrdPlot - a common base class
*
*  Revision 1.8  2009/07/22 11:55:34  jennings
*  Center finder modifications
*
*  Revision 1.7  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

