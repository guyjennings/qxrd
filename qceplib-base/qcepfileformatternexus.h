#ifndef QCEPFILEFORMATTERNEXUS_H
#define QCEPFILEFORMATTERNEXUS_H

#include "qcepfileformatter.h"

#ifdef HAVE_NEXUS

class QcepFileFormatterNexus : public QcepFileFormatter
{
public:
  QcepFileFormatterNexus(QString filePath);

  virtual void beginWriteObject(QString objectName, QString className);
  virtual void endWriteObject();

  virtual void writeComment(QString cmt);

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
};

#endif // HAVE_NEXUS

#endif // QCEPFILEFORMATTERNEXUS_H
