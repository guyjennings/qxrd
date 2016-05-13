#ifdef HAVE_HDF5

#include "qcepfileformatterhdf.h"

QcepFileFormatterHDF::QcepFileFormatterHDF(QString filePath) :
  QcepFileFormatter(filePath)
{

}

void QcepFileFormatterHDF::beginWriteObject(QString objectName, QString className)
{
}

void QcepFileFormatterHDF::endWriteObject()
{
}

void QcepFileFormatterHDF::writeComment(QString cmt)
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

QcepObjectPtr QcepFileFormatterHDF::nextObject()
{
  return QcepObjectPtr();
}

#endif
