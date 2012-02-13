#ifndef QXRDRINGSETSAMPLEDDATA_H
#define QXRDRINGSETSAMPLEDDATA_H

#include <QObject>
#include "qcepmacros.h"
#include "qxrdringsampleddata.h"
//#include "qxrdringsetfitparameters.h"
#include "qxrdsettingssaver.h"

class QxrdRingSetSampledData : public QObject
{
  Q_OBJECT
public:
  explicit QxrdRingSetSampledData(QxrdSettingsSaverPtr saver);

public slots:
  int count() const;
  QxrdRingSampledDataPtr ring(int n) const;
  void setRing(int n, QxrdRingSampledDataPtr d);
  void append();
  void append(QxrdRingSampledDataPtr d);
  void remove(int n);
  void clear();

public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

private:
  mutable QMutex         m_Mutex;

//  QxrdRingSetFitParametersPtr m_FitParms;

  QList<QxrdRingSampledDataPtr> m_Rings;
};

typedef QSharedPointer<QxrdRingSetSampledData> QxrdRingSetSampledDataPtr;

Q_DECLARE_METATYPE(QxrdRingSetSampledDataPtr)

#endif // QXRDRINGSETSAMPLEDDATA_H
