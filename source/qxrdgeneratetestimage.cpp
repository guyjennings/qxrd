#include "qxrdgeneratetestimage.h"

#define _USE_MATH_DEFINES

#include <cmath>

QxrdGenerateTestImage::QxrdGenerateTestImage(QxrdSettingsSaver *saver, QxrdDataProcessorBase *proc, QxrdAllocatorPtr alloc, QObject *parent) :
    QObject(parent),
    m_Processor(proc),
    m_Allocator(alloc),
    m_Geometry(new QxrdDetectorGeometry(this)),
    m_NRows(saver, this, "nRows", 2048),
    m_NCols(saver, this, "nCols", 2048),
    m_CenterX(saver, this, "centerX", 1024),
    m_CenterY(saver, this, "centerY", 1024),
    m_Distance(saver, this, "distance", 1000),
    m_Energy(saver, this, "energy", 22000),
    m_PixelWidth(saver, this, "pixelWidth", 10),
    m_PixelHeight(saver, this, "pixelHeight", 10),
    m_Alpha(saver, this, "alpha", 0),
    m_Beta(saver, this, "beta", 0),
    m_Gamma(saver, this, "gamma", 0),
    m_ChiMin(saver, this, "chiMin", 0),
    m_ChiMax(saver, this, "chiMax", 360),
    m_RingTTH(saver, this, "ringTTH", QcepDoubleList()),
    m_RingIntensity(saver, this, "ringIntensity", QcepDoubleList()),
    m_RingWidth(saver, this, "ringWidth", QcepDoubleList())
{
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
  QxrdDoubleImageDataPtr img = QxrdDoubleImageDataPtr(new QxrdDoubleImageData(NULL,
                                                                              m_Allocator,
                                                                              QxrdAllocator::AllocateDouble,
                                                                              get_NCols(),
                                                                              get_NRows()));

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

  m_Processor -> newData(img, QxrdMaskDataPtr());
}

void QxrdGenerateTestImage::generateTTHImage()
{
  QxrdDoubleImageDataPtr img = QxrdDoubleImageDataPtr(new QxrdDoubleImageData(NULL,
                                                                              m_Allocator,
                                                                              QxrdAllocator::AllocateDouble,
                                                                              get_NCols(),
                                                                              get_NRows()));

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
  m_Processor -> newData(img, QxrdMaskDataPtr());
}

void QxrdGenerateTestImage::generateChiImage()
{
  QxrdDoubleImageDataPtr img = QxrdDoubleImageDataPtr(new QxrdDoubleImageData(NULL,
                                                                              m_Allocator,
                                                                              QxrdAllocator::AllocateDouble,
                                                                              get_NCols(),
                                                                              get_NRows()));

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
  m_Processor -> newData(img, QxrdMaskDataPtr());
}
