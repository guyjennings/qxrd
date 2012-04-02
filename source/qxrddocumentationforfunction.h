#ifndef QXRDDOCUMENTATIONFORFUNCTION_H
#define QXRDDOCUMENTATIONFORFUNCTION_H

#include "qxrddocumentationforobject.h"

class QxrdDocumentationForFunction : public QxrdDocumentationForObject
{
public:
  QxrdDocumentationForFunction(QString functionName, QString functionProto, QString functionDoc);
};

#define QXRD_DOC_FUNCTION(name, proto, doc) { QxrdDocumentationForFunction doc(name, proto, doc); }

#endif // QXRDDOCUMENTATIONFORFUNCTION_H
