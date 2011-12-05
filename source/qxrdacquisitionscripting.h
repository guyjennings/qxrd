#ifndef QXRDACQUISITIONSCRIPTING_H
#define QXRDACQUISITIONSCRIPTING_H

#include "qcepmacros.h"

#include "qxrdacquisitionparameters.h"

#include <QScriptEngine>

//class QxrdDataProcessor;

class QxrdAcquisitionScripting : public QxrdAcquisitionParameters
{
  Q_OBJECT
public:
  QxrdAcquisitionScripting(QxrdSettingsSaver *saver);

public slots:
  void propertyList();
  void Message(QString cmd);
};

#endif // QXRDACQUISITIONSCRIPTING_H
