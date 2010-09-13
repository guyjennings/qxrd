/******************************************************************
*
*  $Id: qxrdcenterfinderplot.cpp,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

#include "qxrdcenterfinderplot.h"
#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"

#include <QPen>
#include <qwt_plot_curve.h>
#include <qwt_plot_picker.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>
#include <QMetaMethod>
#include "qxrdwindow.h"
#include "qxrddataprocessor.h"
#include "qxrdcenterfinder.h"
#include "qxrdplotmeasurer.h"
#include "qwt_double_rect.h"

QxrdCenterFinderPlot::QxrdCenterFinderPlot(QWidget *parent)
  : QxrdPlot(parent),
    m_Window(NULL),
    m_DataProcessor(NULL),
    m_CenterFinder(NULL),
    m_FirstTime(true),
    SOURCE_IDENT("$Id: qxrdcenterfinderplot.cpp,v 1.2 2010/09/13 20:00:39 jennings Exp $")
{
  insertLegend(m_Legend, QwtPlot::RightLegend);
}

void QxrdCenterFinderPlot::setWindow(QxrdWindowPtr win)
{
  m_Window = win;
  m_DataProcessor = m_Window -> dataProcessor();
  m_CenterFinder = m_DataProcessor -> centerFinder();

  connect(m_Measurer, SIGNAL(selected(QwtArray<QwtDoublePoint>)),
          m_DataProcessor, SLOT(printMeasuredPolygon(QwtArray<QwtDoublePoint>)));

  connect(m_CenterFinder, SIGNAL(parameterChanged()), this, SLOT(onParameterChanged()));
}

void QxrdCenterFinderPlot::onParameterChanged()
{
  onCenterChanged(m_CenterFinder -> get_CenterX(), m_CenterFinder -> get_CenterY());
}

void QxrdCenterFinderPlot::onProcessedImageAvailable(QxrdDoubleImageDataPtr /*image*/)
{
  onCenterChanged(m_CenterFinder -> get_CenterX(), m_CenterFinder -> get_CenterY());
}

void QxrdCenterFinderPlot::onMaskedImageAvailable(QxrdDoubleImageDataPtr /*image*/, QxrdMaskDataPtr /*mask*/)
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

void QxrdCenterFinderPlot::onCenterChanged(QwtDoublePoint c)
{
  onCenterChanged(c.x(), c.y());
}

