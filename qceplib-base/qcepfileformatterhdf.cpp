#ifdef HAVE_HDF5

#include "qcepfileformatterhdf.h"

QcepFileFormatterHDF::QcepFileFormatterHDF(QString filePath) :
  QcepFileFormatter(filePath)
{

}

void QcepFileFormatterHDF::beginWriteFile()
{
}

void QcepFileFormatterHDF::endWriteFile()
{
}

void QcepFileFormatterHDF::writeComment(QString cmt)
{
}

void QcepFileFormatterHDF::beginWriteObject(QString objectName, QString className)
{
}

void QcepFileFormatterHDF::endWriteObject()
{
}

void QcepFileFormatterHDF::beginWriteProperties()
{
}

void QcepFileFormatterHDF::writeProperty(QString name, QVariant val)
{
}

void QcepFileFormatterHDF::endWriteProperties()
{
}

void QcepFileFormatterHDF::beginWriteChildren()
{
}

void QcepFileFormatterHDF::endWriteChildren()
{
}

void QcepFileFormatterHDF::beginWriteData()
{
}

void QcepFileFormatterHDF::endWriteData()
{
}

void QcepFileFormatterHDF::beginReadFile()
{
}

void QcepFileFormatterHDF::endReadFile()
{
}

QcepSerializableObjectPtr QcepFileFormatterHDF::nextObject()
{
  return QcepSerializableObjectPtr();
}

void QcepFileFormatterHDF::beginReadObject(QcepSerializableObjectPtr obj)
{
}

void QcepFileFormatterHDF::endReadObject()
{
}

bool QcepFileFormatterHDF::beginReadProperties()
{
  return false;
}

QString QcepFileFormatterHDF::nextPropertyName()
{
  return "";
}

QVariant QcepFileFormatterHDF::nextPropertyValue()
{
  return QVariant();
}

void QcepFileFormatterHDF::endReadProperties()
{
}

bool QcepFileFormatterHDF::beginReadChildren()
{
  return false;
}

QcepSerializableObjectPtr QcepFileFormatterHDF::nextChild()
{
  return QcepSerializableObjectPtr();
}

void QcepFileFormatterHDF::endReadChildren()
{
}

bool QcepFileFormatterHDF::beginReadData()
{
  return false;
}

void QcepFileFormatterHDF::endReadData()
{
}

#endif
