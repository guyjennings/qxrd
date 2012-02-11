#include "qxrddataprocessor.h"
#include <QtConcurrentRun>
#include "qxrdwindow.h"
#include "qxrdacquisition.h"
#include "qxrdimagedata.h"
#include "qxrdcenterfinder.h"
#include "qxrdintegrator.h"
#include "qxrdmutexlocker.h"
#include "qxrdgeneratetestimage.h"

#include "tiffio.h"

#include <QTime>
#include <QPainter>
#include <QDirIterator>
#include <math.h>

QxrdDataProcessor::QxrdDataProcessor
    (QxrdSettingsSaverWPtr saver,
     QxrdExperimentWPtr doc,
     QxrdAcquisitionWPtr acq,
     QxrdAllocatorWPtr allocator,
     QxrdFileSaverWPtr filesaver,
     QSettings *settings,
     QString section,
     QObject *parent)
  : QxrdDataProcessorThreaded(saver, doc, acq, allocator, filesaver, parent)
{
//  readSettings(settings, section);
}

QxrdDataProcessor::~QxrdDataProcessor()
{
}

void QxrdDataProcessor::init()
{
  if (m_GenerateTestImage) {
    m_GenerateTestImage -> setProcessor(this);
  }
}
