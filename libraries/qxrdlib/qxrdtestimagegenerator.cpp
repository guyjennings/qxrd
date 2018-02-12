#include "qxrdtestimagegenerator.h"
#include "qcepallocator.h"
#include "qcepimagedata.h"
#include <QPainter>

QxrdTestImageGenerator::QxrdTestImageGenerator(QString name)
  : QxrdTestGenerator(name)
{
}

void QxrdTestImageGenerator::start()
{
  printMessage("Test Image Generator Started");
}

void QxrdTestImageGenerator::generateTest()
{
  int ind = get_TestIndex();

  prop_TestIndex()->incValue(1);

  QcepDoubleImageDataPtr img = QcepAllocator::newDoubleImage(sharedFromThis(),
                                                             tr("newData-%1").arg(ind),
                                                             2048, 2048,
                                                             QcepAllocator::NullIfNotAvailable);

  if (img) {
    img->set_ImageNumber(ind);

    int ht = img->get_Height();
    int wd = img->get_Width();

    QImage image(wd, ht, QImage::Format_RGB32);
    QPainter painter(&image);

    painter.fillRect(0,0, wd,ht, Qt::black);
    painter.fillRect(50,50, wd-100, ht-100, Qt::gray);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Times", 64, QFont::Bold, true));
    painter.drawText(QPointF(wd/2, ht/2), tr("%1").arg(ind), Qt::AlignHCenter | Qt::AlignVCenter, 0);

    QRgb *rgb = (QRgb*) image.bits();
    int hist[256];

    for (int i=0; i<256; i++) {
      hist[i] = 0;
    }

    for (int j=0; j<ht; j++) {
      for (int i=0; i<wd; i++) {
        int lVal = qGray(*rgb++) & 0xff;

        hist[lVal]++;

        img->setValue(i, ht-j-1, lVal);
      }
    }

    int nPix = 0;

    for (int i=1; i<256; i++) {
      nPix += hist[i];
    }

    printf("%d non-zero pixels\n", nPix);
  }

  emit newImageAvailable(img);
}
