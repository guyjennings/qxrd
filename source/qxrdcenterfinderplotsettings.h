#ifndef QXRDCENTERFINDERPLOTSETTINGS_H
#define QXRDCENTERFINDERPLOTSETTINGS_H

#include <QObject>
#include "qcepplotsettings.h"

class QxrdCenterFinderPlotSettings : public QcepPlotSettings
{
  Q_OBJECT
public:
  explicit QxrdCenterFinderPlotSettings(QcepSettingsSaverWPtr saver, QcepObject *parent);
  
signals:
  
public slots:
  
};

typedef QSharedPointer<QxrdCenterFinderPlotSettings> QxrdCenterFinderPlotSettingsPtr;
typedef QWeakPointer<QxrdCenterFinderPlotSettings>   QxrdCenterFinderPlotSettingsWPtr;

#endif // QXRDCENTERFINDERPLOTSETTINGS_H
