#ifndef QXRDCENTERFINDERPLOTSETTINGS_H
#define QXRDCENTERFINDERPLOTSETTINGS_H

#include "qxrdlib_global.h"
#include <QObject>
#include "qcepplotsettings.h"
#include "qxrdcenterfinderplotsettings-ptr.h"

class QXRD_EXPORT QxrdCenterFinderPlotSettings : public QcepPlotSettings
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdCenterFinderPlotSettings(QString name);
  static QxrdCenterFinderPlotSettingsPtr newCenterFinderPlotSettings();

signals:
  
public slots:
  
};

Q_DECLARE_METATYPE(QxrdCenterFinderPlotSettings*)

#endif // QXRDCENTERFINDERPLOTSETTINGS_H