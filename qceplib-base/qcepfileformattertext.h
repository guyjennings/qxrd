#ifndef QCEPFILEFORMATTERTEXT_H
#define QCEPFILEFORMATTERTEXT_H

#include "qceplib_global.h"
#include "qcepfileformatter.h"
#include <QFile>
#include <QTextStream>
#include <stdio.h>

class QCEP_EXPORT QcepFileFormatterText : public QcepFileFormatter
{
public:
  QcepFileFormatterText(QString filePath);

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

  typedef enum {
    EndOfFile,
    Number,
    String,
    Operator,
    Identifier,
    Variant
  } TokenType;



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

private:
  void    openForWriting();
  void    closeForWriting();

  void    writeIndent();
  void    writeToken(QString tok);
  void    writeVariant(QVariant val);

  void    openForReading();
  void    closeForReading();

  QChar skipWhiteSpace();
  QChar nextChar();
  void  ungetChar(QChar ch);

  void nextToken();

  TokenType  nextToken(QString &token);
  TokenType  numberToken(QString &token);
  TokenType  stringToken(QString &token);
  TokenType  operatorToken(QString &token);
  TokenType  identifierToken(QString &token);
  TokenType  variantToken(QString &token);

private:
  int          m_OutputIndent;
  int          m_OutputStep;
  FILE        *m_File;
  TokenType    m_TokenType;
  QString      m_Token;
};

#endif // QCEPFILEFORMATTERTEXT_H
