#include "qxrdsimulatedsettings.h"
#include "qxrdexperiment.h"
#include "qxrdacquisition.h"
#include "qcepallocator.h"
#include "qxrddebug.h"
#include "qxrdsimulateddialog.h"
#include <stdio.h>
#include <QPainter>

QxrdSimulatedSettings::QxrdSimulatedSettings(QString name) :
  QxrdDetectorSettings(name, SimulatedDetector)
{
#ifndef QT_NO_DEBUG
  printf("Constructing simulated detector\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorSimulated::QxrdDetectorSimulated(%p)\n", this);
  }
}

QxrdSimulatedSettings::~QxrdSimulatedSettings()
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

void QxrdSimulatedSettings::pushDefaultsToProxy(QxrdDetectorProxyPtr /*proxy*/)
{
//  QxrdDetectorSettings::pushDefaultsToProxy(proxy, SimulatedDetector);

//  printf("Need to implement QxrdDetectorSimulated::pushDefaultsToProxy\n");
}

void QxrdSimulatedSettings::pushPropertiesToProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetectorSettings::pushPropertiesToProxy(proxy);

//  printf("Need to implement QxrdDetectorSimulated::pushPropertiesToProxy\n");
}

void QxrdSimulatedSettings::pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetectorSettings::pullPropertiesfromProxy(proxy);

//  printf("Need to implement QxrdDetectorSimulated::pullPropertiesfromProxy\n");
}

void QxrdSimulatedSettings::configureDetector()
{
  QxrdSimulatedDialog *dlog =
      new QxrdSimulatedDialog();

  if (dlog) {
    dlog->exec();
  }
}
