#include "qxrddocumentationforobject.h"
#include "qxrddocumentationdictionary.h"

QxrdDocumentationForObject::QxrdDocumentationForObject(
    QString objectName, QString objectDoc)
{
  if (gDocumentationDirectory == NULL) {
    gDocumentationDirectory = new QxrdDocumentationDictionary();
  }

  gDocumentationDirectory->setDoc(objectName, objectDoc);
}
