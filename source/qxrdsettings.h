#ifndef QXRDSETTINGS_H
#define QXRDSETTINGS_H

#include "qcepmacros.h"

#include <QSettings>

class QxrdSettings : public QSettings
{
  Q_OBJECT;

public:
  QxrdSettings(QObject *parent=0);
  ~QxrdSettings();
};

#endif
