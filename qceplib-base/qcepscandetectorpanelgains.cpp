#include "qcepscandetectorpanelgains.h"
#include "qcepallocator.h"
#include "qcepdatacolumnscan.h"
#include "qcepimagedata.h"
#include "qcepdebug.h"

QcepScanDetectorPanelGains::QcepScanDetectorPanelGains()
  : QcepObject("scanDetectorPanelGains")
{

}

static const int nh = 16;
static const int nv = 2;  // 16 panes across x 2 panes high
//static const int magic = nh/2;
static const int bwidth = 10;

static double linearFit(double xvec[], double yvec[], int np, double xx)
{
  double sumn = 0, sumx = 0, sumxx = 0, sumy = 0, sumxy = 0;

  for (int i=0; i<np; i++) {
    double x = xvec[i];
    double y = yvec[i];

    if (qIsFinite(x) && qIsFinite(y)) {
      sumn  += 1;
      sumx  += x;
      sumxx += x*x;
      sumy  += y;
      sumxy += x*y;
    }
  }

  if (sumn >= 2) {
    double del = sumn*sumxx - sumx*sumx;
    double a   = (sumxx*sumy - sumx*sumxy)/del;
    double b   = (sumn*sumxy - sumx*sumy)/del;

    return a+b*xx;
  } else {
    return qQNaN();
  }
}

static double panelHFit(QcepDoubleImageDataPtr img, int panel, double gain)
{
  if (img) {
    int wd     = img->get_Width();
    int ht     = img->get_Height();
    int mht    = ht/nv;
    int mwd    = wd/nh;
    int panelH = panel % nh;
    int panelV = panel / nh;
    double res = 0;

    double x[bwidth], y[bwidth];

    if (panelH > 0) {
      int c = (panelH)*mwd;

      for (int p=0; p<mht; p++) {
        int r = (panelV*mht + p);

        for (int q=0; q<bwidth; q++) {
          x[q] = c-q-1;
          y[q] = img->getImageData(c-q-1, r);

          if (qcepDebug(DEBUG_PANEL_SHOW)) {
            img->setImageData(c-q-1, r, (quint64) y[q] | (1<<panel));
          }
        }

        double v1 = linearFit(x,y,bwidth,c-0.5);

        for (int q=0; q<bwidth; q++) {
          x[q] = c+q;
          y[q] = img->getImageData(c+q, r)*gain;

          if (qcepDebug(DEBUG_PANEL_SHOW)) {
            img->setImageData(c+q, r, (quint64) y[q] | (1<<panel));
          }
        }

        double v2 = linearFit(x,y,bwidth,c-0.5);

        if (qIsFinite(v1) && qIsFinite(v2)) {
          double dv = (v1-v2)/(v1+v2);

          if (qIsFinite(dv)) {
            res += dv*dv;
          }
        }
      }
    }

    if (panelH < (nh-1)) {
      int c = (panelH+1)*mwd;

      for (int p=0; p<mht; p++) {
        int r = (panelV*mht + p);

        for (int q=0; q<bwidth; q++) {
          x[q] = c-q-1;
          y[q] = img->getImageData(c-q-1, r)*gain;

          if (qcepDebug(DEBUG_PANEL_SHOW)) {
            img->setImageData(c-q-1, r, (quint64) y[q] | (1<<panel));
          }
        }

        double v1 = linearFit(x,y,bwidth,c-0.5);

        for (int q=0; q<bwidth; q++) {
          x[q] = c+q;
          y[q] = img->getImageData(c+q, r);

          if (qcepDebug(DEBUG_PANEL_SHOW)) {
            img->setImageData(c+q, r, (quint64) y[q] | (1<<panel));
          }
        }

        double v2 = linearFit(x,y,bwidth,c-0.5);

        if (qIsFinite(v1) && qIsFinite(v2)) {
          double dv = (v1-v2)/(v1+v2);

          if (qIsFinite(dv)) {
            res += dv*dv;
          }
        }
      }
    }

    return res;
  } else {
    return qQNaN();
  }
}

