#include "qxrddataprocessor.h"
#include <QtConcurrentRun>
#include "qxrdwindow.h"
#include "qxrdacquisition.h"
#include "qcepimagedata.h"
#include "qxrdcenterfinder.h"
#include "qxrdintegrator.h"
#include "qcepmutexlocker.h"
#include "qxrdgeneratetestimage.h"

#include "tiffio.h"

#include <QTime>
#include <QPainter>
#include <QDirIterator>
#include <qmath.h>

QxrdDataProcessor::QxrdDataProcessor
    (QcepSettingsSaverWPtr saver,
     QxrdExperimentWPtr doc,
     QxrdAcquisitionWPtr acq,
     QcepAllocatorWPtr allocator,
     QxrdFileSaverWPtr filesaver)
  : QxrdDataProcessorThreaded(saver, doc, acq, filesaver)
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
