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
    (QxrdExperiment *doc,
     QxrdAcquisition *acq,
     QxrdAllocator *allocator,
     QxrdFileSaverThread *saver,
     QObject *parent)
  : QxrdDataProcessorThreaded(doc, acq, allocator, saver, parent)
{
}

QxrdDataProcessor::~QxrdDataProcessor()
{
}
