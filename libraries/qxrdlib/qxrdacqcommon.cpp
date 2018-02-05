#include "qxrdacqcommon.h"

QxrdAcqCommon::QxrdAcqCommon(QString name) :
  inherited(name)
{
}

QxrdAcqCommon::~QxrdAcqCommon()
{
}

void QxrdAcqCommon::initialize(QObjectWPtr parent)
{
  inherited::initialize(parent);
}
