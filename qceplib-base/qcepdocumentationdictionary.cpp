#include "qcepdocumentationdictionary.h"

QcepDocumentationDictionary *gDocumentationDirectory = NULL;

QcepDocumentationDictionary::QcepDocumentationDictionary()
{
}

void QcepDocumentationDictionary::setProto(QString name, QString proto)
{
  m_Protos.insert(name, proto);
}

void QcepDocumentationDictionary::setDoc(QString name, QString doc)
{
  m_Docs.insert(name, doc);
}

void QcepDocumentationDictionary::setLongDoc(QString name, QString doc)
{
  m_LongDocs.insert(name, doc);
}

QString QcepDocumentationDictionary::proto(QString name)
{
  return m_Protos.value(name);
}

QString QcepDocumentationDictionary::doc(QString name)
{
  return m_Docs.value(name);
}

QString QcepDocumentationDictionary::longDoc(QString name)
{
  return m_LongDocs.value(name);
}

QString QcepDocumentationDictionary::get_Proto(QString name)
{
  if (gDocumentationDirectory) {
    return gDocumentationDirectory->proto(name);
  } else {
    return QString();
  }
}

QString QcepDocumentationDictionary::get_Doc(QString name)
{
  if (gDocumentationDirectory) {
    return gDocumentationDirectory->doc(name);
  } else {
    return QString();
  }
}

QString QcepDocumentationDictionary::get_LongDoc(QString name)
{
  if (gDocumentationDirectory) {
    return gDocumentationDirectory->longDoc(name);
  } else {
    return QString();
  }
}

void QcepDocumentationDictionary::set_Proto(QString name, QString proto)
{
  if (gDocumentationDirectory) {
    return gDocumentationDirectory->setProto(name, proto);
  }
}

void QcepDocumentationDictionary::set_Doc(QString name, QString doc)
{
  if (gDocumentationDirectory) {
    return gDocumentationDirectory->setDoc(name, doc);
  }
}

void QcepDocumentationDictionary::set_LongDoc(QString name, QString longDoc)
{
  if (gDocumentationDirectory) {
    return gDocumentationDirectory->setLongDoc(name, longDoc);
  }
}

QcepDocumentationForObject::QcepDocumentationForObject(
    QString objectName, QString objectDoc)
{
  if (gDocumentationDirectory == NULL) {
    gDocumentationDirectory = new QcepDocumentationDictionary();
  }

  gDocumentationDirectory->setDoc(objectName, objectDoc);
}

QcepLongDocumentationForObject::QcepLongDocumentationForObject(
    QString objectName, QString longDoc)
{
  if (gDocumentationDirectory == NULL) {
    gDocumentationDirectory = new QcepDocumentationDictionary();
  }

  gDocumentationDirectory->setLongDoc(objectName, longDoc);
}

QcepDocumentationForFunction::QcepDocumentationForFunction(
    QString functionName, QString functionProto, QString functionDoc, QString functionLongDoc)
  : QcepDocumentationForObject(functionName, functionDoc)
{
  gDocumentationDirectory->setProto(functionName, functionProto);
  gDocumentationDirectory->setLongDoc(functionName, functionLongDoc);
}

QHash<QString,QString> QcepDocumentationDictionary::docs()
{
  if (gDocumentationDirectory) {
    return gDocumentationDirectory->m_Docs;
  } else {
    return QHash<QString,QString>();
  }
}

QHash<QString,QString> QcepDocumentationDictionary::protos()
{
  if (gDocumentationDirectory) {
    return gDocumentationDirectory->m_Protos;
  } else {
    return QHash<QString,QString>();
  }
}

QHash<QString,QString> QcepDocumentationDictionary::longDocs()
{
  if (gDocumentationDirectory) {
    return gDocumentationDirectory->m_LongDocs;
  } else {
    return QHash<QString,QString>();
  }
}
