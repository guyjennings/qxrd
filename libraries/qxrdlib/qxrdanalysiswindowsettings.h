#ifndef QXRDANALYSISWINDOWSETTINGS_H
#define QXRDANALYSISWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdAnalysisWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdAnalysisWindowSettings(QString name);

  QxrdMainWindowPtr newWindow();
};

Q_DECLARE_METATYPE(QxrdAnalysisWindowSettings*)

#endif // QXRDANALYSISWINDOWSETTINGS_H
