#include "qxrddetectorsettingssimulated.h"
#include "qxrdexperiment.h"
#include "qxrdacquisition.h"
#include "qcepallocator.h"
#include "qxrddebug.h"

#include <stdio.h>
#include <QPainter>

QxrdDetectorSettingsSimulated::QxrdDetectorSettingsSimulated(QxrdApplicationWPtr app,
                                                             QxrdExperimentWPtr    expt,
                                                             QxrdAcquisitionWPtr   acq,
                                                             int                   detNum) :
  QxrdDetectorSettings(app, expt, acq, SimulatedDetector, detNum)
{
#ifndef QT_NO_DEBUG
  printf("Constructing simulated detector\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorSimulated::QxrdDetectorSimulated(%p)\n", this);
  }
}

QxrdDetectorSettingsSimulated::~QxrdDetectorSettingsSimulated()
{
#ifndef QT_NO_DEBUG
  printf("Deleting simulated detector\n");
#endif

  if (QThread::currentThread() != thread()) {
    printf("Deleting simulated detector from wrong thread\n");
  }

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorSimulated::~QxrdDetectorSimulated(%p)\n", this);
  }
}

void QxrdDetectorSettingsSimulated::pushDefaultsToProxy(QxrdDetectorProxyPtr proxy)
{
//  QxrdDetectorSettings::pushDefaultsToProxy(proxy, SimulatedDetector);

//  printf("Need to implement QxrdDetectorSimulated::pushDefaultsToProxy\n");
}

void QxrdDetectorSettingsSimulated::pushPropertiesToProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetectorSettings::pushPropertiesToProxy(proxy);

//  printf("Need to implement QxrdDetectorSimulated::pushPropertiesToProxy\n");
}

void QxrdDetectorSettingsSimulated::pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetectorSettings::pullPropertiesfromProxy(proxy);

//  printf("Need to implement QxrdDetectorSimulated::pullPropertiesfromProxy\n");
}

