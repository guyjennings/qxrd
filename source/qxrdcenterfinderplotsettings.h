#ifndef QXRDCENTERFINDERPLOTSETTINGS_H
#define QXRDCENTERFINDERPLOTSETTINGS_H

#include <QObject>
#include "qxrdplotsettings.h"

class QxrdCenterFinderPlotSettings : public QxrdPlotSettings
{
  Q_OBJECT
public:
  explicit QxrdCenterFinderPlotSettings(QcepSettingsSaverWPtr saver, QObject *parent);
  
signals:
  
public slots:
  
};

typedef QSharedPointer<QxrdCenterFinderPlotSettings> QxrdCenterFinderPlotSettingsPtr;
typedef QWeakPointer<QxrdCenterFinderPlotSettings>   QxrdCenterFinderPlotSettingsWPtr;

#endif // QXRDCENTERFINDERPLOTSETTINGS_H
