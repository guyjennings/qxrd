#ifndef QCEPSERIALIZABLEOBJECT_H
#define QCEPSERIALIZABLEOBJECT_H

#include "qceplib_global.h"
#include "qcepobject.h"
#include "qcepserializableobject-ptr.h"
#include "qcepfileformatter-ptr.h"

class QCEP_EXPORT QcepSerializableObject : public QcepObject
{
  Q_OBJECT

public:
  QcepSerializableObject(QString name);

  virtual ~QcepSerializableObject();

  static QcepSerializableObjectPtr readDataObject(QcepFileFormatterPtr fmt);

  void readObject(QcepFileFormatterPtr fmt);
  void writeObject(QcepFileFormatterPtr fmt);

  virtual void readObjectData(QcepFileFormatterPtr fmt);
  virtual void writeObjectData(QcepFileFormatterPtr fmt);


  virtual void propertyChanged(QcepProperty *prop);


public slots:
  void dumpObjectTreePtr(int level=0);

  int childrenChanged() const;
  QString childrenChangedBy() const;


//  QVector<QcepSerializableObjectPtr> childrenPtr() const;


private:
};

#endif // QCEPSERIALIZABLEOBJECT_H
