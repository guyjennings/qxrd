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
#include "qwt_plot_piecewise_curve.h"

QxrdCenterFinderPlot::QxrdCenterFinderPlot(QWidget *parent)
  : QxrdPlot(parent),
    m_Window(NULL),
    m_DataProcessor(NULL),
    m_CenterFinder(NULL),
    m_FirstTime(true)
{
  insertLegend(m_Legend, QwtPlot::RightLegend);
}

void QxrdCenterFinderPlot::setWindow(QxrdWindow *win)
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

        QwtPlotCurve *pc = new QwtPlotPiecewiseCurve(this,QString("%1").arg(180*(ang)/M_PI));

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
