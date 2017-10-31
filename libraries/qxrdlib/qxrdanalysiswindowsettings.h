#ifndef QXRDANALYSISWINDOWSETTINGS_H
#define QXRDANALYSISWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdAnalysisWindowSettings : public QxrdMainWindowSettings
{
public:
  Q_INVOKABLE QxrdAnalysisWindowSettings(QString name);
};

#endif // QXRDANALYSISWINDOWSETTINGS_H
