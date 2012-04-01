#ifndef QXRDDOCUMENTATIONFOROBJECT_H
#define QXRDDOCUMENTATIONFOROBJECT_H

#include <QString>

class QxrdDocumentationForObject
{
public:
  QxrdDocumentationForObject(QString objectName, QString objectDoc);
};

#define QXRD_DOC_OBJECT(name,doc) { QxrdDocumentationForObject d(name, doc); }

#endif // QXRDDOCUMENTATIONFOROBJECT_H
