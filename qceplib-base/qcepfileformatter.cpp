#include "qcepfileformatter.h"
#include "qcepfileformatterhdf.h"
#include "qcepfileformatternexus.h"
#include "qcepfileformattertext.h"
#include "qcepserializableobject.h"

QcepFileFormatter::QcepFileFormatter(QString filePath) :
  QcepObject("fileFormatter"),
  m_FilePath(filePath)
{
}

QcepFileFormatterPtr QcepFileFormatter::defaultFormatter(QString filePath, QString exten)
{
#ifdef HAVE_HDF5
  if (exten == "h5") {
    return QcepFileFormatterPtr(new QcepFileFormatterHDF(filePath));
  }
#endif

#ifdef HAVE_NEXUS
  if (exten == "nxs") {
    return QcepFileFormatterPtr(new QcepFileFormatterNexus(filePath));
  }
#endif

  if (exten == "qxrdp2") {
    return QcepFileFormatterPtr(new QcepFileFormatterText(filePath));
  }

  return QcepFileFormatterPtr(new QcepFileFormatterText(filePath));
}

QcepSerializableObjectPtr QcepFileFormatter::construct(QString className)
{
  QcepSerializableObjectPtr res;

  int typeId = QMetaType::type(qPrintable(className+"*"));

  if (typeId == QMetaType::UnknownType) {
    printMessage(tr("Type %1 is unknown").arg(className));
  } else {
    const QMetaObject *obj = QMetaType::metaObjectForType(typeId);

    if (obj == NULL) {
      printMessage(tr("Metaobject is NULL"));
    } else {
      QObject *qobj = obj->newInstance();

      if (qobj == NULL) {
        printMessage(tr("qObject == NULL"));
      } else {
        QcepSerializableObject *qcobj = qobject_cast<QcepSerializableObject*>(qobj);

        if (qcobj == NULL) {
          printMessage(tr("QcepSerializableObject == NULL"));
        } else {
          res= QcepSerializableObjectPtr(qcobj);
        }
      }
    }
  }

  if (res) {
    printMessage(tr("Constructed %1:%2").arg(res->get_Name()).arg(res->className()));
  }

  return res;
}
