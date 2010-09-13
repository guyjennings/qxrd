/******************************************************************
*
*  $Id: qxrddataprocessorthreaded.h,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

#ifndef QXRDDATAPROCESSORTHREADED_H
#define QXRDDATAPROCESSORTHREADED_H

#include "qcepmacros.h"

#include "qxrddataprocessorbase.h"
#include "qxrdresultserializer.h"

class QxrdDataProcessorThreaded : public QxrdDataProcessorBase
{
  Q_OBJECT;
public:
  QxrdDataProcessorThreaded(QxrdAcquisitionPtr acq, QxrdAllocatorPtr allocator, QxrdFileSaverThreadPtr saver, QObject *parent=0);

public:
  void beginAcquisition(int isDark);
  void idleInt16Image(QxrdInt16ImageDataPtr image);
  void acquiredInt16Image(QxrdInt16ImageDataPtr image);
  void acquiredInt32Image(QxrdInt32ImageDataPtr image);

private slots:
  void onCorrectedImageAvailable();
  void onIntegratedDataAvailable();
  void onROIDataAvailable();
  void onHistogramDataAvailable();

private:
  QxrdDoubleImageDataPtr correctInt16Image(QxrdInt16ImageDataPtr image, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask);
  QxrdDoubleImageDataPtr correctInt32Image(QxrdInt32ImageDataPtr image, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask);
  QxrdIntegratedDataPtr  integrateImage(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr mask, double cx, double cy);
  QxrdROIDataPtr         calculateROI(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr mask);
  QxrdHistogramDataPtr   calculateHistogram(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr mask);

private:
  QxrdResultSerializer<QxrdDoubleImageDataPtr>  m_CorrectedImages;
  QxrdResultSerializer<QxrdIntegratedDataPtr>   m_IntegratedData;
  QxrdResultSerializer<QxrdROIDataPtr>          m_ROIData;
  QxrdResultSerializer<QxrdHistogramDataPtr>    m_HistogramData;

protected:
  double estimatedProcessingTime(double estSerTime, double estParallelTime);

  HEADER_IDENT("$Id: qxrddataprocessorthreaded.h,v 1.2 2010/09/13 20:00:39 jennings Exp $");
};

#endif // QXRDDATAPROCESSORTHREADED_H

/******************************************************************
*
*  $Log: qxrddataprocessorthreaded.h,v $
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:38  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.10  2010/07/21 19:39:08  jennings
*  Improved calculation of estimated processing time
*
*  Revision 1.1.2.9  2010/06/23 21:44:42  jennings
*  Tracked frame correction times and integration times allowing for multiple threads
*
*  Revision 1.1.2.8  2010/06/17 16:02:35  jennings
*  Skeleton code to support result serialization during crucial points in processing
*  Separate QxrdDoubleImageData class which can hold a reference to a mask
*
*  Revision 1.1.2.7  2010/06/15 20:18:54  jennings
*  First steps to adding CUDA support
*
*  Revision 1.1.2.6  2010/06/09 19:20:29  jennings
*  Removed references to the QxrdFileSaver - all public accesses now through QxrdFileSaverThread
*
*  Revision 1.1.2.5  2010/05/24 21:02:38  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.1.2.4  2010/05/20 20:16:58  jennings
*  Added simple threaded processor implementation using QtConcurrent::run
*
*  Revision 1.1.2.3  2010/04/26 20:53:26  jennings
*  More attempts to get QSharedPointers to work...
*
*  Revision 1.1.2.2  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.1.2.1  2010/04/21 19:48:41  jennings
*  Added QxrdAcquisitionAreaDetector, QxrdAcquisitionPilatus, QxrdDataProcessorCuda, QxrdDataProcessorThreaded
*  and QxrdPreferencesDialog - they don't do much yet, though.
*
*
*******************************************************************/

