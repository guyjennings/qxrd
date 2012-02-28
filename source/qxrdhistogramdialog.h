#ifndef QXRDHISTOGRAMDIALOG_H
#define QXRDHISTOGRAMDIALOG_H

#include <QDockWidget>
#include "ui_qxrdhistogramdialog.h"
#include "qxrdimagedata.h"
#include "qxrdimagedata-ptr.h"
#include "qxrdmaskdata.h"
#include "qxrdmaskdata-ptr.h"
#include "qwt_double_rect.h"
#include "qxrdsettingssaver-ptr.h"

class QxrdHistogramDialog : public QDockWidget, public Ui::QxrdHistogramDialog
{
  Q_OBJECT

public:
  explicit QxrdHistogramDialog(QxrdSettingsSaverWPtr saver, QWidget *parent = 0);

  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  void onProcessedImageAvailable(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow);

public slots:
  void histogramSelectionChanged(QwtDoubleRect rect);

private:
  void recalculateHistogram();

private:
  QxrdSettingsSaverWPtr    m_Saver;
  QxrdDoubleImageDataPtr   m_Image;
  QwtDoubleRect            m_HistogramRect;
};

#endif // QXRDHISTOGRAMDIALOG_H
