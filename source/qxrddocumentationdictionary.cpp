#include "qxrddocumentationdictionary.h"

QxrdDocumentationDictionary *gDocumentationDirectory = NULL;

QxrdDocumentationDictionary::QxrdDocumentationDictionary()
{
}

void QxrdDocumentationDictionary::setDoc(QString name, QString doc)
{
  m_Docs.insert(name, doc);
}

void QxrdDocumentationDictionary::setProto(QString name, QString proto)
{
  m_Protos.insert(name, proto);
}

QString QxrdDocumentationDictionary::doc(QString name)
{
  return m_Docs.value(name);
}

QString QxrdDocumentationDictionary::proto(QString name)
{
  return m_Protos.value(name);
}

QString QxrdDocumentationDictionary::get_Doc(QString name)
{
  if (gDocumentationDirectory) {
    return gDocumentationDirectory->doc(name);
  } else {
    return QString();
  }
}

QString QxrdDocumentationDictionary::get_Proto(QString name)
{
  if (gDocumentationDirectory) {
    return gDocumentationDirectory->proto(name);
  } else {
    return QString();
  }
}
