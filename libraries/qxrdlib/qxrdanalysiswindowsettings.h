#ifndef QXRDANALYSISWINDOWSETTINGS_H
#define QXRDANALYSISWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdAnalysisWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

private:
  typedef QxrdMainWindowSettings inherited;

public:
  Q_INVOKABLE QxrdAnalysisWindowSettings(QString name);
  void initialize(QcepObjectWPtr parent);

  QcepMainWindowPtr newWindow();
};

Q_DECLARE_METATYPE(QxrdAnalysisWindowSettings*)

#endif // QXRDANALYSISWINDOWSETTINGS_H
