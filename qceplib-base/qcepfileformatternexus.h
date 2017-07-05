#ifndef QCEPFILEFORMATTERNEXUS_H
#define QCEPFILEFORMATTERNEXUS_H

#include "qcepfileformatter.h"

//#ifdef HAVE_NEXUS

class QcepFileFormatterNexus : public QcepFileFormatter
{
public:
  QcepFileFormatterNexus(QString filePath);

  virtual void beginWriteFile();
  virtual void endWriteFile();

  virtual void writeComment(QString cmt);

  virtual void beginWriteObject(QString objectName, QString className);
  virtual void endWriteObject();

  virtual void beginWriteProperties();
  virtual void writeProperty(QString name, QVariant val);
  virtual void endWriteProperties();

  virtual void beginWriteChildren();
  virtual void endWriteChildren();

  virtual void beginWriteData();
  virtual void endWriteData();


  virtual void beginReadFile();
  virtual void endReadFile();

  virtual QcepSerializableObjectPtr nextObject();

  virtual void beginReadObject(QcepSerializableObjectPtr obj);
  virtual void endReadObject();

  virtual bool beginReadProperties();
  virtual QString nextPropertyName();
  virtual QVariant nextPropertyValue();
  virtual void endReadProperties();

  virtual bool beginReadChildren();
  virtual QcepSerializableObjectPtr nextChild();
  virtual void endReadChildren();

  virtual bool beginReadData();
  virtual void endReadData();
};

//#endif // HAVE_NEXUS

#endif // QCEPFILEFORMATTERNEXUS_H
