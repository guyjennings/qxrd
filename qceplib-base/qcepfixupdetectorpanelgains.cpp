#include "qcepfixupdetectorpanelgains.h"
#include "qcepimagedata.h"
#include "levmar.h"

QcepFixupDetectorPanelGains::QcepFixupDetectorPanelGains()
  : QcepObject("fixupDetectorPanelGains")
{

}

static const int nh = 16;
static const int nv = 2;  // 16 panes across x 2 panes high
static const int magic = nh/2;
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

static double moduleGain(double *parms, int i, int j)
{
  int n=j*nh+i;

//  printf("moduleGain(%d,%d) = parms[%d]\n", i,j,n);

  if (n < 0) {
    printf("moduleGain(%d,%d) underflow [%d]", i,j,n);
  } else if (n < magic) {
    return parms[n];
  } else if (n == magic) {
    return 1.0;
  } else if (n < (nh*nv)){
    return parms[n-1];
  } else {
    printf("moduleGain(%d,%d) overflow [%d]", i,j,n);
  }

  return qQNaN();
}

static void staticFitPanelGains(double *parm, double *xv, int /*nparm*/, int nxv, void *adata)
{
  printf ("Module Gains\n");
  for (int j=0; j<nv; j++) {
    for (int i=0; i<nh; i++) {
      printf("%g\t", moduleGain(parm, i,j));
    }
    printf("\n");
  }
  printf("\n");

  QcepDoubleImageData *data = (QcepDoubleImageData*) adata;

  if (data) {
    int wd = data->get_Width();
    int ht = data->get_Height();
    int mht = ht/nv;
    int mwd = wd/nh;

    int ix=0;

    double x[bwidth], y[bwidth];

    for (int i=0; i<(nh-1); i++) {
      for (int j=0; j<nv; j++) {
        double g1 = moduleGain(parm,i,j);
        double g2 = moduleGain(parm,i+1,j);

        for (int p=0; p<mht; p++) { // Vertical boundaries...
          int c = (i+1)*mwd;
          int r = j*mht+p;

          for (int q=0; q<bwidth; q++) {
            x[q] = c-q-1;
            y[q] = data->getImageData(c-q-1, r)*g1;
          }

          double v1 = linearFit(x,y,bwidth,c-0.5);

          for (int q=0; q<bwidth; q++) {
            x[q] = c+q;
            y[q] = data->getImageData(c+q, r)*g2;
          }

          double v2 = linearFit(x,y,bwidth,c-0.5);

          if (qIsFinite(v1) && qIsFinite(v2)) {
            xv[ix++] = v1-v2;
          } else {
            xv[ix++] = 0;
          }
        }
      }
    }

    for (int i=0; i<nh; i++) {
      for (int j=0; j<(nv-1); j++) {
        double g1 = moduleGain(parm,i,j);
        double g2 = moduleGain(parm,i,j+1);

        for (int p=0; p<mwd; p++) { // Horizontal boundaries
          int c = i*mwd+p;
          int r = (j+1)*mht;

          for (int q=0; q<bwidth; q++) {
            x[q] = r-q-1;
            y[q] = data->getImageData(c, r-q-1)*g1;
          }

          double v1 = linearFit(x,y,bwidth,r-0.5);

          for (int q=0; q<bwidth; q++) {
            x[q] = r+q;
            y[q] = data->getImageData(c, r+q)*g2;
          }

          double v2 = linearFit(x,y,bwidth,r-0.5);

          if (qIsFinite(v1) && qIsFinite(v2)) {
            xv[ix++] = v1-v2;
          } else {
            xv[ix++] = 0;
          }
        }
      }
    }

    if (ix != nxv) {
      printf("ix (%d) != nxv (%d)", ix, nxv);
    }

    double sumsq = 0;

    for (int i=0; i<ix; i++) {
      sumsq += pow(xv[i],2);
    }

    printf("AvgSumsq = %g\n", sumsq/ix);
  }
}

QcepDoubleImageDataPtr QcepFixupDetectorPanelGains::exec(QcepDoubleImageDataPtr img)
{
  if (img) {
    QcepDoubleImageDataPtr res = img->duplicate();

    int wd = img->get_Width();
    int ht = img->get_Height();
    int mht = ht/nv;
    int mwd = wd/nh;

    double parms[31];
    int nParms = 31;
    int nPoints = (nv-1)*wd + (nh-1)*ht;

    for (int i=0; i<nParms; i++) {
      parms[i] = 0;
    }

    for (int i=0; i<nParms; i++) {
      parms[i] = 1;

      for (int r=0; r<nv; r++) {
        for (int c=0; c<nh; c++) {
          printf("%g\t", moduleGain(parms, c, r));
        }
        printf("\n");
      }

      printf("\n");
    }

    int nIter = dlevmar_dif(::staticFitPanelGains,
                            parms, NULL, nParms, nPoints, 100, NULL, NULL, NULL, NULL, img.data());

    if (nIter > 0) {
      for (int i=0; i<nh; i++) {
        for (int j=0; j<nv; j++) {
          double g = moduleGain(parms, i,j)/*+(i+j)%2*/;

          for (int r=0; r<mht; r++) {
            for (int c=0; c<mwd; c++) {
              double v = img->getImageData(i*mwd+c,j*mht+r)*g;

              res->setImageData(i*mwd+c, j*mht+r, v);
            }
          }
        }
      }

      return res;
    }
  }

  return QcepDoubleImageDataPtr();
}
