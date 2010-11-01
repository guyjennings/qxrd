#ifndef QXRDIMAGEDISPLAYWIDGET_H
#define QXRDIMAGEDISPLAYWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QVector>
#include <QRgb>

#include "qxrddoubleimagedata.h"
#include "qxrdmaskdata.h"
#include "qxrdforwardtypes.h"

class QxrdImageDisplayWidget : public QWidget
{
  Q_OBJECT;

public:
  explicit QxrdImageDisplayWidget(QWidget *parent = 0);

public:
  void paintEvent(QPaintEvent *event);
  QSize	sizeHint () const;

  static QxrdImageDisplayWidget* insertNew(QxrdApplication *app, QTabWidget *tw);

signals:
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

public slots:
  void updateImage(QxrdDoubleImageDataPtr img,
                   QxrdMaskDataPtr        ovflow = QxrdMaskDataPtr(),
                   QxrdMaskDataPtr        mask = QxrdMaskDataPtr());
  void setGrayscale();

private:
  void rebuildImage();

private:
  QxrdDoubleImageDataPtr   m_Data;
  QxrdMaskDataPtr          m_Mask;
  QxrdMaskDataPtr          m_Overflow;

  QImage                   m_DataImage;

  double                   m_Minimum;
  double                   m_Maximum;
  double                   m_StepSize;

  int                      m_TableMax;
  QVector<QRgb>            m_ColorTable;

  double                   m_DisplayScale;
};

#endif // QXRDIMAGEDISPLAYWIDGET_H
