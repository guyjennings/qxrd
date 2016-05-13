#ifdef HAVE_NEXUS

#include "qcepfileformatternexus.h"

QcepFileFormatterNexus::QcepFileFormatterNexus(QString filePath)
  : QcepFileFormatter(filePath)
{

}

void QcepFileFormatterNexus::beginWriteObject(QString objectName, QString className)
{
}

void QcepFileFormatterNexus::endWriteObject()
{
}

void QcepFileFormatterNexus::writeComment(QString cmt)
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

QcepObjectPtr QcepFileFormatterNexus::nextObject()
{
  return QcepObjectPtr();
}

#endif
