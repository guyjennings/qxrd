#include "qxrdimagedisplaywidget.h"
#include <QPainter>
#include <QTime>
#include <QScrollArea>
#include <QPaintEvent>
#include "qxrdapplication.h"

QxrdImageDisplayWidget::QxrdImageDisplayWidget(QWidget *parent) :
    QWidget(parent),
    m_Data(NULL),
    m_Mask(NULL),
    m_Overflow(NULL),
    m_Minimum(0),
    m_Maximum(25.5),
    m_StepSize(0.1),
    m_TableMax(255),
    m_ColorTable(256),
    m_DisplayScale(0.5)
{
  setGrayscale();
}

void QxrdImageDisplayWidget::setGrayscale()
{
  m_ColorTable.resize(257);

  for (int i=0; i<256; i++) {
    m_ColorTable[i] = qRgb(i,i,i);
  }
}

void QxrdImageDisplayWidget::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);

  QRegion rgn = event->region();
  QVector<QRect> rects = rgn.rects();

  foreach(QRect rect, rects) {
//    printf("paint rect %d,%d,%d,%d\n",
//           rect.left(), rect.top(),
//           rect.right(), rect.bottom());

    QRect  srcRect(rect.left()/m_DisplayScale, rect.top()/m_DisplayScale,
                   rect.width()/m_DisplayScale+1, rect.height()/m_DisplayScale+1);
    QRect  dstRect(srcRect.left()*m_DisplayScale, srcRect.top()*m_DisplayScale,
                   srcRect.width()*m_DisplayScale, srcRect.height()*m_DisplayScale);

    painter.drawImage(dstRect, m_DataImage, srcRect);
  }

//  painter.fillRect(rect(), Qt::lightGray);
//
//  painter.drawImage(rect(), m_DataImage, m_DataImage.rect());

//  painter.fillRect(QRect(10,10,40,40), Qt::red);
}

void QxrdImageDisplayWidget::updateImage
    (QxrdDoubleImageDataPtr img,
     QxrdMaskDataPtr        ovflow,
     QxrdMaskDataPtr        mask)
{
  QTime tic;
  tic.start();

  if (img) {
    m_Data = img;
  }

  if (ovflow) {
    m_Overflow = ovflow;
  }

  if (mask) {
    m_Mask = mask;
  }

  rebuildImage();

  adjustSize();
  repaint();

  emit printMessage(QDateTime::currentDateTime(),
                    tr("Image rebuilt after %1 msec").arg(tic.elapsed()));
}

QSize QxrdImageDisplayWidget::sizeHint() const
{
  if (m_Data) {
    return QSize(m_Data->get_Width()*m_DisplayScale,
                 m_Data->get_Height()*m_DisplayScale);
  } else {
    return QSize(128,128);
  }
}

void QxrdImageDisplayWidget::rebuildImage()
{
  if (m_Data) {
    int width = m_Data->get_Width();
    int height = m_Data->get_Height();
    double *data = m_Data->data();
    QRgb   *table = m_ColorTable.data();

    m_DataImage = QImage(width, height, QImage::Format_ARGB32_Premultiplied);

    for (int row=0; row<height; row++) {
      QRgb* line = (QRgb*)(m_DataImage.scanLine(row));

      for (int col=0; col<width; col++) {
        double val = *data++;

        if (val<m_Minimum) {
          *line++ = table[0];
        } else if (val>m_Maximum) {
          *line++ = table[m_TableMax];
        } else {
          int n = (int)((val - m_Minimum)/m_StepSize);
          *line++ = table[n];
        }
      }
    }
  }
}

QxrdImageDisplayWidget* QxrdImageDisplayWidget::insertNew(QxrdApplication *app, QTabWidget *tw)
{
  QxrdImageDisplayWidget *res = NULL;

  if (tw) {
    QScrollArea *sa = new QScrollArea();

    res = new QxrdImageDisplayWidget();

    sa -> setWidget(res);

    tw->insertTab(1, sa, "Image");

    connect(res, SIGNAL(printMessage(QDateTime,QString)), app, SIGNAL(printMessage(QDateTime,QString)));
    connect(res, SIGNAL(statusMessage(QDateTime,QString)), app, SIGNAL(statusMessage(QDateTime,QString)));
    connect(res, SIGNAL(criticalMessage(QDateTime,QString)), app, SIGNAL(criticalMessage(QDateTime,QString)));
  }

  return res;
}
