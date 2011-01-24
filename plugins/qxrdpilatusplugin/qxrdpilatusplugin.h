#ifndef QXRDPILATUSPLUGIN_H
#define QXRDPILATUSPLUGIN_H

#include <QObject>
#include "qxrddetectorplugininterface.h"

class QxrdPilatusPlugin : public QObject, public QxrdDetectorPluginInterface
{
  Q_OBJECT;
  Q_INTERFACES(QxrdDetectorPluginInterface);

public:
  QxrdPilatusPlugin();

  QString name() const;
};

#endif // QXRDPILATUSPLUGIN_H
