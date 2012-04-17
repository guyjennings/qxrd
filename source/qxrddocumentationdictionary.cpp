#include "qxrddocumentationdictionary.h"

QxrdDocumentationDictionary *gDocumentationDirectory = NULL;

QxrdDocumentationDictionary::QxrdDocumentationDictionary()
{
}

void QxrdDocumentationDictionary::setProto(QString name, QString proto)
{
  m_Protos.insert(name, proto);
}

void QxrdDocumentationDictionary::setDoc(QString name, QString doc)
{
  m_Docs.insert(name, doc);
}

void QxrdDocumentationDictionary::setLongDoc(QString name, QString doc)
{
  m_LongDocs.insert(name, doc);
}

QString QxrdDocumentationDictionary::proto(QString name)
{
  return m_Protos.value(name);
}

QString QxrdDocumentationDictionary::doc(QString name)
{
  return m_Docs.value(name);
}

QString QxrdDocumentationDictionary::longDoc(QString name)
{
  return m_LongDocs.value(name);
}

QString QxrdDocumentationDictionary::get_Proto(QString name)
{
  if (gDocumentationDirectory) {
    return gDocumentationDirectory->proto(name);
  } else {
    return QString();
  }
}

QString QxrdDocumentationDictionary::get_Doc(QString name)
{
  if (gDocumentationDirectory) {
    return gDocumentationDirectory->doc(name);
  } else {
    return QString();
  }
}

QString QxrdDocumentationDictionary::get_LongDoc(QString name)
{
  if (gDocumentationDirectory) {
    return gDocumentationDirectory->longDoc(name);
  } else {
    return QString();
  }
}

void QxrdDocumentationDictionary::set_Proto(QString name, QString proto)
{
  if (gDocumentationDirectory) {
    return gDocumentationDirectory->setProto(name, proto);
  }
}

void QxrdDocumentationDictionary::set_Doc(QString name, QString doc)
{
  if (gDocumentationDirectory) {
    return gDocumentationDirectory->setDoc(name, doc);
  }
}

void QxrdDocumentationDictionary::set_LongDoc(QString name, QString longDoc)
{
  if (gDocumentationDirectory) {
    return gDocumentationDirectory->setLongDoc(name, longDoc);
  }
}

QxrdDocumentationForObject::QxrdDocumentationForObject(
    QString objectName, QString objectDoc)
{
  if (gDocumentationDirectory == NULL) {
    gDocumentationDirectory = new QxrdDocumentationDictionary();
  }

  gDocumentationDirectory->setDoc(objectName, objectDoc);
}

QxrdLongDocumentationForObject::QxrdLongDocumentationForObject(
    QString objectName, QString longDoc)
{
  if (gDocumentationDirectory == NULL) {
    gDocumentationDirectory = new QxrdDocumentationDictionary();
  }

  gDocumentationDirectory->setLongDoc(objectName, longDoc);
}

QxrdDocumentationForFunction::QxrdDocumentationForFunction(
    QString functionName, QString functionProto, QString functionDoc, QString functionLongDoc)
  : QxrdDocumentationForObject(functionName, functionDoc)
{
  gDocumentationDirectory->setProto(functionName, functionProto);
  gDocumentationDirectory->setLongDoc(functionName, functionLongDoc);
}
