//#ifdef HAVE_NEXUS

#include "qcepfileformatternexus.h"

QcepFileFormatterNexus::QcepFileFormatterNexus(QString filePath)
  : QcepFileFormatter(filePath)
{

}

void QcepFileFormatterNexus::beginWriteFile()
{
}

void QcepFileFormatterNexus::endWriteFile()
{
}

void QcepFileFormatterNexus::writeComment(QString cmt)
{
}

void QcepFileFormatterNexus::beginWriteObject(QString objectName, QString className)
{
}

void QcepFileFormatterNexus::endWriteObject()
{
}

void QcepFileFormatterNexus::beginWriteProperties()
{
}

void QcepFileFormatterNexus::writeProperty(QString name, QVariant val)
{
}

void QcepFileFormatterNexus::endWriteProperties()
{
}

void QcepFileFormatterNexus::beginWriteChildren()
{
}

void QcepFileFormatterNexus::endWriteChildren()
{
}

void QcepFileFormatterNexus::beginWriteData()
{
}

void QcepFileFormatterNexus::endWriteData()
{
}

void QcepFileFormatterNexus::beginReadFile()
{
}

void QcepFileFormatterNexus::endReadFile()
{
}

QcepSerializableObjectPtr QcepFileFormatterNexus::nextObject()
{
  return QcepSerializableObjectPtr();
}

void QcepFileFormatterNexus::beginReadObject(QcepSerializableObjectPtr obj)
{
}

void QcepFileFormatterNexus::endReadObject()
{
}

bool QcepFileFormatterNexus::beginReadProperties()
{
  return false;
}

QString QcepFileFormatterNexus::nextPropertyName()
{
  return "";
}

QVariant QcepFileFormatterNexus::nextPropertyValue()
{
  return QVariant();
}

void QcepFileFormatterNexus::endReadProperties()
{
}

bool QcepFileFormatterNexus::beginReadChildren()
{
  return false;
}

QcepSerializableObjectPtr QcepFileFormatterNexus::nextChild()
{
  return QcepSerializableObjectPtr();
}

void QcepFileFormatterNexus::endReadChildren()
{
}

bool QcepFileFormatterNexus::beginReadData()
{
  return false;
}

void QcepFileFormatterNexus::endReadData()
{
}

//#endif