void QxrdCenterFinderPlot::onCenterChanged(double cx, double cy)
{
  try {
    QxrdDoubleImageDataPtr img = m_Window -> data();
    QxrdMaskDataPtr mask = m_Window -> mask();

    if (img && mask) {
      int width =img->get_Width();
      int height=img->get_Height();

      int len = (int) sqrt((double)(width*width+height*height));

      clear();

      QPen pen;

      for (double ang=0; ang<2*M_PI; ang+=M_PI/12) {
        double x = cx, y = cy;
        double dx = cos(ang);
        double dy = sin(ang);
        int nn=0;

        double distance = m_CenterFinder->get_DetectorDistance();

        if (distance <= 0) distance = 1000;

        double plx = m_CenterFinder->get_DetectorXPixelSize();
        double ply = m_CenterFinder->get_DetectorYPixelSize();

        m_XData.resize(len);
        m_YData.resize(len);

        if (m_CenterFinder->get_ImplementTilt()) {
          double rot = m_CenterFinder->get_TiltPlaneRotation()*M_PI/180.0;
          double sinrot = sin(rot);
          double cosrot = cos(rot);
          double beta = m_CenterFinder->get_DetectorTilt()*M_PI/180.0;
          double sinbeta = sin(beta);
          double cosbeta = cos(beta);

          for (int n = 0; n<=len; n++) {
            double twoTheta = m_CenterFinder->getTwoTheta(cx,cy,distance,x,y,plx,ply,cosbeta,sinbeta,cosrot,sinrot);

            int ix = (int)qRound(x);
            int iy = (int)qRound(y);

            if (ix >= 0 && iy >= 0 && ix < width && iy < height) {
              double v = img->value(x,y);

              bool mv = true;

              if (mask) {
                mv = mask->maskValue(ix,iy);
              }

              if (mv) {
                m_XData[nn] = twoTheta;
                m_YData[nn] = v;

                nn++;
              }
            }

            x += dx;
            y += dy;
          }
        } else {
          for (int n = 0; n<=len; n++) {
            double delx = (x - cx)*plx/1000.0;
            double dely = (y - cy)*ply/1000.0;

            double radius = sqrt(delx*delx + dely*dely);
            double twoTheta = atan2(radius, distance)*180.0/M_PI;

            int ix = (int)qRound(x);
            int iy = (int)qRound(y);

            if (ix >= 0 && iy >= 0 && ix < width && iy < height) {
              double v = img->value(x,y);

              bool mv = true;

              if (mask) {
                mv = mask->maskValue(ix,iy);
              }

              if (mv) {
                m_XData[nn] = twoTheta;
                m_YData[nn] = v;

                nn++;
              }
            }

            x += dx;
            y += dy;
          }
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

      setAxisTitle(xBottom, "2Theta (deg)");

      if (m_Zoomer && m_Zoomer -> zoomRectIndex() == 0) {
        m_Zoomer -> setZoomBase();
      }

      if (m_FirstTime) {
        autoScale();
        m_FirstTime = false;
      } else {
        replot();
      }
    }
  }

  catch(...) {
    printf("QxrdCenterFinderPlot::onCenterChanged failed\n");
  }
}

/******************************************************************
*
*  $Log: qxrdcenterfinderplot.cpp,v $
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.3  2010/09/10 18:54:10  jennings
*  Partial implement tilt in center finder dialog
*
*  Revision 1.1.2.2  2010/08/11 15:58:27  jennings
*  Added changedValue(QwtDoublePoint) slots
*
*  Revision 1.1.2.1  2010/07/22 18:39:38  jennings
*  Moving files into source subdirectory
*
*  Revision 1.18.2.7  2010/06/23 21:47:26  jennings
*  Eliminated crashing problem if image/mask was NULL
*
*  Revision 1.18.2.6  2010/05/25 20:43:37  jennings
*  Added try catch blocks around various memory allocations
*
*  Revision 1.18.2.5  2010/05/02 16:56:50  jennings
*  Removed embedded C comments from commit log
*
*  Revision 1.18.2.4  2010/05/02 08:12:06  jennings
*  Replaced 'returnImageToPool' and 'replaceImageFromPool' by
*  equivalent smart pointer assignments
*
*  Revision 1.18.2.3  2010/04/26 23:46:13  jennings
*  *** empty log message ***
*
*  Revision 1.18.2.2  2010/04/26 20:53:25  jennings
*  More attempts to get QSharedPointers to work...
*
*  Revision 1.18.2.1  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.18  2009/12/01 14:10:37  jennings
*  Added interpolating T value(double x, double y) method to QcepImageData
*  Changed center finder plot to use interpolating value routine
*
*  Revision 1.17  2009/11/12 20:17:43  jennings
*  Version 0.3.5, fix problems with first-time zooming of graphs
*
*  Revision 1.16  2009/11/02 20:16:22  jennings
*  Changes to make it work with VC compiler
*
*  Revision 1.15  2009/09/25 14:22:16  jennings
*  Simplified double-buffering for plotted data - there is now a separate copy of data and mask
*  in QxrdWindow
*
*  Revision 1.14  2009/09/07 22:03:46  jennings
*  Allow NULL mask
*
*  Revision 1.13  2009/08/25 18:43:03  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.12  2009/08/12 19:44:58  jennings
*  Reorganized plot zoomers into a single class, initialized in QxrdPlot, which
*  takes its tracker text from a QxrdPlot virtual member function
*
*  Revision 1.11  2009/08/09 15:39:10  jennings
*  Added a separate QxrdImagePlotMeasurer class
*
*  Revision 1.10  2009/08/04 16:45:20  jennings
*  Moved mask data into separate class
*
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

