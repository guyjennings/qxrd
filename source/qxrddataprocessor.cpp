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

QxrdDataProcessor::QxrdDataProcessor(QString name)
  : QxrdDataProcessorThreaded(name)
{
}

QxrdDataProcessor::QxrdDataProcessor()
  : QxrdDataProcessor("processor")
{
}

QxrdDataProcessor::~QxrdDataProcessor()
{
}

//void QxrdDataProcessor::initialize(QxrdExperimentWPtr doc,
//                                   QxrdAcquisitionWPtr acq,
//                                   QxrdFileSaverWPtr fileSaver)
//{
//  QxrdDataProcessorThreaded::initialize(doc, acq, fileSaver);

//  if (m_GenerateTestImage) {
//    m_GenerateTestImage -> setProcessor(qSharedPointerDynamicCast<QxrdDataProcessor>(sharedFromThis()));
//  }
//}
