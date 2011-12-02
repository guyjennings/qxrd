#ifndef QXRDHISTOGRAMDIALOG_H
#define QXRDHISTOGRAMDIALOG_H

#include <QDockWidget>
#include "ui_qxrdhistogramdialog.h"
#include "qxrdsettings.h"
#include "qxrdimagedata.h"
#include "qwt_double_rect.h"

class QxrdHistogramDialog : public QDockWidget, public Ui::QxrdHistogramDialog
{
  Q_OBJECT

public:
  explicit QxrdHistogramDialog(QWidget *parent = 0);

  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  void onProcessedImageAvailable(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow);

public slots:
  void histogramSelectionChanged(QwtDoubleRect rect);

private:
  void recalculateHistogram();

private:
  QxrdDoubleImageDataPtr   m_Image;
  QwtDoubleRect            m_HistogramRect;
};

#endif // QXRDHISTOGRAMDIALOG_H
