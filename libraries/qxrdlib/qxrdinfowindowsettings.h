#ifndef QXRDINFOWINDOWSETTINGS_H
#define QXRDINFOWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdInfoWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

private:
  typedef QxrdMainWindowSettings inherited;

public:
  QxrdInfoWindowSettings(QString name, QString desc);
  void initialize(QcepObjectWPtr parent);

  QxrdMainWindowPtr newWindow();
};

Q_DECLARE_METATYPE(QxrdInfoWindowSettings*)

#endif // QXRDINFOWINDOWSETTINGS_H
