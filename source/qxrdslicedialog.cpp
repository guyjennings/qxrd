#include "qxrdslicedialog.h"
#include "ui_qxrdslicedialog.h"
#include "qwt_plot_piecewise_curve.h"
#include <QSettings>
#include "qxrdsettingssaver.h"
#include "qxrddebug.h"

QxrdSliceDialog::QxrdSliceDialog(QxrdSliceDialogSettingsWPtr settings, QWidget *parent) :
  QDockWidget(parent),
  m_SliceDialogSettings(settings)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdSliceDialog::QxrdSliceDialog(%p)\n", this);
  }

  setupUi(this);
}

QxrdSliceDialog::~QxrdSliceDialog()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdSliceDialog::~QxrdSliceDialog(%p)\n", this);
  }
}

void QxrdSliceDialog::onProcessedImageAvailable(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr /*overflow*/)
{
  m_Image = image;

  reslice();
}

void QxrdSliceDialog::slicePolygon(QwtArray<QwtDoublePoint> poly)
{
  QxrdSliceDialogSettingsPtr set(m_SliceDialogSettings);

  if (set) {
    set->set_SlicePolygon(poly);
  }

  reslice();
}

void QxrdSliceDialog::reslice()
{
  m_SlicePlot->clear();

  QxrdSliceDialogSettingsPtr set(m_SliceDialogSettings);

  if (set && m_Image) {
    QVector<double> xp,yp;
    QString title = m_Image->get_Title();

    double length = 0;

    QcepPolygon poly = set->get_SlicePolygon();

    if (poly.size() >= 2) {
      QwtDoublePoint p0 = poly[0];

      for (int i=1; i<poly.size(); i++) {
        QwtDoublePoint p1 = poly[i];
        double dx = p1.x() - p0.x();
        double dy = p1.y() - p0.y();
        length += sqrt(dx*dx + dy*dy);
        p0=p1;
      }

      p0 = poly[0];
      double r = 0;
      double r0 = 0;

      title += tr(" [%1,%2]").arg(p0.x()).arg(p0.y());

      for (int i=1; i<poly.size(); i++) {
        QwtDoublePoint p1 = poly[i];
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

      if (poly.size() > 2) {
        title += "...";
      } else {
        title += "-";
      }

      QwtDoublePoint p1 = poly.last();
      title += tr("[%1,%2]").arg(p1.x()).arg(p1.y());

      QwtPlotCurve *pc = new QwtPlotPiecewiseCurve(m_SlicePlot, title);

      pc->setData(xp, yp);

      pc->attach(m_SlicePlot);

      m_SlicePlot->updateZoomer();
      m_SlicePlot->replot();
    }
  }
}
