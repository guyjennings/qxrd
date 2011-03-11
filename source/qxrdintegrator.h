#ifndef QXRDINTEGRATOR_H
#define QXRDINTEGRATOR_H

#include "qcepmacros.h"

#include <QObject>
#include "qxrdintegrateddata.h"
#include "qcepproperty.h"
#include "qxrdsettings.h"
#include "qxrddataprocessor.h"
#include <qwt_double_rect.h>

class QxrdDataProcessorBase;
class QxrdAllocator;

class QxrdIntegrator : public QObject
{
  Q_OBJECT;

public:
  QxrdIntegrator(QxrdDataProcessorBase *proc, QxrdAllocator *alloc, QObject *parent=0);

public:
  Q_PROPERTY(int oversample READ get_Oversample WRITE set_Oversample);
  QCEP_INTEGER_PROPERTY(Oversample);

  Q_PROPERTY(double integrationStep READ get_IntegrationStep WRITE set_IntegrationStep);
  QCEP_DOUBLE_PROPERTY(IntegrationStep);

public:
  void readSettings(QxrdSettings &settings, QString section);
  void writeSettings(QxrdSettings &settings, QString section);
  QxrdDataProcessorBase *dataProcessor() const;

signals:
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

public slots:
  QxrdIntegratedDataPtr performIntegration(QxrdIntegratedDataPtr integ, QxrdDoubleImageDataPtr dimg, QxrdMaskDataPtr mask);
  QxrdIntegratedDataPtr integrate(QxrdIntegratedDataPtr integ, QxrdDoubleImageDataPtr dimg, QxrdMaskDataPtr mask, int oversample, int normalize);
  QxrdIntegratedDataPtr integrate(QxrdIntegratedDataPtr integ, QxrdDoubleImageDataPtr dimg, QxrdMaskDataPtr mask, double cx, double cy, int oversample, int normalize);

  QxrdIntegratedDataPtr sliceLine(QxrdIntegratedDataPtr integ, QxrdDoubleImageDataPtr dimg, double x0, double y0, double x1, double y1, double width);
  QxrdIntegratedDataPtr slicePolygon(QxrdIntegratedDataPtr integ, QxrdDoubleImageDataPtr dimg, QwtArray<QwtDoublePoint> poly, double width);

private:
  mutable QMutex         m_Mutex;
  QxrdDataProcessorBase *m_DataProcessor;
  QxrdAllocator         *m_Allocator;
};

#endif // QXRDINTEGRATOR_H
