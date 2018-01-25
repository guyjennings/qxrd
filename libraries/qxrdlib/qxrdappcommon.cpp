#include "qxrdappcommon.h"

QxrdAppCommon::QxrdAppCommon(int &argc, char **argv)
  : QcepApplication(argc, argv)
{
}

QxrdAppCommon::~QxrdAppCommon()
{
}

bool QxrdAppCommon::init(int &argc, char **argv)
{
  return true;
}
