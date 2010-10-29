#ifndef QXRDINTEGRATOR_H
#define QXRDINTEGRATOR_H

#include "qcepmacros.h"

#include <QObject>
#include "qxrdintegrateddata.h"
#include "qcepproperty.h"
#include "qxrdsettings.h"
#include "qxrddataprocessor.h"
#include <qwt_double_rect.h>

class QxrdIntegrator : public QObject
{
  Q_OBJECT;

public:
  QxrdIntegrator(QxrdDataProcessorPtr proc, QxrdAllocatorPtr alloc, QObject *parent=0);

public:
  Q_PROPERTY(int oversample READ get_Oversample WRITE set_Oversample);
  QCEP_INTEGER_PROPERTY(Oversample);

  Q_PROPERTY(double integrationStep READ get_IntegrationStep WRITE set_IntegrationStep);
  QCEP_DOUBLE_PROPERTY(IntegrationStep);

public:
  void readSettings(QxrdSettings &settings, QString section);
  void writeSettings(QxrdSettings &settings, QString section);

signals:
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

public slots:
  void integrateSaveAndDisplay();
  QxrdIntegratedDataPtr performIntegration(QxrdDoubleImageDataPtr dimg, QxrdMaskDataPtr mask);
  void saveIntegratedData(QxrdIntegratedDataPtr d);
  void displayIntegratedData(QxrdIntegratedDataPtr d);
  QxrdIntegratedDataPtr integrate(QxrdDoubleImageDataPtr dimg, QxrdMaskDataPtr mask, int oversample, int normalize);
  QxrdIntegratedDataPtr integrate(QxrdDoubleImageDataPtr dimg, QxrdMaskDataPtr mask, double cx, double cy, int oversample, int normalize);

  QxrdIntegratedDataPtr sliceLine(QxrdDoubleImageDataPtr dimg, double x0, double y0, double x1, double y1, double width);
  QxrdIntegratedDataPtr slicePolygon(QxrdDoubleImageDataPtr dimg, QwtArray<QwtDoublePoint> poly, double width);

private:
  mutable QMutex       m_Mutex;
  QxrdDataProcessorPtr m_DataProcessor;
  QxrdAllocatorPtr     m_Allocator;
};

#endif // QXRDINTEGRATOR_H
