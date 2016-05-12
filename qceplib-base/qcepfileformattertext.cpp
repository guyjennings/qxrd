#include "qcepfileformattertext.h"

QcepFileFormatterText::QcepFileFormatterText(QString filePath) :
  QcepFileFormatter(filePath),
  m_OutputIndent(0),
  m_OutputStep(2),
  m_File(NULL)
{
}

void QcepFileFormatterText::beginWriteFile()
{
  openForWriting();
}

void QcepFileFormatterText::endWriteFile()
{
  closeForWriting();
}

void QcepFileFormatterText::writeIndent()
{
  writeToken(tr("%1").arg("",m_OutputIndent*m_OutputStep));
}

void QcepFileFormatterText::writeToken(QString tok)
{
  fputs(qPrintable(tok), m_File);
}

void QcepFileFormatterText::writeVariant(QVariant val)
{
  QString s = toScriptLiteral(val);

  writeToken(s);
}

void QcepFileFormatterText::beginWriteObject(QString objectName, QString className)
{
  printMessage(tr("%1%2:%3 {").arg("", m_OutputIndent).arg(objectName).arg(className));

  writeIndent();
  writeToken(objectName);
  writeToken(":");
  writeToken(className);
  writeToken(" {\n");

  m_OutputIndent++;
  m_ObjectPath.append(objectName);
}

void QcepFileFormatterText::endWriteObject()
{
  m_ObjectPath.takeLast();
  m_OutputIndent--;

  printMessage(tr("%1}").arg("", m_OutputIndent));

  writeIndent();
  writeToken("}\n");
}

void QcepFileFormatterText::beginWriteProperties()
{
  printMessage(tr("%1properties {").arg("", m_OutputIndent));

  writeIndent();
  writeToken("properties {\n");

  m_OutputIndent++;
}

void QcepFileFormatterText::writeProperty(QString name, QVariant val)
{
  printMessage(tr("%1%2 = %3").arg("",m_OutputIndent).arg(name).arg(toScriptLiteral(val)));

  writeIndent();
  writeToken(name);
  writeToken(" = ");
  writeVariant(val);
  writeToken("\n");
}

void QcepFileFormatterText::endWriteProperties()
{
  m_OutputIndent--;

  printMessage(tr("%1}").arg("", m_OutputIndent));

  writeIndent();
  writeToken("}\n");
}

void QcepFileFormatterText::beginWriteChildren()
{
  printMessage(tr("%1children {").arg("", m_OutputIndent));

  writeIndent();
  writeToken("children {\n");

  m_OutputIndent++;
}

void QcepFileFormatterText::endWriteChildren()
{
  m_OutputIndent--;

  printMessage(tr("%1}").arg("", m_OutputIndent));

  writeIndent();
  writeToken("}\n");
}

void QcepFileFormatterText::beginWriteData()
{
  printMessage(tr("%1data {").arg("", m_OutputIndent));

  writeIndent();
  writeToken("data {\n");

  m_OutputIndent++;
}

void QcepFileFormatterText::endWriteData()
{
  m_OutputIndent--;

  printMessage(tr("%1}").arg("", m_OutputIndent));

  writeIndent();
  writeToken("}\n");
}

void QcepFileFormatterText::openForWriting()
{
  m_File = fopen(qPrintable(m_FilePath), "w+");

  if (m_File == NULL) {
    printMessage(tr("File open of %1 failed").arg(m_FilePath));
  }
}

void QcepFileFormatterText::closeForWriting()
{
  fclose(m_File);
}

QcepFileFormatterText::TokenType QcepFileFormatterText::nextToken(QString &token)
{
}

QcepFileFormatterText::TokenType QcepFileFormatterText::numberToken(QString &token)
{
}

QcepFileFormatterText::TokenType QcepFileFormatterText::stringToken(QString &token)
{
}

QcepFileFormatterText::TokenType QcepFileFormatterText::operatorToken(QString &token)
{
}

QcepFileFormatterText::TokenType QcepFileFormatterText::identifierToken(QString &token)
{
}
