#include "qswdebug.h"
#include <QApplication>
#include "qceprunguard.h"
#include <QMessageBox>
#include "qswapplication.h"
#include "qswapplication-ptr.h"
#include "qcepallocator.h"

int main(int argc, char *argv[])
{
  QcepRunGuard guard("QSW");

  if ( !guard.tryToRun() ) {
    QApplication app(argc, argv);

    QMessageBox::critical(NULL, "Already Running",
                          "Another instance of QSW is already running");

    return 0;
  }

  g_DebugLevel = QSharedPointer<QswDebugDictionary>(NEWPTR(QswDebugDictionary()));

  if (qcepDebug(DEBUG_APP)) {
    printf("App starts\n");
  }

  int res = 0;

  {
    QswApplicationPtr app =
        QswApplicationPtr(
          NEWPTR(QswApplication(argc, argv)));

    if (app) {
      app -> initializeRoot();

      res = app -> exec();
    }
  }

  int nObjAlloc = QcepObject::allocatedObjects();
  int nObjDeleted = QcepObject::deletedObjects();

  int nDataAlloc = QcepDataObject::allocatedObjects();
  int nDataDeleted = QcepDataObject::deletedObjects();

  quint64 allocated = QcepAllocator::allocatedMemory();

#ifdef QT_NO_DEBUG
  bool printIt = (nObjAlloc != nObjDeleted) || (nDataAlloc != nDataDeleted) || (allocated != 0);
#else
  bool printIt = true;
#endif

  if (printIt) {
    printf("%d objects allocated\n", nObjAlloc);
    printf("%d objects deleted\n",   nObjDeleted);
    printf("%d objects leaked\n",    nObjAlloc - nObjDeleted);

    printf("%d data objects allocated\n", nDataAlloc);
    printf("%d data objects deleted\n",   nDataDeleted);
    printf("%d data objects leaked\n",    nDataAlloc - nDataDeleted);

    printf("%lld bytes still allocated\n", allocated);
  }

#ifndef QT_NO_DEBUG
  if (nObjAlloc != nObjDeleted) {
    int i=0;
    foreach(QcepObject* obj, QcepObject::allocatedObjectsSet()) {
      printf("%d : %s : %s\n",
             i++,
             qPrintable(obj->objectName()),
             qPrintable(obj->className()));
    }
  }
#endif

  return res;
}
