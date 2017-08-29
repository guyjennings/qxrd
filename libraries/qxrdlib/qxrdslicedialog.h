#ifndef QXRDSLICEDIALOG_H
#define QXRDSLICEDIALOG_H

#include <QDockWidget>
#include "qcepplot.h"
#include "ui_qxrdslicedialog.h"
#include "qcepimagedata.h"
#include "qxrdslicedialogsettings-ptr.h"

class QxrdSliceDialog : public QDockWidget, public Ui::QxrdSliceDialog
{
  Q_OBJECT

public:
  explicit QxrdSliceDialog(QxrdSliceDialogSettingsWPtr settings, QWidget *parent);
  virtual ~QxrdSliceDialog();

  void onProcessedImageAvailable(QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow);

public slots:
  void slicePolygon(QVector<QPointF> poly);

private:
  void reslice();

private:
  QxrdSliceDialogSettingsWPtr m_SliceDialogSettings;
  QcepDoubleImageDataPtr      m_Image;
};

#endif // QXRDSLICEDIALOG_H
