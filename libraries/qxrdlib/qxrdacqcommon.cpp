#include "qxrdacqcommon.h"

QxrdAcqCommon::QxrdAcqCommon(QString name) :
  inherited(name)
{
}

QxrdAcqCommon::~QxrdAcqCommon()
{
}

void QxrdAcqCommon::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}
