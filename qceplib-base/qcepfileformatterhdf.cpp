#ifdef HAVE_HDF5

#include "qcepfileformatterhdf.h"

QcepFileFormatterHDF::QcepFileFormatterHDF(QString filePath) :
  inherited(filePath)
{

}

void QcepFileFormatterHDF::beginWriteFile()
{
}

void QcepFileFormatterHDF::endWriteFile()
{
}

void QcepFileFormatterHDF::writeComment(QString /*cmt*/)
{
}

void QcepFileFormatterHDF::beginWriteObject(QString /*objectName*/, QString /*className*/)
{
}

void QcepFileFormatterHDF::endWriteObject()
{
}

void QcepFileFormatterHDF::beginWriteProperties()
{
}

void QcepFileFormatterHDF::writeProperty(QString /*name*/, QVariant /*val*/)
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

QcepObjectPtr QcepFileFormatterHDF::nextObject()
{
  return QcepObjectPtr();
}

void QcepFileFormatterHDF::beginReadObject(QcepObjectPtr /*obj*/)
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

QcepObjectPtr QcepFileFormatterHDF::nextChild()
{
  return QcepObjectPtr();
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
