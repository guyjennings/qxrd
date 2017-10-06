#include "qxrddebug.h"
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

void QxrdCenterFinderPlot::onMaskedImageAvailable(QxrdMaskDataPtr mask)
{
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
  if (g_Application && qcepDebug(DEBUG_DISPLAY)) {
    QObject *obj = sender();
    g_Application->printMessage(tr("QxrdCenterFinderPlot::updateCenterFinderPlot called by %1")
                                .arg(obj?obj->objectName():"NULL"));
  }


  QxrdPlotCurveVectorPtr res =
      QxrdPlotCurveVectorPtr(new QxrdPlotCurveVector());

  QxrdCenterFinderPtr cf(m_CenterFinder);

  QxrdDoubleImageDataPtr image = m_Image;
  QxrdMaskDataPtr        mask  = m_Mask;

  if (cf) {
    try {
      if (image /* && m_Mask*/) {
        int width =image->get_Width();
        int height=image->get_Height();

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

          QVector<double> xData(len);
          QVector<double> yData(len);

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
                double v = image->value(x,y);

                bool mv = true;

                if (mask) {
                  mv = mask->maskValue(ix,iy);
                }

                if (mv) {
                  xData[nn] = twoTheta;
                  yData[nn] = v;

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
                double v = image->value(x,y);

                bool mv = true;

                if (mask) {
                  mv = mask->maskValue(ix,iy);
                }

                if (mv) {
                  xData[nn] = twoTheta;
                  yData[nn] = v;

                  nn++;
                }
              }

              x += dx;
              y += dy;
            }
          }

          xData.resize(nn);
          yData.resize(nn);

          double angdeg = 180*(ang)/M_PI;

          QwtPlotPiecewiseCurve *pc = new QwtPlotPiecewiseCurve(this,QString("%1").arg(180*(ang)/M_PI));

          pc->setSamples(xData, yData);
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
  if (g_Application && qcepDebug(DEBUG_DISPLAY) && curves ) {
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
