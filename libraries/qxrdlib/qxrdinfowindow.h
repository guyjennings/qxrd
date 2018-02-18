#ifndef QXRDINFOWINDOW_H
#define QXRDINFOWINDOW_H

#include "qcepmacros.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdinfowindow.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"

class QXRD_EXPORT QxrdInfoWindow : public QxrdMainWindow, public Ui::QxrdInfoWindow
{
  Q_OBJECT

public:
  explicit QxrdInfoWindow(QString name,
                          QxrdAppCommonWPtr app,
                          QxrdExperimentWPtr expt,
                          QxrdAcqCommonWPtr acqw,
                          QxrdProcessorWPtr procw);
  ~QxrdInfoWindow();

  void onProcessedImageAvailable(QcepDoubleImageDataPtr image,
                                 QcepMaskDataPtr overflow);

protected:
  void changeEvent(QEvent *e);
};

#endif // QXRDINFOWINDOW_H
