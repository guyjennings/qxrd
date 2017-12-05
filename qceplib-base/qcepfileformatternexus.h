#ifndef QCEPFILEFORMATTERNEXUS_H
#define QCEPFILEFORMATTERNEXUS_H

#include "qceplib_global.h"
#include "qcepfileformatter.h"

//#ifdef HAVE_NEXUS

class QCEP_EXPORT QcepFileFormatterNexus : public QcepFileFormatter
{
  Q_OBJECT

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

  virtual QcepObjectPtr nextObject();

  virtual void beginReadObject(QcepObjectPtr obj);
  virtual void endReadObject();

  virtual bool beginReadProperties();
  virtual QString nextPropertyName();
  virtual QVariant nextPropertyValue();
  virtual void endReadProperties();

  virtual bool beginReadChildren();
  virtual QcepObjectPtr nextChild();
  virtual void endReadChildren();

  virtual bool beginReadData();
  virtual void endReadData();
};

//#endif // HAVE_NEXUS

#endif // QCEPFILEFORMATTERNEXUS_H
