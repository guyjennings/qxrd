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
     QxrdFileSaverWPtr filesaver)
  : QxrdDataProcessorThreaded(saver, doc, acq, allocator, filesaver)
{
}

QxrdDataProcessor::~QxrdDataProcessor()
{
}

void QxrdDataProcessor::initialize(QxrdDataProcessorWPtr proc)
{
  m_Processor = proc;

  if (m_GenerateTestImage) {
    m_GenerateTestImage -> setProcessor(m_Processor);
  }
}
