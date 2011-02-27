#include "qxrdslicedialog.h"
#include "ui_qxrdslicedialog.h"
#include "qwt_plot_piecewise_curve.h"

QxrdSliceDialog::QxrdSliceDialog(QWidget *parent) :
  QDockWidget(parent)
{
  setupUi(this);
}

void QxrdSliceDialog::readSettings(QxrdSettings &settings, QString section)
{
  m_SlicePlot->readSettings(settings, section+"/plot");

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdSliceDialog::writeSettings(QxrdSettings &settings, QString section)
{
  m_SlicePlot->writeSettings(settings, section+"/plot");

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdSliceDialog::onProcessedImageAvailable(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow)
{
  m_Image = image;

  reslice();
}

void QxrdSliceDialog::slicePolygon(QwtArray<QwtDoublePoint> poly)
{
  m_Polygon = poly;

  reslice();
}

void QxrdSliceDialog::reslice()
{
  m_SlicePlot->clear();

  if (m_Image) {
    QVector<double> xp,yp;

    double length = 0;

    if (m_Polygon.size() >= 2) {
      QwtDoublePoint p0 = m_Polygon[0];

      for (int i=1; i<m_Polygon.size(); i++) {
        QwtDoublePoint p1 = m_Polygon[i];
        double dx = p1.x() - p0.x();
        double dy = p1.y() - p0.y();
        length += sqrt(dx*dx + dy*dy);
        p0=p1;
      }

      p0 = m_Polygon[0];
      double r = 0;
      double r0 = 0;

      for (int i=1; i<m_Polygon.size(); i++) {
        QwtDoublePoint p1 = m_Polygon[i];
        double dx = p1.x() - p0.x();
        double dy = p1.y() - p0.y();
        double len = sqrt(dx*dx + dy*dy);

        if (len > 0) {
          for (; r<len; r+=1) {
            double x = p0.x() + r*dx/len;
            double y = p0.y() + r*dy/len;

            xp.append(r+r0);
            yp.append(m_Image->value((int) x, (int) y));
          }
        }

        p0 = p1;
        r0 += len;
        r  -= len;
      }

      QwtPlotCurve *pc = new QwtPlotPiecewiseCurve(m_SlicePlot, m_Image->get_Title());

      pc->setData(xp, yp);

      pc->attach(m_SlicePlot);

      m_SlicePlot->replot();
    }
  }
}
