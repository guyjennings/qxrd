#include "qcepfileformattertext.h"

QcepFileFormatterText::QcepFileFormatterText(QString filePath) :
  QcepFileFormatter(filePath),
  m_OutputIndent(0)
{
}

void QcepFileFormatterText::beginWriteObject(QString objectName, QString className)
{
  printMessage(tr("%1%2:%3 {").arg("", m_OutputIndent).arg(objectName).arg(className));
  m_OutputIndent++;
  m_ObjectPath.append(objectName);
}

void QcepFileFormatterText::endWriteObject()
{
  m_ObjectPath.takeLast();
  m_OutputIndent--;
  printMessage(tr("%1}").arg("", m_OutputIndent));
}

void QcepFileFormatterText::beginWriteProperties()
{
  printMessage(tr("%1properties {").arg("", m_OutputIndent));
  m_OutputIndent++;
}

void QcepFileFormatterText::writeProperty(QString name, QVariant val)
{
  printMessage(tr("%1%2 = %3").arg("",m_OutputIndent).arg(name).arg(val.toString()));
}

void QcepFileFormatterText::endWriteProperties()
{
  m_OutputIndent--;
  printMessage(tr("%1}").arg("", m_OutputIndent));
}

void QcepFileFormatterText::beginWriteChildren()
{
  printMessage(tr("%1children {").arg("", m_OutputIndent));
  m_OutputIndent++;
}

void QcepFileFormatterText::endWriteChildren()
{
  m_OutputIndent--;
  printMessage(tr("%1}").arg("", m_OutputIndent));
}

void QcepFileFormatterText::beginWriteData()
{
  printMessage(tr("%1data {").arg("", m_OutputIndent));
  m_OutputIndent++;
}

void QcepFileFormatterText::endWriteData()
{
  m_OutputIndent--;
  printMessage(tr("%1}").arg("", m_OutputIndent));
}
