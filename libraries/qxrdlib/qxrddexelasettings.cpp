#include "qxrddexelasettings.h"
#include "qxrdexperiment.h"
#include "qxrdacquisition.h"
#include "qcepallocator.h"
#include "qxrddebug.h"

#include <stdio.h>
#include <QPainter>

QxrdDexelaSettings::QxrdDexelaSettings(QxrdApplicationWPtr app,
                                                             QxrdExperimentWPtr    expt,
                                                             QxrdAcquisitionWPtr   acq,
                                                             int                   detNum) :
  QxrdDetectorSettings(app, expt, acq, DexelaDetector, detNum)
{
#ifndef QT_NO_DEBUG
  printf("Constructing dexela detector\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorDexela::QxrdDetectorDexela(%p)\n", this);
  }
}

QxrdDexelaSettings::~QxrdDexelaSettings()
{
#ifndef QT_NO_DEBUG
  printf("Deleting Dexela detector\n");
#endif

  if (QThread::currentThread() != thread()) {
    printf("Deleting Dexela detector from wrong thread\n");
  }

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorDexela::~QxrdDetectorDexela(%p)\n", this);
  }
}

void QxrdDexelaSettings::pushDefaultsToProxy(QxrdDetectorProxyPtr /*proxy*/)
{
//  QxrdDetectorSettings::pushDefaultsToProxy(proxy, DexelaDetector);

//  printf("Need to implement QxrdDetectorDexela::pushDefaultsToProxy\n");
}

void QxrdDexelaSettings::pushPropertiesToProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetectorSettings::pushPropertiesToProxy(proxy);

//  printf("Need to implement QxrdDetectorDexela::pushPropertiesToProxy\n");
}

void QxrdDexelaSettings::pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetectorSettings::pullPropertiesfromProxy(proxy);

//  printf("Need to implement QxrdDetectorDexela::pullPropertiesfromProxy\n");
}

