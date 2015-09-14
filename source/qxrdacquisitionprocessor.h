#ifndef QXRDACQUISITIONPROCESSOR_H
#define QXRDACQUISITIONPROCESSOR_H

#include "qcepmacros.h"
#include "qcepobject.h"

#include "qcepsettingssaver-ptr.h"
#include "qxrdmultipleacquisition-ptr.h"
#include "qxrddetector-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qcepallocator-ptr.h"
#include "qxrdacquisitionprocessor-ptr.h"
#include <QScriptEngine>

class QxrdAcquisitionProcessor : public QcepObject, public QEnableSharedFromThis<QxrdAcquisitionProcessor>
{
    Q_OBJECT

public:
  QxrdAcquisitionProcessor(QcepSettingsSaverWPtr saver,
                           QxrdExperimentWPtr    doc,
                           QxrdDetectorWPtr      det);

signals:

public slots:

public:

  static QScriptValue toScriptValue(QScriptEngine *engine, const QxrdAcquisitionProcessorPtr &proc);
  static void fromScriptValue(const QScriptValue &obj, QxrdAcquisitionProcessorPtr &proc);

private:
  QcepSettingsSaverWPtr m_Saver;

public:
  // Properties...

private:
  QxrdExperimentWPtr    m_Experiment;
  QxrdDetectorWPtr      m_Detector;
};

#endif // QXRDACQUISITIONPROCESSOR_H
