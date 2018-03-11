#ifndef QXRDINFOWINDOWSETTINGS_H
#define QXRDINFOWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdInfoWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

private:
  typedef QxrdMainWindowSettings inherited;

public:
  Q_INVOKABLE QxrdInfoWindowSettings(QString name);
  void initialize(QcepObjectWPtr parent);

  QcepMainWindowPtr newWindow();
};

Q_DECLARE_METATYPE(QxrdInfoWindowSettings*)

#endif // QXRDINFOWINDOWSETTINGS_H
