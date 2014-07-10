#include "qxrddebug.h"
#include <QApplication>

#include "qxrdapplication.h"
#include "qxrdapplication-ptr.h"
#include "qxrdwindow.h"
#include "qxrdsplashscreen.h"
#include "qxrdsplashscreen-ptr.h"
#include <stdio.h>
#include <QSplashScreen>

/*!
\mainpage QXRD - Readout and Analysis software for SAXS/PDF/Powder X-Ray measurements

QXRD is software for the acquisition and analysis of X-ray data taken with 2 dimensional detectors.
The software can drive a Perkin Elmer XRD series flat panel detector and can be
remote-controlled via a socket interface, or directly from SPEC

*/

int main(int argc, char *argv[])
{
  g_DebugLevel = QSharedPointer<QxrdDebugDictionary>(new QxrdDebugDictionary());

  if (qcepDebug(DEBUG_APP)) {
    printf("App starts\n");
  }

  QxrdApplicationPtr app = QxrdApplicationPtr(
        new QxrdApplication(argc, argv));

  int res = 0;

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

  if (qcepDebug(DEBUG_APP)) {
    printf("App finishes\n");
  }

  return res;
}
