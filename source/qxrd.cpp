#include "qxrddebug.h"
#include <QApplication>

#include "qxrdapplication.h"
#include "qxrdapplication-ptr.h"
#include "qxrdwindow.h"
#include "qxrdsplashscreen.h"
#include "qxrdsplashscreen-ptr.h"
#include <stdio.h>
#include <QSplashScreen>
#include "qcepdataobject.h"

#if defined(Q_OS_UNIX) && !defined(Q_OS_MACX)
#include <mcheck.h>
#endif

/*!
\mainpage QXRD - Readout and Analysis software for SAXS/PDF/Powder X-Ray measurements

QXRD is software for the acquisition and analysis of X-ray data taken with 2 dimensional detectors.
The software can drive a Perkin Elmer XRD series flat panel detector and can be
remote-controlled via a socket interface, or directly from SPEC

*/

int main(int argc, char *argv[])
{
#if defined(Q_OS_UNIX) && !defined(Q_OS_MACX)
  mtrace();
#endif

  g_DebugLevel = QSharedPointer<QxrdDebugDictionary>(new QxrdDebugDictionary());

  if (qcepDebug(DEBUG_APP)) {
    printf("App starts\n");
  }

  int res = 0;

  {
    QxrdApplicationPtr app = QxrdApplicationPtr(
          new QxrdApplication(argc, argv));

    if (app->init(app, argc, argv)) {
      if (app->get_GuiWanted()) {
        res = app->exec();
      } else {
        app->processEvents();
        app->exit();
      }

      if (qcepDebug(DEBUG_EXITWAIT)) {
        while(1) {}
      }
    }
  }

  if (qcepDebug(DEBUG_APP)) {
    printf("App finishes\n");
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

    printf("%Ld bytes still allocated\n", allocated);
  }

#ifndef QT_NO_DEBUG
  if (nObjAlloc != nObjDeleted) {
    int i=0;
    foreach(QcepObject* obj, QcepObject::allocatedObjectsSet()) {
      printf("%d : %s\n", i++, qPrintable(obj->objectName()));
    }
  }
#endif

  return res;
}
