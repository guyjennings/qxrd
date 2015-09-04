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

class QcepDataGroup : public QcepDataObject
{
  Q_OBJECT

public:
  QcepDataGroup(QcepSettingsSaverWPtr saver, QString name, QcepObject *parent);

  static QcepDataGroupPtr newDataGroup(QcepSettingsSaverWPtr saver, QString name, QcepObject *parent);

public slots:
  virtual QString description() const;

  QcepDataObjectPtr item(int n);
  QcepDataObjectPtr item(QString nm);

  QcepDataGroupPtr  group(QString path);
  QcepDataArrayPtr  array(QString path);
  QcepDataColumnPtr column(QString path);
  QcepDataColumnScanPtr columnScan(QString path);
  QcepDoubleImageDataPtr image(QString path);

  int                childCount() const;

  void append(QcepDataObjectPtr obj);
  void append(QString path, QcepDataObjectPtr obj);
  void remove(QcepDataObjectPtr obj);
  void remove(QString path);

  QcepDataGroupPtr createGroup(QString path);

  QcepDataGroupPtr newGroup(QString path);
  QcepDataArrayPtr newArray(QString path, QVector<int> dims);
  QcepDataColumnPtr newColumn(QString path, int nrow);
  QcepDataColumnScanPtr newColumnScan(QString path, int nrow, QStringList cols);
  QcepDoubleImageDataPtr newImage(QString path, int width=0, int height=0);

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
