#ifndef QCEPFILEFORMATTERTEXT_H
#define QCEPFILEFORMATTERTEXT_H

#include "qcepfileformatter.h"

class QcepFileFormatterText : public QcepFileFormatter
{
public:
  QcepFileFormatterText(QString filePath);

  virtual void beginWriteObject(QString objectName, QString className);
  virtual void endWriteObject();

  virtual void beginWriteProperties();
  virtual void writeProperty(QString name, QVariant val);
  virtual void endWriteProperties();

  virtual void beginWriteChildren();
  virtual void endWriteChildren();

  virtual void beginWriteData();
  virtual void endWriteData();

private:
  int m_OutputIndent;
};

#endif // QCEPFILEFORMATTERTEXT_H
