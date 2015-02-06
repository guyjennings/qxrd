#ifndef QXRDSLICEDIALOG_H
#define QXRDSLICEDIALOG_H

#include <QDockWidget>
#include "qxrdplot.h"
#include "ui_qxrdslicedialog.h"
#include "qxrdimagedata.h"
#include "qxrdsettingssaver-ptr.h"
#include "qxrdslicedialogsettings.h"

class QxrdSliceDialog : public QDockWidget, public Ui::QxrdSliceDialog
{
  Q_OBJECT

public:
  explicit QxrdSliceDialog(QxrdSliceDialogSettingsWPtr settings, QWidget *parent);
  virtual ~QxrdSliceDialog();

  void onProcessedImageAvailable(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow);

public slots:
  void slicePolygon(QVector<QPointF> poly);

private:
  void reslice();

private:
  QxrdSliceDialogSettingsWPtr m_SliceDialogSettings;
  QxrdDoubleImageDataPtr      m_Image;
};

#endif // QXRDSLICEDIALOG_H
