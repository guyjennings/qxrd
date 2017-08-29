#ifndef QXRDIMAGEDISPLAYWIDGET_H
#define QXRDIMAGEDISPLAYWIDGET_H

#include "qxrdlib_global.h"
#include <QWidget>
#include <QTabWidget>
#include <QVector>
#include <QRgb>

#include "qcepimagedata.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata.h"
#include "qcepmaskdata-ptr.h"
#include "qxrdapplication-ptr.h"

class QXRD_EXPORT QxrdImageDisplayWidget : public QWidget
{
  Q_OBJECT

public:
  explicit QxrdImageDisplayWidget(QWidget *parent = 0);

public:
  void paintEvent(QPaintEvent *event);
  QSize	sizeHint () const;

  static QxrdImageDisplayWidget* insertNew(QxrdApplication *app, QTabWidget *tw);

public slots:
  void updateImage(QcepDoubleImageDataPtr img,
                   QcepMaskDataPtr        ovflow = QcepMaskDataPtr(),
                   QcepMaskDataPtr        mask = QcepMaskDataPtr());
  void setGrayscale();

private:
  void rebuildImage();

private:
  QcepDoubleImageDataPtr   m_Data;
  QcepMaskDataPtr          m_Mask;
  QcepMaskDataPtr          m_Overflow;

  QImage                   m_DataImage;

  double                   m_Minimum;
  double                   m_Maximum;
  double                   m_StepSize;

  int                      m_TableMax;
  QVector<QRgb>            m_ColorTable;

  double                   m_DisplayScale;
};

#endif // QXRDIMAGEDISPLAYWIDGET_H
