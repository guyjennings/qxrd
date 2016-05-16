#ifndef QXRDCENTERFINDERPLOTSETTINGS_H
#define QXRDCENTERFINDERPLOTSETTINGS_H

#include <QObject>
#include "qcepplotsettings.h"

class QxrdCenterFinderPlotSettings : public QcepPlotSettings
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdCenterFinderPlotSettings(QString name);
  QxrdCenterFinderPlotSettings();

signals:
  
public slots:
  
};

Q_DECLARE_METATYPE(QxrdCenterFinderPlotSettings*)

typedef QSharedPointer<QxrdCenterFinderPlotSettings> QxrdCenterFinderPlotSettingsPtr;
typedef QWeakPointer<QxrdCenterFinderPlotSettings>   QxrdCenterFinderPlotSettingsWPtr;

#endif // QXRDCENTERFINDERPLOTSETTINGS_H
