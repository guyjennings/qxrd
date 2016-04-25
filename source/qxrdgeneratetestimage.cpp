#include "qxrdgeneratetestimage.h"

#define _USE_MATH_DEFINES

#include <cmath>

#include "qxrddataprocessor.h"
#include "qxrddetectorgeometry.h"
#include "qcepallocator.h"
#include "qcepsettingssaver.h"

QxrdGenerateTestImage::QxrdGenerateTestImage(QcepObjectWPtr parent, QcepSettingsSaverWPtr saver) :
  QcepObject("testImage", parent),
  m_Processor(),
  m_Geometry(new QxrdDetectorGeometry("testGeometry", parent)),
  m_NRows(saver, this, "nRows", 2048, "Number of Rows"),
  m_NCols(saver, this, "nCols", 2048, "Number of Cols"),
  m_CenterX(saver, this, "centerX", 1024, "X Center"),
  m_CenterY(saver, this, "centerY", 1024, "Y Center"),
  m_Distance(saver, this, "distance", 1000, "Detector - Sample Distance (in mm)"),
  m_Energy(saver, this, "energy", 22000, "Beam Energy (in eV)"),
  m_PixelWidth(saver, this, "pixelWidth", 10, "Pixel Width (in um)"),
  m_PixelHeight(saver, this, "pixelHeight", 10, "Pixel Height (in um)"),
  m_Alpha(saver, this, "alpha", 0, "Alpha"),
  m_Beta(saver, this, "beta", 0, "Beta"),
  m_Gamma(saver, this, "gamma", 0, "Gamma"),
  m_ChiMin(saver, this, "chiMin", 0, "Chi Min"),
  m_ChiMax(saver, this, "chiMax", 360, "Chi Max"),
  m_RingTTH(saver, this, "ringTTH", QcepDoubleList(), "TTH values of rings"),
  m_RingIntensity(saver, this, "ringIntensity", QcepDoubleList(), "Intensities of rings"),
  m_RingWidth(saver, this, "ringWidth", QcepDoubleList(), "Widths of Rings")
{
}

void QxrdGenerateTestImage::setProcessor(QxrdDataProcessorBaseWPtr proc)
{
  m_Processor = proc;
}

void QxrdGenerateTestImage::setDimension(int nc, int nr)
{
  set_NRows(nr);
  set_NCols(nc);
}

void QxrdGenerateTestImage::setCenter(double cx, double cy)
{
  set_CenterX(cx);
  set_CenterY(cy);
}

void QxrdGenerateTestImage::setDistance(double l, double pw, double ph)
{
  set_Distance(l);
  set_PixelWidth(pw);
  set_PixelHeight(ph);
}

void QxrdGenerateTestImage::setEnergy(double energy)
{
  set_Energy(energy);
}

void QxrdGenerateTestImage::setTiltAngles(double alpha, double beta, double gamma)
{
  set_Alpha(alpha);
  set_Beta(beta);
  set_Gamma(gamma);
}

void QxrdGenerateTestImage::setChiRange(double chiMin, double chiMax)
{
  set_ChiMin(chiMin);
  set_ChiMax(chiMax);
}

void QxrdGenerateTestImage::clearRings()
{
  m_RingTTH.clear();
  m_RingIntensity.clear();
  m_RingWidth.clear();
}

void QxrdGenerateTestImage::appendRing(double tth, double intensity, double width)
{
  m_RingTTH.appendValue(tth);
  m_RingIntensity.appendValue(intensity);
  m_RingWidth.appendValue(width);
}

