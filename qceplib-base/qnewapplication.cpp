#include "qnewapplication.h"

QNewApplication::QNewApplication(int &argc, char **argv)
  : inherited(argc, argv)
{

}

bool QNewApplication::event(QEvent *e)
{
  return inherited::event(e);
}
