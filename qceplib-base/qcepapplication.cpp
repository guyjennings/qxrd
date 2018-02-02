#include "qcepapplication.h"

QcepApplication *g_Application = NULL;

QcepApplication::QcepApplication(int &argc, char **argv) :
  QApplication(argc, argv)
{
  g_Application = this;
}

void QcepApplication::initialize()
{
}