void QxrdGenerateTestImage::generateImage()
{
  QcepDoubleImageDataPtr img =
      QcepAllocator::newDoubleImage("testImage", get_NCols(), get_NRows(), QcepAllocator::NullIfNotAvailable);

  if (img) {
    img->clear();

    int col, row, ncols = get_NCols(), nrows = get_NRows();

    double xc = get_CenterX();
    double yc = get_CenterY();
    double distance = get_Distance();
    double pixWidth = get_PixelWidth();
    double pixHeight = get_PixelHeight();
    double alpha = get_Alpha();
    double cos_alpha = cos(alpha*M_PI/180.0);
    double sin_alpha = sin(alpha*M_PI/180.0);
    double beta = get_Beta();
    double cos_beta = cos(beta*M_PI/180.0);
    double sin_beta = sin(beta*M_PI/180.0);
    double rotation = get_Gamma();
    double cos_rotation = cos(rotation*M_PI/180.0);
    double sin_rotation = sin(rotation*M_PI/180.0);

    double twoTheta, chi;
    double chiMin = get_ChiMin();
    double chiMax = get_ChiMax();

    int nrings = get_RingTTH().length();

    for (row = 0; row < nrows; row++) {
      for (col = 0; col < ncols; col++) {
        m_Geometry -> getTwoThetaChi(xc,yc,distance,col,row,
                                     pixWidth, pixHeight,
                                     rotation, cos_beta, sin_beta,
                                     cos_alpha, sin_alpha,
                                     cos_rotation, sin_rotation,
                                     &twoTheta, &chi);

        if (chiMin <= chi && chi <= chiMax) {
          double sum = 0;

          for (int i = 0; i<nrings; i++) {
            double ringTTH = get_RingTTH()[i];
            double ringInt = get_RingIntensity()[i];
            double ringWdt = get_RingWidth()[i];

            double nsigma = fabs((ringTTH-twoTheta)/ringWdt);

            if (nsigma < 5) {
              sum += ringInt*exp(-nsigma*nsigma);
            }
          }

          img->setValue(col,row,sum);
        }
      }
    }

    QxrdDataProcessorBasePtr proc(m_Processor);

    if (proc) {
      proc -> newData(img, QcepMaskDataPtr());
    }
  }
}

void QxrdGenerateTestImage::generateTTHImage()
{
  QcepDoubleImageDataPtr img = QcepAllocator::newDoubleImage("testImage", get_NCols(), get_NRows(), QcepAllocator::NullIfNotAvailable);

  if (img) {
  img->clear();

  int col, row, ncols = get_NCols(), nrows = get_NRows();

  double xc = get_CenterX();
  double yc = get_CenterY();
  double distance = get_Distance();
  double pixWidth = get_PixelWidth();
  double pixHeight = get_PixelHeight();
  double alpha = get_Alpha();
  double cos_alpha = cos(alpha*M_PI/180.0);
  double sin_alpha = sin(alpha*M_PI/180.0);
  double beta = get_Beta();
  double cos_beta = cos(beta*M_PI/180.0);
  double sin_beta = sin(beta*M_PI/180.0);
  double rotation = get_Gamma();
  double cos_rotation = cos(rotation*M_PI/180.0);
  double sin_rotation = sin(rotation*M_PI/180.0);

  double twoTheta, chi;

  for (row = 0; row < nrows; row++) {
    for (col = 0; col < ncols; col++) {
      m_Geometry -> getTwoThetaChi(xc,yc,distance,col,row,
                                   pixWidth, pixHeight,
                                   rotation, cos_beta, sin_beta,
                                   cos_alpha, sin_alpha,
                                   cos_rotation, sin_rotation,
                                   &twoTheta, &chi);

      img->setValue(col,row,twoTheta);
    }
  }

  QxrdDataProcessorBasePtr proc(m_Processor);

  if (proc) {
    proc -> newData(img, QcepMaskDataPtr());
  }
  }
}

void QxrdGenerateTestImage::generateChiImage()
{
  QcepDoubleImageDataPtr img = QcepAllocator::newDoubleImage("testImage", get_NCols(), get_NRows(), QcepAllocator::NullIfNotAvailable);

  if (img) {
    img->clear();

    int col, row, ncols = get_NCols(), nrows = get_NRows();

    double xc = get_CenterX();
    double yc = get_CenterY();
    double distance = get_Distance();
    double pixWidth = get_PixelWidth();
    double pixHeight = get_PixelHeight();
    double alpha = get_Alpha();
    double cos_alpha = cos(alpha*M_PI/180.0);
    double sin_alpha = sin(alpha*M_PI/180.0);
    double beta = get_Beta();
    double cos_beta = cos(beta*M_PI/180.0);
    double sin_beta = sin(beta*M_PI/180.0);
    double rotation = get_Gamma();
    double cos_rotation = cos(rotation*M_PI/180.0);
    double sin_rotation = sin(rotation*M_PI/180.0);

    double twoTheta, chi;

    for (row = 0; row < nrows; row++) {
      for (col = 0; col < ncols; col++) {
        m_Geometry -> getTwoThetaChi(xc,yc,distance,col,row,
                                     pixWidth, pixHeight,
                                     rotation, cos_beta, sin_beta,
                                     cos_alpha, sin_alpha,
                                     cos_rotation, sin_rotation,
                                     &twoTheta, &chi);

        img->setValue(col,row,chi);
      }
    }

    QxrdDataProcessorBasePtr proc(m_Processor);

    if (proc) {
      proc -> newData(img, QcepMaskDataPtr());
    }
  }
}
