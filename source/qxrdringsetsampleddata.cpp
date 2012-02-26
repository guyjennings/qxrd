#include "qxrdringsetsampleddata.h"
#include "qxrdringsampleddata.h"
#include "qxrdmutexlocker.h"
#include "qcepproperty.h"
#include <stdio.h>
#include <QThread>
#include "qxrdapplication.h"

QxrdRingSetSampledData::QxrdRingSetSampledData(QxrdSettingsSaverPtr saver) :
    QObject()
{
}

void QxrdRingSetSampledData::writeSettings(QSettings *settings, QString section)
{
  if (settings) {
    QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    QcepProperty::writeSettings(this, &staticMetaObject, section, settings);

    settings->beginWriteArray(section+"/rings", count());

    for (int i=0; i<count(); i++) {
      QxrdRingSampledDataPtr r = ring(i);

      if (r==NULL) {
        if (g_Application) {
          g_Application->printMessage("NULL ring sampled data\n");
        }
      } else {
        settings->setArrayIndex(i);
        r -> writeSettings(settings, "");
      }
    }

    settings->endArray();
  }
}

void QxrdRingSetSampledData::readSettings(QSettings *settings, QString section)
{
  if (settings) {
    QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    QcepProperty::readSettings(this, &staticMetaObject, section, settings);

    clear();

    int sz = settings->beginReadArray(section+"/rings");

    for (int i=0; i<sz; i++) {
      append();

      QxrdRingSampledDataPtr r = ring(i);

      settings->setArrayIndex(i);

      r-> readSettings(settings, "");
    }

    settings->endArray();
  }
}

void QxrdRingSetSampledData::clear()
{
  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "clear", Qt::BlockingQueuedConnection));
  } else {
    m_Rings.clear();
  }
}

int QxrdRingSetSampledData::count() const
{
  return m_Rings.count();
}

QxrdRingSampledDataPtr QxrdRingSetSampledData::ring(int n) const
{
  return m_Rings.value(n);
}

void QxrdRingSetSampledData::setRing(int n, QxrdRingSampledDataPtr d)
{
  m_Rings.replace(n, d);
}

void QxrdRingSetSampledData::append()
{
  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "append", Qt::BlockingQueuedConnection));
  } else {
    m_Rings.append(QxrdRingSampledDataPtr(new QxrdRingSampledData(this)));
  }
}

void QxrdRingSetSampledData::append(QxrdRingSampledDataPtr d)
{
  if (d) {
    m_Rings.append(d);
  } else {
    m_Rings.append(QxrdRingSampledDataPtr(new QxrdRingSampledData(this)));
  }
}

void QxrdRingSetSampledData::remove(int n)
{
  m_Rings.removeAt(n);
}
