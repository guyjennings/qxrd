#ifndef QXRDGENERATETESTIMAGE_H
#define QXRDGENERATETESTIMAGE_H

#include "qcepmacros.h"

#include <QObject>

#include "qcepproperty.h"
#include "qxrddataprocessor-ptr.h"
#include "qxrdallocator-ptr.h"
#include "qxrddetectorgeometry-ptr.h"
#include "qxrddetectorgeometry.h"
#include "qxrdobjectnamer.h"

class QxrdGenerateTestImage : public QObject
{
  Q_OBJECT
public:
  QxrdGenerateTestImage(QxrdSettingsSaverWPtr saver,
                        QxrdAllocatorWPtr alloc);

public slots:
  void setDimension(int nc, int nr);
  void setCenter(double cx, double cy);
  void setDistance(double l, double pw, double ph);
  void setEnergy(double energy);
  void setTiltAngles(double alpha, double beta, double gamma);
  void setChiRange(double chiMin, double chiMax);

  void clearRings();
  void appendRing(double tth, double intensity, double width);
  void generateImage();
  void generateTTHImage();
  void generateChiImage();

public:
  void setProcessor(QxrdDataProcessorWPtr proc);

private:
  QxrdObjectNamer         m_ObjectNamer;

  QxrdDataProcessorWPtr   m_Processor;
  QxrdAllocatorWPtr       m_Allocator;
  QxrdDetectorGeometryPtr m_Geometry;

public:
  Q_PROPERTY(int     nRows      READ get_NRows WRITE set_NRows)
  QCEP_INTEGER_PROPERTY(NRows)

  Q_PROPERTY(int     nCols      READ get_NCols WRITE set_NCols)
  QCEP_INTEGER_PROPERTY(NCols)

  Q_PROPERTY(double centerX READ get_CenterX WRITE set_CenterX)
  QCEP_DOUBLE_PROPERTY(CenterX)

  Q_PROPERTY(double centerY READ get_CenterY WRITE set_CenterY)
  QCEP_DOUBLE_PROPERTY(CenterY)

  Q_PROPERTY(double distance READ get_Distance WRITE set_Distance)
  QCEP_DOUBLE_PROPERTY(Distance)

  Q_PROPERTY(double energy READ get_Energy WRITE set_Energy)
  QCEP_DOUBLE_PROPERTY(Energy)

  Q_PROPERTY(double pixelWidth READ get_PixelWidth WRITE set_PixelWidth)
  QCEP_DOUBLE_PROPERTY(PixelWidth)

  Q_PROPERTY(double pixelHeight READ get_PixelHeight WRITE set_PixelHeight)
  QCEP_DOUBLE_PROPERTY(PixelHeight)

  Q_PROPERTY(double alpha READ get_Alpha WRITE set_Alpha)
  QCEP_DOUBLE_PROPERTY(Alpha)

  Q_PROPERTY(double beta READ get_Beta WRITE set_Beta)
  QCEP_DOUBLE_PROPERTY(Beta)

  Q_PROPERTY(double gamma READ get_Gamma WRITE set_Gamma)
  QCEP_DOUBLE_PROPERTY(Gamma)

  Q_PROPERTY(double chiMin READ get_ChiMin WRITE set_ChiMin)
  QCEP_DOUBLE_PROPERTY(ChiMin)

  Q_PROPERTY(double chiMax READ get_ChiMax WRITE set_ChiMax)
  QCEP_DOUBLE_PROPERTY(ChiMax)

  Q_PROPERTY(QcepDoubleList ringTTH READ get_RingTTH WRITE set_RingTTH)
  QCEP_DOUBLE_LIST_PROPERTY(RingTTH)

  Q_PROPERTY(QcepDoubleList ringIntensity READ get_RingIntensity WRITE set_RingIntensity)
  QCEP_DOUBLE_LIST_PROPERTY(RingIntensity)

  Q_PROPERTY(QcepDoubleList ringWidth READ get_RingWidth WRITE set_RingWidth)
  QCEP_DOUBLE_LIST_PROPERTY(RingWidth)
};

#endif // QXRDGENERATETESTIMAGE_H
