#include "qxrdimagedataformathis.h"
#include "qxrdimagedata.h"
#include <QFileInfo>

static QxrdImageDataFormatHis fmt;

QxrdImageDataFormatHis::QxrdImageDataFormatHis(QObject *parent)
  : QcepImageDataFormat<double>(parent)
{
}

QxrdImageDataFormatHis::Priority QxrdImageDataFormatHis::priority() const
{
  return Specific;
}

typedef struct {
  uint16_t FileType;
  uint16_t unk;
  uint16_t HeaderSize;		        // Size of this file header in Bytes
  uint32_t FileSize;			// Size of the whole file in Bytes
  uint16_t ImageHeaderSize;	        // Size of the image header in Bytes
  uint16_t ULX, ULY, BRX, BRY;          // bounding rectangle of the image
  uint16_t NrOfFrames;		        // self explanatory
  uint16_t Correction;	  	        // 0 = none, 1 = offset, 2 = gain, 4 = bad pixel, (ored)
  double IntegrationTime;	        // frame time in microseconds
  uint16_t TypeOfNumbers;		// short, long integer, float, signed/u};
} HISHeader;

QxrdImageDataFormatHis* QxrdImageDataFormatHis::canLoadFile(QString path)
{
  QxrdImageDataFormatHis* res = NULL;

  FILE *file = fopen(qPrintable(path), "rb");
  HISHeader h;

  if (file) {
    fread(&h.FileType,     2, 1, file);
    fread(&h.unk,          2, 1, file);
    fread(&h.HeaderSize,   2, 1, file);
    fread(&h.FileSize,     4, 1, file);
    fread(&h.ImageHeaderSize, 2, 1, file);
    fread(&h.ULX, 2, 1, file);
    fread(&h.ULY, 2, 1, file);
    fread(&h.BRX, 2, 1, file);
    fread(&h.BRY, 2, 1, file);
    fread(&h.NrOfFrames, 2, 1, file);
    fread(&h.Correction, 2, 1, file);
    fread(&h.IntegrationTime, 8, 1, file);
    fread(&h.TypeOfNumbers, 2, 1, file);

    if (h.FileType == 0x7000 && 
	h.HeaderSize == 100 &&
	(h.TypeOfNumbers == 4 || h.TypeOfNumbers == 32)) {
      res = this;
    }
  }

  fclose(file);

  return res;
}

QxrdImageDataFormatHis* QxrdImageDataFormatHis::loadFile(QString path, QcepImageData<double> *img)
{
  FILE *file = fopen(qPrintable(path), "rb");

  HISHeader h;

  if (file) {
    fread(&h.FileType,     2, 1, file);
    fread(&h.unk,          2, 1, file);
    fread(&h.HeaderSize,   2, 1, file);
    fread(&h.FileSize,     4, 1, file);
    fread(&h.ImageHeaderSize, 2, 1, file);
    fread(&h.ULX, 2, 1, file);
    fread(&h.ULY, 2, 1, file);
    fread(&h.BRX, 2, 1, file);
    fread(&h.BRY, 2, 1, file);
    fread(&h.NrOfFrames, 2, 1, file);
    fread(&h.Correction, 2, 1, file);
    fread(&h.IntegrationTime, 8, 1, file);
    fread(&h.TypeOfNumbers, 2, 1, file);

    int width = h.BRX-h.ULX+1;
    int height = h.BRY-h.ULY+1;

    fseek(file, 100, SEEK_SET);

    img -> resize(width, height);

    if (h.TypeOfNumbers == 4) {
      quint16 pix;
      for (int y=0; y<height; y++) {
	for (int x=0; x<width; x++) {
	  fread(&pix, 2, 1, file);
	  img -> setValue(x,y,pix);
	}
      }
    } else if (h.TypeOfNumbers == 32) {
      quint32 pix;
      for (int y=0; y<height; y++) {
	for (int x=0; x<width; x++) {
	  fread(&pix, 2, 1, file);
	  img -> setValue(x,y,pix);
	}
      }
    }

    img -> setFilename(path);
    img -> setTitle(QFileInfo(path).fileName());

    fclose(file);

    img -> calculateRange();
    img -> setFullyUnmasked();
  }

  return NULL;
}

QxrdImageDataFormatHis* QxrdImageDataFormatHis::saveFile(QString /*path*/, QcepImageData<double> * /*img*/)
{
  return NULL;
}


