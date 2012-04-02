#include "qxrddocumentationforfunction.h"
#include "qxrddocumentationdictionary.h"

QxrdDocumentationForFunction::QxrdDocumentationForFunction(
    QString functionName, QString functionProto, QString functionDoc)
  : QxrdDocumentationForObject(functionName, functionDoc)
{
  gDocumentationDirectory->setProto(functionName, functionProto);
}
