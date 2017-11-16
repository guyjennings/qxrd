#include "qxrddetectorsettingsdexela.h"
#include "qxrdexperiment.h"
#include "qxrdacquisition.h"
#include "qcepallocator.h"
#include "qxrddebug.h"

#include <stdio.h>
#include <QPainter>

QxrdDetectorSettingsDexela::QxrdDetectorSettingsDexela(QxrdApplicationWPtr app,
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

QxrdDetectorSettingsDexela::~QxrdDetectorSettingsDexela()
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

void QxrdDetectorSettingsDexela::pushDefaultsToProxy(QxrdDetectorProxyPtr proxy)
{
//  QxrdDetectorSettings::pushDefaultsToProxy(proxy, DexelaDetector);

//  printf("Need to implement QxrdDetectorDexela::pushDefaultsToProxy\n");
}

void QxrdDetectorSettingsDexela::pushPropertiesToProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetectorSettings::pushPropertiesToProxy(proxy);

//  printf("Need to implement QxrdDetectorDexela::pushPropertiesToProxy\n");
}

void QxrdDetectorSettingsDexela::pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetectorSettings::pullPropertiesfromProxy(proxy);

//  printf("Need to implement QxrdDetectorDexela::pullPropertiesfromProxy\n");
}

