#include "qxrddebug.h"
#include <QApplication>

#include "qxrdapplication.h"
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
//  printf("App starts\n");
  g_DebugLevel = QSharedPointer<QxrdDebugDictionary>(new QxrdDebugDictionary());

  QxrdApplication app(argc, argv);

  QxrdSplashScreenPtr splash;

  if (app.get_GuiWanted()) {
    splash = QxrdSplashScreenPtr(new QxrdSplashScreen(NULL));

    splash->show();
    QFont f;
    f.setPointSize(14);
    splash->setFont(f);
    splash->showMessage("Qxrd Version " STR(QXRD_VERSION) "\nInitializing QXRD, Please Wait...", Qt::AlignBottom | Qt::AlignHCenter);
    app.processEvents();
  }

  int res = 0;

  if (app.init(argc, argv)) {
    if (app.get_GuiWanted()) {
      splash->finish(app.window());
      res = app.exec();
    } else {
      app.processEvents();
      app.exit();
    }

    if (qcepDebug(DEBUG_EXITWAIT)) {
      while(1) {}
    }
  }
  return res;
}
