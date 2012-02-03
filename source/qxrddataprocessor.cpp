#include "qxrddataprocessor.h"
#include <QtConcurrentRun>
#include "qxrdwindow.h"
#include "qxrdacquisition.h"
#include "qxrdimagedata.h"
#include "qxrdcenterfinder.h"
#include "qxrdintegrator.h"
#include "qxrdmutexlocker.h"

#include "tiffio.h"

#include <QTime>
#include <QPainter>
#include <QDirIterator>
#include <math.h>

QxrdDataProcessor::QxrdDataProcessor
    (QxrdSettingsSaverPtr saver,
     QxrdExperimentPtr doc,
     QxrdAcquisitionPtr acq,
     QxrdAllocatorPtr allocator,
     QxrdFileSaverPtr filesaver,
     QSettings *settings,
     QString section,
     QObject *parent)
  : QxrdDataProcessorThreaded(saver, doc, acq, allocator, filesaver, parent)
{
  readSettings(settings, section);
}

QxrdDataProcessor::~QxrdDataProcessor()
{
}
