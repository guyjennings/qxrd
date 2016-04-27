#ifndef QCEPDATAOBJECT_H
#define QCEPDATAOBJECT_H

#include <QObject>
#include "qcepobject.h"
#include "qcepdataobject-ptr.h"
#include "qcepdatagroup-ptr.h"
#include <QScriptValue>
#include "qcepproperty.h"

class QcepDataObject : public QcepObject
{
  Q_OBJECT

public:
  QcepDataObject(QcepObjectWPtr parent,
                 QString name,
                 int byteSize);

  virtual ~QcepDataObject();

  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  static QString mimeType();

  static QString fileFormatAny();
  static QString fileFormatHDF5();
  static QString fileFormatNexus();

signals:
  void dataObjectChanged();

public slots:
  virtual QString description() const;
  QString metaTypeName(int id) const;
  QString pathName() const;
  QcepDataGroupPtr rootItem();
  virtual QcepDataGroupPtr parentItem() const;

public:
  typedef enum {
    NoOverwrite,
    CanOverwrite
  } Overwrite;

  virtual int childCount() const;
  virtual QcepDataObjectPtr item(int n);
  virtual QcepDataObjectPtr item(QString nm);
  virtual void setParentItem(QcepDataGroupWPtr parent);
  virtual int indexInParent() const;
  virtual int rowCount() const;
  virtual int columnCount() const;
  virtual QVariant columnData(int col) const;

  virtual QString fileFormatFilterString();
  static  QString fileFormatTIFF();
  static  QString fileFormatTabDelimited();
  static  QString fileFormatTransposedTabDelimited();
  static  QString fileFormatCSV();
  static  QString fileFormatTransposedCSV();
  static  QString fileFormatSpec();
  static  QString fileFormatCIF();

  static QScriptValue toScriptValue(QScriptEngine *engine, const QcepDataObjectPtr &data);
  static void fromScriptValue(const QScriptValue &obj, QcepDataObjectPtr &data);

  static int allocatedObjects();
  static int deletedObjects();

  void setNameAndSuffix(QString oldName, QString newSuffix);

  virtual void saveData(QString &name, QString filter, Overwrite canOverwrite=NoOverwrite);

protected:
  void mkPath(QString filePath);
  QString uniqueFileName(QString name);

private:
  QcepDataGroupWPtr     m_Parent;
//  QcepSettingsSaverWPtr m_Saver;

protected:
  mutable QMutex        m_Mutex;

public:
  Q_PROPERTY(quint64 byteSize READ get_ByteSize WRITE set_ByteSize STORED false)
  QCEP_INTEGER64_PROPERTY(ByteSize)

  Q_PROPERTY(QString creator READ get_Creator WRITE set_Creator)
  QCEP_STRING_PROPERTY(Creator)

  Q_PROPERTY(QString version READ get_Version WRITE set_Version)
  QCEP_STRING_PROPERTY(Version)

  Q_PROPERTY(QString qtVersion READ get_QtVersion WRITE set_QtVersion)
  QCEP_STRING_PROPERTY(QtVersion)

  Q_PROPERTY(QString description READ get_Description WRITE set_Description)
  QCEP_STRING_PROPERTY(Description)

  Q_PROPERTY(QString fileName READ get_FileName WRITE set_FileName)
  QCEP_STRING_PROPERTY(FileName)

  Q_PROPERTY(int objectSaved READ get_ObjectSaved WRITE set_ObjectSaved STORED false)
  QCEP_INTEGER_PROPERTY(ObjectSaved)

  Q_PROPERTY(int index READ get_Index WRITE set_Index)
  QCEP_INTEGER_PROPERTY(Index)
};

#endif // QCEPDATAOBJECT_H
