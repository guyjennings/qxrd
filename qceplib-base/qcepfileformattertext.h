#ifndef QCEPFILEFORMATTERTEXT_H
#define QCEPFILEFORMATTERTEXT_H

#include "qcepfileformatter.h"
#include <QFile>
#include <QTextStream>
#include <stdio.h>

class QcepFileFormatterText : public QcepFileFormatter
{
public:
  QcepFileFormatterText(QString filePath);

  virtual void beginWriteFile();
  virtual void endWriteFile();

  virtual void beginWriteObject(QString objectName, QString className);
  virtual void endWriteObject();

  virtual void beginWriteProperties();
  virtual void writeProperty(QString name, QVariant val);
  virtual void endWriteProperties();

  virtual void beginWriteChildren();
  virtual void endWriteChildren();

  virtual void beginWriteData();
  virtual void endWriteData();

  typedef enum {
    EndOfFile,
    Number,
    String,
    Operator,
    Identifier
  } TokenType;

private:
  void    openForWriting();
  void    closeForWriting();

  void    writeIndent();
  void    writeToken(QString tok);
  void    writeVariant(QVariant val);

  void    openForReading();
  void    closeForReading();

  TokenType  nextToken(QString &token);
  TokenType  numberToken(QString &token);
  TokenType  stringToken(QString &token);
  TokenType  operatorToken(QString &token);
  TokenType  identifierToken(QString &token);

private:
  int          m_OutputIndent;
  int          m_OutputStep;
  FILE        *m_File;
};

#endif // QCEPFILEFORMATTERTEXT_H