static double panelVFit(QcepDoubleImageDataPtr img, int panel, double gain)
{
  if (img) {
    int wd     = img->get_Width();
    int ht     = img->get_Height();
    int mht    = ht/nv;
    int mwd    = wd/nh;
    int panelH = panel % nh;
    int panelV = panel / nh;

    double res = 0;

    double x[bwidth], y[bwidth];

    if (panelV > 0) {
      int r = panelV*mht;

      for (int p=0; p<mwd; p++) {
        int c = panelH*mwd+p;

        for (int q=0; q<bwidth; q++) {
          x[q] = r-q-1;
          y[q] = img->getImageData(c, r-q-1);

          if (qcepDebug(DEBUG_PANEL_SHOW)) {
            img->setImageData(c, r-q-1, (quint64) y[q] | (1<<panel));
          }
        }

        double v1 = linearFit(x, y, bwidth, r-0.5);

        for (int q=0; q<bwidth; q++) {
          x[q] = r+q;
          y[q] = img->getImageData(c, r+q)*gain;

          if (qcepDebug(DEBUG_PANEL_SHOW)) {
            img->setImageData(c, r+q, (quint64) y[q] | (1<<panel));
          }
        }

        double v2 = linearFit(x, y, bwidth, r-0.5);

        if (qIsFinite(v1) && qIsFinite(v2)) {
          double dv = (v1-v2)/(v1+v2);

          if (qIsFinite(dv)) {
            res += dv*dv;
          }
        }
      }
    }

    if (panelV < (nv-1)) {
      int r = (panelV+1)*mht;

      for (int p=0; p<mwd; p++) {
        int c = panelH*mwd+p;

        for (int q=0; q<bwidth; q++) {
          x[q] = r-q-1;
          y[q] = img->getImageData(c, r-q-1);

          if (qcepDebug(DEBUG_PANEL_SHOW)) {
            img->setImageData(c, r-q-1, (quint64) y[q] | (1<<panel));
          }
        }

        double v1 = linearFit(x, y, bwidth, r-0.5);

        for (int q=0; q<bwidth; q++) {
          x[q] = r+q;
          y[q] = img->getImageData(c, r+q)*gain;

          if (qcepDebug(DEBUG_PANEL_SHOW)) {
            img->setImageData(c, r+q, (quint64) y[q] | (1<<panel));
          }
        }

        double v2 = linearFit(x, y, bwidth, r-0.5);

        if (qIsFinite(v1) && qIsFinite(v2)) {
          double dv = (v1-v2)/(v1+v2);

          if (qIsFinite(dv)) {
            res += dv*dv;
          }
        }
      }
    }

    return res;
  } else {
    return qQNaN();
  }
}

static double panelFit(QcepDoubleImageDataPtr img, int panel, double gain)
{
  return panelHFit(img, panel, gain) + panelVFit(img, panel, gain);
}

QcepDataColumnScanPtr QcepScanDetectorPanelGains::exec(QcepDoubleImageDataPtr img)
{
  QStringList cols;

  cols << "Gain";

  for (int i=0; i<32; i++) {
    cols << tr("Fit%1").arg(i);
    cols << tr("HFit%1").arg(i);
    cols << tr("VFit%1").arg(i);
  }

  QcepDataColumnScanPtr res = QcepAllocator::newColumnScan(tr("%1.sc").arg(img->get_Name()),
                                                           cols, 21, QcepAllocator::AlwaysAllocate);

  for (int i=0; i<21; i++) {
    double gain = 0.5+i*0.05;

    res->setValue(0, i, gain);

    for (int p=0; p<32; p++) {
      res->setValue(p*3+1, i, panelFit(img, p, gain));
      res->setValue(p*3+2, i, panelHFit(img, p, gain));
      res->setValue(p*3+3, i, panelVFit(img, p, gain));
    }
  }

  return res;
}

QcepDoubleImageDataPtr QcepScanDetectorPanelGains::hEdges(QcepDoubleImageDataPtr img)
{
  QcepDoubleImageDataPtr res = img->duplicate();

  int nr = img->get_Height();
  int nc = img->get_Width();

  double x[bwidth], y[bwidth];

  for (int r=0; r<nr; r++) {
    for (int c=0; c<nc; c++) {
      for (int q=0; q<bwidth; q++) {
        x[q] = c-q-1;
        y[q] = img->getImageData(c-q-1, r);
      }

      double v1 = linearFit(x,y,bwidth, c-0.5);

      for (int q=0; q<bwidth; q++) {
        x[q] = c+q;
        y[q] = img->getImageData(c+q, r);
      }

      double v2 = linearFit(x,y,bwidth, c-0.5);

      double dv = (v1-v2)/(v1+v2);

      res -> setImageData(c, r, dv);
    }
  }

  return res;
}

QcepDoubleImageDataPtr QcepScanDetectorPanelGains::vEdges(QcepDoubleImageDataPtr img)
{
  QcepDoubleImageDataPtr res = img->duplicate();

  int nr = img->get_Height();
  int nc = img->get_Width();

  double x[bwidth], y[bwidth];

  for (int r=0; r<nr; r++) {
    for (int c=0; c<nc; c++) {
      for (int q=0; q<bwidth; q++) {
        x[q] = r-q-1;
        y[q] = img->getImageData(c, r-q-1);
      }

      double v1 = linearFit(x,y,bwidth, r-0.5);

      for (int q=0; q<bwidth; q++) {
        x[q] = r+q;
        y[q] = img->getImageData(c, r+q);
      }

      double v2 = linearFit(x,y,bwidth, r-0.5);

      double dv = (v1-v2)/(v1+v2);

      res -> setImageData(c, r, dv);
    }
  }

  return res;
}
