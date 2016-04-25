#ifndef QCEPDATAGROUP_H
#define QCEPDATAGROUP_H

#include <QObject>
#include <QVector>
#include "qcepdataobject.h"
#include "qcepdataobject-ptr.h"
#include "qcepdatagroup-ptr.h"
#include "qcepdataarray-ptr.h"
#include "qcepdatacolumn-ptr.h"
#include "qcepdatacolumnscan-ptr.h"
#include "qcepimagedata-ptr.h"
#include "qcepintegrateddata-ptr.h"

class QcepDataGroup : public QcepDataObject
{
  Q_OBJECT

public:
  QcepDataGroup(QcepObjectWPtr parent,
                QcepSettingsSaverWPtr saver, QString name);

  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

public slots:
  virtual QString description() const;

  QcepDataObjectPtr item(QString nm);
  QcepDataObjectPtr item(int n);

  QcepDataGroupPtr  group(QString path);
  QcepDataGroupPtr  group(int n);

  QcepDataArrayPtr  array(QString path);
  QcepDataArrayPtr  array(int n);

  QcepDataColumnPtr column(QString path);
  QcepDataColumnPtr column(int n);

  QcepDataColumnScanPtr columnScan(QString path);
  QcepDataColumnScanPtr columnScan(int n);

  QcepDoubleImageDataPtr image(QString path);
  QcepDoubleImageDataPtr image(int n);

  QcepIntegratedDataPtr  integratedData(QString path);
  QcepIntegratedDataPtr  integratedData(int n);

  int                childCount() const;

  void clear();
  void insert(int atRow, QcepDataObjectPtr obj);

  void append(QcepDataObjectPtr obj);
  void append(QString path, QcepDataObjectPtr obj);
  void remove(int n);
  void remove(QcepDataObjectPtr obj);
  void remove(QString path);

  QcepDataGroupPtr createGroup(QString path);

  static QScriptValue toGroupScriptValue(QScriptEngine *engine, const QcepDataGroupPtr &data);
  static void fromGroupScriptValue(const QScriptValue &obj, QcepDataGroupPtr &data);

  QString directoryName(QString path);
  QString object(QString path);

  QcepDataGroupPtr containingGroup(QString path);
  QcepDataObjectPtr referencedObject(QString path);

private:
  QVector<QcepDataObjectPtr> m_Objects;
};

#endif // QCEPDATAGROUP_H
