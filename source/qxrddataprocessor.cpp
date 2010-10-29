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
    (QxrdAcquisitionPtr acq, QxrdAllocatorPtr allocator, QxrdFileSaverThreadPtr saver, QObject *parent)
  : QObject(parent),
//    m_ProcessorType(this,"processorType",0),
//    m_ProcessorTypeName(this,"processorTypeName","processorType"),
    m_OutputDirectory(this,"outputDirectory", ""),
    m_FileName(this,"fileName",""),
    m_DataPath(this,"dataPath", ""),
    m_DarkImagePath(this, "darkImagePath", ""),
    m_BadPixelsPath(this, "badPixelsPath", ""),
    m_GainMapPath(this, "gainMapPath", ""),
    m_MaskPath(this, "maskPath", ""),
    m_LogFilePath(this, "logFilePath", "qxrd.log"),
    m_PerformDarkSubtraction(this, "performDarkSubtraction", true),
    m_SaveRawImages(this, "saveRawImages", true),
    m_SaveDarkImages(this, "saveDarkImages", true),
    m_PerformBadPixels(this, "performBadPixels", true),
    m_PerformGainCorrection(this, "performGainCorrection", true),
    m_SaveSubtracted(this, "saveSubtracted", true),
    m_SaveAsText(this, "saveAsText", false),
    m_SaveAsTextSeparator(this, "saveAsTextSeparator", " "),
    m_SaveAsTextPerLine(this,"saveAsTextPerLine",16),
    m_PerformIntegration(this, "performIntegration", true),
    m_DisplayIntegratedData(this, "displayIntegratedData", true),
    m_SaveIntegratedData(this, "saveIntegratedData", true),
    m_PerformDarkSubtractionTime(this, "performDarkSubtractionTime", 0.01),
    m_PerformBadPixelsTime(this, "performBadPixelsTime", 0.01),
    m_PerformGainCorrectionTime(this, "performGainCorrectionTime", 0.01),
    m_SaveSubtractedTime(this, "saveSubtractedTime", 0.1),
    m_SaveAsTextTime(this, "saveAsTextTime", 0.1),
    m_PerformIntegrationTime(this, "performIntegrationTime", 0.05),
    m_DisplayIntegratedDataTime(this, "displayIntegratedDataTime", 0.2),
    m_SaveIntegratedDataTime(this, "saveIntegratedDataTime", 0.01),
    m_EstimatedProcessingTime(this, "estimatedProcessingTime", 0.1),
    m_AveragingRatio(this, "averagingRatio", 0.1),
//    m_FileName(this,"fileName",""),
    m_MaskMinimumValue(this, "maskMinimumValue", 0),
    m_MaskMaximumValue(this, "maskMaximumValue", 20000),
    m_MaskCircleRadius(this, "maskCircleRadius", 10),
    m_MaskSetPixels(this, "maskSetPixels", true),
    m_CompressImages(this, "compressImages", false),
    m_Average(this,"average",0.0),
    m_AverageDark(this,"averageDark",0.0),
    m_AverageRaw(this,"averageRaw",0.0)
{
}

QxrdDataProcessor::~QxrdDataProcessor()
{
}
