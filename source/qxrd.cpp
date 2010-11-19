#include <QApplication>

#include "qxrdapplication.h"
#include <stdio.h>

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

//  printf("App Constructed\n");

  int res = app.exec();

  QCEP_DEBUG(DEBUG_EXITWAIT,
             while(1) {}
  );

  return res;
}
