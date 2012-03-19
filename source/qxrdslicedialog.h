#ifndef QXRDSLICEDIALOG_H
#define QXRDSLICEDIALOG_H

#include <QDockWidget>
#include "qxrdplot.h"
#include "ui_qxrdslicedialog.h"
#include "qxrdimagedata.h"
#include "qwt_array.h"
#include "qxrdsettingssaver-ptr.h"

class QxrdSliceDialog : public QDockWidget, public Ui::QxrdSliceDialog
{
  Q_OBJECT

public:
  explicit QxrdSliceDialog(QxrdSettingsSaverWPtr saver, QWidget *parent = 0);

  void onProcessedImageAvailable(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow);

public slots:
  void slicePolygon(QwtArray<QwtDoublePoint> poly);

private:
  void reslice();

private:
  QxrdDoubleImageDataPtr   m_Image;
};

#endif // QXRDSLICEDIALOG_H
