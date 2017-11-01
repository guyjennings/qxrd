#ifndef QXRDMASKINGWINDOWSETTINGS_H
#define QXRDMASKINGWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdMaskingWindowSettings : public QxrdMainWindowSettings
{
public:
  Q_INVOKABLE QxrdMaskingWindowSettings(QString name,
                                        QxrdApplicationPtr app,
                                        QxrdExperimentPtr expt);
};

#endif // QXRDMASKINGWINDOWSETTINGS_H
