#include "qcepfileformatter.h"
#include "qcepfileformatterhdf.h"
#include "qcepfileformatternexus.h"
#include "qcepfileformattertext.h"
#include "qcepmacros.h"
#include "qcepdebug.h"

QcepFileFormatter::QcepFileFormatter(QString filePath) :
  inherited("fileFormatter"),
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

QcepObjectPtr QcepFileFormatter::construct(QString name, QString className)
{
  QcepObjectPtr res;

  int typeId = QMetaType::type(qPrintable(className+"*"));

  if (typeId == QMetaType::UnknownType) {
    printMessage(tr("Type %1 is unknown").arg(className));
  } else {
    const QMetaObject *obj = QMetaType::metaObjectForType(typeId);

    if (obj == NULL) {
      printMessage(tr("Metaobject is NULL"));
    } else {
      QObject *qobj = obj->newInstance(Q_ARG(QString, name));

      if (qobj == NULL) {
        printMessage(tr("qObject == NULL"));
      } else {
        QcepObject* qcobj = qobject_cast<QcepObject*>(qobj);

        if (qcobj == NULL) {
          printMessage(tr("QcepObject == NULL"));
        } else {
          res= QcepObjectPtr(qcobj);
        }
      }
    }
  }

  if (qcepDebug(DEBUG_IMPORT) && res) {
    printMessage(tr("Constructed %1:%2").arg(res->get_Name()).arg(res->className()));
  }

  return res;
}
