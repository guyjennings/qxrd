#ifndef QXRDRINGSAMPLEDDATA_H
#define QXRDRINGSAMPLEDDATA_H

#include <QObject>
#include <QMutex>
#include "qcepmacros.h"
#include <QSharedPointer>
#include <QScriptEngine>
#include <QSettings>

class QxrdRingSampledData : public QObject
{
  Q_OBJECT
public:
  explicit QxrdRingSampledData(QObject *parent = 0);

signals:

public slots:
  void append(QPointF pt);
  void remove(int n);
  void clear();
  void setPoint(int n, QPointF pt);
  int  count() const;
  QPointF point(int n) const;

public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

private:
  mutable QMutex         m_Mutex;

  QVector<QPointF> m_Points;
};

//typedef QSharedPointer<QxrdRingSampledData> QxrdRingSampledDataPtr;
typedef QxrdRingSampledData *QxrdRingSampledDataPtr;

Q_DECLARE_METATYPE(QxrdRingSampledData*)

QScriptValue QxrdRingSampledDataToScriptValue(QScriptEngine *engine, QxrdRingSampledData* const &in);
void QxrdRingSampledDataFromScriptValue(const QScriptValue &object, QxrdRingSampledData* &out);

#endif // QXRDRINGSAMPLEDDATA_H
