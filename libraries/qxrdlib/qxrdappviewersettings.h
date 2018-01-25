#ifndef QXRDAPPVIEWERSETTINGS_H
#define QXRDAPPVIEWERSETTINGS_H

#include "qxrdlib_global.h"
#include "qxrdappcommonsettings.h"
#include "qxrdappviewer-ptr.h"

class QXRD_EXPORT QxrdAppViewerSettings : public QxrdAppCommonSettings
{
  Q_OBJECT

private:
  typedef QxrdAppCommonSettings inherited;

public:
  QxrdAppViewerSettings(QxrdAppViewerWPtr app, int argc, char **argv);
  virtual ~QxrdAppViewerSettings();
  void init();
};

#endif // QXRDAPPVIEWERSETTINGS_H
