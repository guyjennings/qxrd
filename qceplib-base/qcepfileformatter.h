#ifndef QCEPFILEFORMATTER_H
#define QCEPFILEFORMATTER_H

#include "qceplib_global.h"
#include "qcepobject.h"
#include "qcepobject-ptr.h"
#include "qcepfileformatter-ptr.h"

class QCEP_EXPORT QcepFileFormatter : public QcepObject
{
  Q_OBJECT

public:
  QcepFileFormatter(QString filePath);

  static QcepFileFormatterPtr defaultFormatter(QString filePath, QString exten);

  virtual void beginWriteFile() = 0;
  virtual void endWriteFile() = 0;

  virtual void writeComment(QString cmt) = 0;

  virtual void beginWriteObject(QString objectName, QString className) = 0;
  virtual void endWriteObject() = 0;

  virtual void beginWriteProperties() = 0;
  virtual void writeProperty(QString name, QVariant val) = 0;
  virtual void endWriteProperties() = 0;

  virtual void beginWriteChildren() = 0;
  virtual void endWriteChildren() = 0;

  virtual void beginWriteData() = 0;
  virtual void endWriteData() = 0;


  virtual void beginReadFile() = 0;
  virtual void endReadFile() = 0;

  virtual QcepObjectPtr nextObject() = 0;

  virtual void beginReadObject(QcepObjectPtr obj) = 0;
  virtual void endReadObject() = 0;

  virtual bool beginReadProperties() = 0;
  virtual QString nextPropertyName() = 0;
  virtual QVariant nextPropertyValue() = 0;
  virtual void endReadProperties() = 0;

  virtual bool beginReadChildren() = 0;
  virtual QcepObjectPtr nextChild() = 0;
  virtual void endReadChildren() = 0;

  virtual bool beginReadData() = 0;
  virtual void endReadData() = 0;

  virtual QcepObjectPtr construct(QString name, QString className);

protected:
  QString     m_FilePath;
  QStringList m_ObjectPath;
};

#endif // QCEPFILEFORMATTER_H
