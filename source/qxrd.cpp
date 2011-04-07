#include <QApplication>

#include "qxrdapplication.h"
#include "qxrdwindow.h"

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

  QxrdApplication app(argc, argv);

  QPixmap pixmap(":images/qxrd-splash-screen.png");
  QSplashScreen splash(pixmap);
  splash.show();
  QFont f;
  f.setPointSize(14);
  splash.setFont(f);
  splash.showMessage("Qxrd Version " QXRD_VERSION "\nInitializing QXRD, Please Wait...", Qt::AlignBottom | Qt::AlignHCenter);

  int res = 0;
  app.processEvents();

  if (app.init(&splash)) {

    //  printf("App Constructed\n");

    splash.finish(app.window());

    res = app.exec();

    if (qcepDebug(DEBUG_EXITWAIT)) {
      while(1) {}
    }
  }
  return res;
}
