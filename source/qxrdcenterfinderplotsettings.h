#ifndef QXRDCENTERFINDERPLOTSETTINGS_H
#define QXRDCENTERFINDERPLOTSETTINGS_H

#include <QObject>
#include "qxrdplotsettings.h"

class QxrdCenterFinderPlotSettings : public QxrdPlotSettings
{
  Q_OBJECT
public:
  explicit QxrdCenterFinderPlotSettings(QxrdSettingsSaverWPtr saver, QObject *parent);
  
signals:
  
public slots:
  
};

#endif // QXRDCENTERFINDERPLOTSETTINGS_H
