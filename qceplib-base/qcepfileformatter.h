#ifndef QCEPFILEFORMATTER_H
#define QCEPFILEFORMATTER_H

#include "qcepobject.h"
#include "qcepfileformatter-ptr.h"

class QcepFileFormatter : public QcepObject
{
public:
  QcepFileFormatter(QString filePath);

  static QcepFileFormatterPtr defaultFormatter(QString filePath, QString exten);

  virtual void beginWriteFile() = 0;
  virtual void endWriteFile() = 0;

  virtual void beginWriteObject(QString objectName, QString className) = 0;
  virtual void endWriteObject() = 0;

  virtual void beginWriteProperties() = 0;
  virtual void writeProperty(QString name, QVariant val) = 0;
  virtual void endWriteProperties() = 0;

  virtual void beginWriteChildren() = 0;
  virtual void endWriteChildren() = 0;

  virtual void beginWriteData() = 0;
  virtual void endWriteData() = 0;

protected:
  QString     m_FilePath;
  QStringList m_ObjectPath;
};

#endif // QCEPFILEFORMATTER_H
