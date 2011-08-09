#ifndef QXRDRINGSAMPLEDDATA_H
#define QXRDRINGSAMPLEDDATA_H

#include <QObject>
#include <QMutex>
#include "qcepmacros.h"
#include <QSharedPointer>
#include "qwt_double_rect.h"
#include "qxrdsettings.h"
#include <QScriptEngine>

class QxrdRingSampledData : public QObject
{
  Q_OBJECT
public:
  explicit QxrdRingSampledData(QObject *parent = 0);

signals:

public slots:
  void append(QwtDoublePoint pt);
  void remove(int n);
  void clear();
  void setPoint(int n, QwtDoublePoint pt);
  int  count() const;
  QwtDoublePoint point(int n) const;

public:
  void readSettings(QxrdSettings &settings, QString section);
  void writeSettings(QxrdSettings &settings, QString section);

private:
  mutable QMutex         m_Mutex;

  QVector<QwtDoublePoint> m_Points;
};

//typedef QSharedPointer<QxrdRingSampledData> QxrdRingSampledDataPtr;
typedef QxrdRingSampledData *QxrdRingSampledDataPtr;

Q_DECLARE_METATYPE(QxrdRingSampledData*)

QScriptValue QxrdRingSampledDataToScriptValue(QScriptEngine *engine, QxrdRingSampledData* const &in);
void QxrdRingSampledDataFromScriptValue(const QScriptValue &object, QxrdRingSampledData* &out);

#endif // QXRDRINGSAMPLEDDATA_H
