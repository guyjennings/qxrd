#ifndef QXRDHISTOGRAMDIALOG_H
#define QXRDHISTOGRAMDIALOG_H

#include <QDockWidget>
#include "ui_qxrdhistogramdialog.h"
#include "qxrdimagedata.h"
#include "qxrdimagedata-ptr.h"
#include "qxrdmaskdata.h"
#include "qxrdmaskdata-ptr.h"
#include "qwt_double_rect.h"
#include "qxrdhistogramdialogsettings.h"

class QxrdHistogramDialog : public QDockWidget, public Ui::QxrdHistogramDialog
{
  Q_OBJECT

public:
  explicit QxrdHistogramDialog(QxrdHistogramDialogSettingsWPtr settings, QWidget *parent);

  void onProcessedImageAvailable(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow);

public slots:
  void histogramSelectionChanged(QwtDoubleRect rect);

private:
  void recalculateHistogram();

private:
  QxrdHistogramDialogSettingsWPtr m_HistogramDialogSettings;
  QxrdDoubleImageDataPtr          m_Image;
};

#endif // QXRDHISTOGRAMDIALOG_H
