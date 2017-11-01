#ifndef QXRDMASKINGWINDOWSETTINGS_H
#define QXRDMASKINGWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"

class QXRD_EXPORT QxrdMaskingWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdMaskingWindowSettings(QString name,
                                        QxrdApplicationPtr app,
                                        QxrdExperimentPtr expt);
};

Q_DECLARE_METATYPE(QxrdMaskingWindowSettings*)

#endif // QXRDMASKINGWINDOWSETTINGS_H
