/******************************************************************
*
*  $Id: qxrdacquisitionoperations.h,v 1.4 2009/07/10 22:54:23 jennings Exp $
*
*******************************************************************/

#ifndef QXRDACQUISITIONOPERATIONS_H
#define QXRDACQUISITIONOPERATIONS_H

#include "qcepmacros.h"

#include "qxrdacquisitionscripting.h"

#include "qxrdimagequeue.h"

class QxrdDataProcessor;

class QxrdAcquisitionOperations : public QxrdAcquisitionScripting
{
  Q_OBJECT;
public:
  QxrdAcquisitionOperations(QxrdAcquisitionThread *thread, QxrdDataProcessor *proc);

  QxrdImageData *takeNextFreeImage();
  QxrdImageData *takeNextAcquiredImage();

  void returnImageToPool(QxrdImageData *img);
  void newAcquiredImage(QxrdImageData *img);

  void loadData(QString name);
  void saveData(QString name);

  void saveImageData(QxrdImageData *image);
  void saveRawData(QxrdImageData *image);
  void saveNamedImageData(QString name, QxrdImageData *image);

  void loadDarkImage(QString name);
  void loadBadPixels(QString name);
  void loadGainMap(QString name);

  void newData(QxrdImageData *image);
  void newDarkImage(QxrdImageData *image);
  void newBadPixelsImage(QxrdImageData *image);
  void newGainMapImage(QxrdImageData *image);

  void showMaskRange(double min, double max);
  void hideMaskRange(double min, double max);
  void showMaskAll();
  void hideMaskAll();

  QxrdImageData *data() const;
  QxrdImageData *darkImage() const;

public slots:
  void onProcessedImageAvailable();
  void onDarkImageAvailable();

signals:
  void acquiredImageAvailable();

private:
  mutable QMutex         m_Mutex;
  QxrdDataProcessor     *m_DataProcessor;
  QxrdImageQueue         m_FreeImages;
  QxrdImageQueue         m_AcquiredImages;

  QxrdImageData         *m_Data;
  QxrdImageData         *m_DarkFrame;
  QxrdImageData         *m_BadPixels;
  QxrdImageData         *m_GainFrame;

  HEADER_IDENT("$Id: qxrdacquisitionoperations.h,v 1.4 2009/07/10 22:54:23 jennings Exp $");
};

#endif // QXRDACQUISITIONOPERATIONS_H

/******************************************************************
*
*  $Log: qxrdacquisitionoperations.h,v $
*  Revision 1.4  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.3  2009/07/09 01:15:09  jennings
*  Added some locks
*
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

