#include "qcepapplication.h"

QcepApplication *g_Application = NULL;

QcepApplication::QcepApplication(int &argc, char **argv) :
  inherited(argc, argv)
{
  g_Application = this;
}

void QcepApplication::initialize()
{
}
