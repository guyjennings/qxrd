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
#include "qwt_plot_piecewise_curve.h"
#include "qxrdapplication.h"
#include <QtConcurrentRun>
#include "qxrdplotcurvevector.h"

QxrdCenterFinderPlot::QxrdCenterFinderPlot(QWidget *parent)
  : QxrdPlot(parent),
    m_ObjectNamer(this, "centeringGraph"),
    m_Window(),
    m_DataProcessor(),
    m_CenterFinder(),
    m_FirstTime(true)
{
  qRegisterMetaType<QxrdPlotCurveVectorPtr>("QxrdPlotCurveVectorPtr");

  connect(this, &QxrdCenterFinderPlot::newCenterFinderCurves,
          this, &QxrdCenterFinderPlot::onNewCenterFinderCurves,
          Qt::QueuedConnection);
}

void QxrdCenterFinderPlot::init(QxrdPlotSettingsWPtr settings)
{
  QxrdPlot::init(settings);

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

    connect(m_Measurer, SIGNAL(selected(QVector<QPointF>)),
            dp.data(), SLOT(printMeasuredPolygon(QVector<QPointF>)));

    QxrdCenterFinderPtr cf(m_CenterFinder);

    if (cf) {
      connect(cf.data(), SIGNAL(parameterChanged()), this, SLOT(onParameterChanged()));
    }
  }
}

void QxrdCenterFinderPlot::onParameterChanged()
{
  QtConcurrent::run(this, &QxrdCenterFinderPlot::updateCenterFinderPlot);
}

void QxrdCenterFinderPlot::onProcessedImageAvailable(QxrdDoubleImageDataPtr image)
{
  m_Image = image;

  QtConcurrent::run(this, &QxrdCenterFinderPlot::updateCenterFinderPlot);
}

void QxrdCenterFinderPlot::onMaskedImageAvailable(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr mask)
{
  m_Image = image;
  m_Mask  = mask;

  QtConcurrent::run(this, &QxrdCenterFinderPlot::updateCenterFinderPlot);
}

void QxrdCenterFinderPlot::onCenterXChanged(double cx)
{
  QtConcurrent::run(this, &QxrdCenterFinderPlot::updateCenterFinderPlot);
}

void QxrdCenterFinderPlot::onCenterYChanged(double cy)
{
  QtConcurrent::run(this, &QxrdCenterFinderPlot::updateCenterFinderPlot);
}

void QxrdCenterFinderPlot::onCenterChanged(QPointF c)
{
  QtConcurrent::run(this, &QxrdCenterFinderPlot::updateCenterFinderPlot);
}

void QxrdCenterFinderPlot::onCenterChanged(double cx, double cy)
{
  QtConcurrent::run(this, &QxrdCenterFinderPlot::updateCenterFinderPlot);
}

void QxrdCenterFinderPlot::updateCenterFinderPlot()
{
  QxrdPlotCurveVectorPtr res =
      QxrdPlotCurveVectorPtr(new QxrdPlotCurveVector());

  QxrdCenterFinderPtr cf(m_CenterFinder);
  if (cf) {
    try {
      if (m_Image /* && m_Mask*/) {
        int width =m_Image->get_Width();
        int height=m_Image->get_Height();

        int len = (int) sqrt((double)(width*width+height*height));

        QPen pen;

        double plx = cf->get_DetectorXPixelSize();
        double ply = cf->get_DetectorYPixelSize();

        double cx  = cf->get_CenterX(), cy = cf->get_CenterY();
        for (double ang=0; ang<2*M_PI; ang+=M_PI/36) {
          double x  = cx, y = cy;
          double dx = cos(ang);
          double dy = sin(ang);
          int nn=0;

          double distance = cf->get_DetectorDistance();

          if (distance <= 0) distance = 1000;


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
                double v = m_Image->value(x,y);

                bool mv = true;

                if (m_Mask) {
                  mv = m_Mask->maskValue(ix,iy);
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
                double v = m_Image->value(x,y);

                bool mv = true;

                if (m_Mask) {
                  mv = m_Mask->maskValue(ix,iy);
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

          QwtPlotPiecewiseCurve *pc = new QwtPlotPiecewiseCurve(this,QString("%1").arg(180*(ang)/M_PI));

          pc->setSamples(m_XData, m_YData);
          //    pc->setStyle(QwtPlotCurve::Dots);
          pen.setColor(QColor::fromHsv((int) angdeg, 255, 255));

          QwtSymbol *s = new QwtSymbol();

          s->setStyle(QwtSymbol::Rect);
          s->setSize(3,3);
          s->setPen(pen);

          pc->setSymbol(s);
          pc->setPen(pen);

          res->append(pc);
        }

        emit newCenterFinderCurves(res);
      }
    }

    catch(...) {
      if (g_Application) {
        g_Application->printMessage("QxrdCenterFinderPlot::onCenterChanged failed");
      }
    }
  }
}

void QxrdCenterFinderPlot::onNewCenterFinderCurves(QxrdPlotCurveVectorPtr curves)
{
  if (g_Application && curves) {
    g_Application->printMessage(tr("QxrdCenterFinderPlot::onNewCenterFinderCurves added %1 curves")
                                .arg(curves->count()));
  }

  m_Curves = curves;

  detachItems();

  if (m_Curves) {
    for (int i=0; i<m_Curves->count(); i++) {
      QwtPlotPiecewiseCurve *c = m_Curves->value(i);

      if (c) {
        c -> attach(this);
      }
    }
  }

  QxrdCenterFinderPtr cf(m_CenterFinder);

  if (m_Image && cf) {
    QString title = QString("Center:%1:").arg(m_Image->get_Title());
    title += QString("(%1,%2):").arg(cf->get_CenterX()).arg(cf->get_CenterY());

    setTitle(title);
  }

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
