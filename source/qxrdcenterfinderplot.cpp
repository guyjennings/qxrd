#include "qxrdcenterfinderplot.h"
#include "qcepimagedata.h"
#include "qcepmaskdata.h"

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
#include "qcepplotmeasurer.h"
#include "qwt_plot_piecewise_curve.h"
#include "qxrdapplication.h"

QxrdCenterFinderPlot::QxrdCenterFinderPlot(QWidget *parent)
  : QcepPlot(parent),
    m_ObjectNamer(this, "centeringGraph"),
    m_Window(),
    m_DataProcessor(),
    m_CenterFinder(),
    m_FirstTime(true)
{
}

void QxrdCenterFinderPlot::init(QcepPlotSettingsWPtr settings)
{
  QcepPlot::init(settings);

  if (m_Legend) {
    insertLegend(m_Legend, QwtPlot::RightLegend);
  }
}

void QxrdCenterFinderPlot::setWindow(QxrdWindow *win)
{
  m_Window = win;

  QxrdWindow *wp = m_Window;

  if (wp) {
    m_DataProcessor = wp -> dataProcessor();
  }

  QxrdDataProcessorPtr dp(m_DataProcessor);

  if (dp) {
    m_CenterFinder = dp -> centerFinder();

    connect(m_Measurer, (void (QcepPlotMeasurer::*)( const QVector<QPointF> &)) &QcepPlotMeasurer::selected,
            dp.data(),  &QxrdDataProcessor::printMeasuredPolygon);

    QxrdCenterFinderPtr cf(m_CenterFinder);

    if (cf) {
      connect(cf.data(), &QxrdCenterFinder::parameterChanged, this, &QxrdCenterFinderPlot::onParameterChanged);
    }
  }
}

void QxrdCenterFinderPlot::onParameterChanged()
{
  QxrdCenterFinderPtr cf(m_CenterFinder);

  if (cf) {
    onCenterChanged(cf -> get_CenterX(), cf -> get_CenterY());
  }
}

void QxrdCenterFinderPlot::onProcessedImageAvailable(QcepDoubleImageDataPtr /*image*/)
{
  QxrdCenterFinderPtr cf(m_CenterFinder);

  if (cf) {
    onCenterChanged(cf -> get_CenterX(), cf -> get_CenterY());
  }
}

void QxrdCenterFinderPlot::onMaskedImageAvailable(QcepDoubleImageDataPtr /*image*/, QcepMaskDataPtr /*mask*/)
{
  QxrdCenterFinderPtr cf(m_CenterFinder);

  if (cf) {
    onCenterChanged(cf -> get_CenterX(), cf -> get_CenterY());
  }
}

void QxrdCenterFinderPlot::onCenterXChanged(double cx)
{
  QxrdCenterFinderPtr cf(m_CenterFinder);

  if (cf) {
    onCenterChanged(cx, cf -> get_CenterY());
  }
}

void QxrdCenterFinderPlot::onCenterYChanged(double cy)
{
  QxrdCenterFinderPtr cf(m_CenterFinder);

  if (cf) {
    onCenterChanged(cf -> get_CenterX(), cy);
  }
}

void QxrdCenterFinderPlot::onCenterChanged(QPointF c)
{
  onCenterChanged(c.x(), c.y());
}

void QxrdCenterFinderPlot::onCenterChanged(double cx, double cy)
{
  QxrdWindow *wp = m_Window;
  QxrdCenterFinderPtr cf(m_CenterFinder);

  if (wp && cf) {
    try {
      QcepDoubleImageDataPtr img = wp -> data();
      QcepMaskDataPtr mask = wp -> mask();

      if (img /* && mask*/) {
        int width =img->get_Width();
        int height=img->get_Height();

        int len = (int) sqrt((double)(width*width+height*height));

        detachItems(QwtPlotItem::Rtti_PlotCurve);
        detachItems(QwtPlotItem::Rtti_PlotMarker);

        QPen pen;

        for (double ang=0; ang<2*M_PI; ang+=M_PI/36) {
          double x = cx, y = cy;
          double dx = cos(ang);
          double dy = sin(ang);
          int nn=0;

          double distance = cf->get_DetectorDistance();

          if (distance <= 0) distance = 1000;

          double plx = cf->get_DetectorXPixelSize();
          double ply = cf->get_DetectorYPixelSize();

          m_XData.resize(len);
          m_YData.resize(len);

          if (cf->get_ImplementTilt()) {
            double rot = cf->get_TiltPlaneRotation()*M_PI/180.0;
            double sinrot = sin(rot);
            double cosrot = cos(rot);
            double beta = cf->get_DetectorTilt()*M_PI/180.0;
            double sinbeta = sin(beta);
            double cosbeta = cos(beta);

            for (int n = 0; n<=len; n++) {
              double twoTheta = cf->getTwoTheta(cx,cy,distance,x,y,plx,ply,cosbeta,sinbeta,cosrot,sinrot);

              int ix = (int)qRound(x);
              int iy = (int)qRound(y);

              if (ix >= 0 && iy >= 0 && ix < width && iy < height) {
                double v = img->value(ix,iy);

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
                double v = img->value(ix,iy);

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

          pc->setSamples(m_XData, m_YData);
          //    pc->setStyle(QwtPlotCurve::Dots);
          pen.setColor(QColor::fromHsv((int) angdeg, 255, 255));

          QwtSymbol *s = new QwtSymbol();

          s->setStyle(QwtSymbol::Rect);
          s->setSize(3,3);
          s->setPen(pen);

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
      if (g_Application) {
        g_Application->printMessage("QxrdCenterFinderPlot::onCenterChanged failed");
      }
    }
  }
}
