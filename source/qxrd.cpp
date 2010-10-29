#include <QApplication>

#include "qxrdapplication.h"
#include <stdio.h>

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
