/******************************************************************
*
*  $Id: qxrdimagedataformathis.cpp,v 1.2 2010/09/13 20:00:40 jennings Exp $
*
*******************************************************************/

#include "qxrdimagedataformathis.h"
#include "qxrdimagedata.h"
#include <QFileInfo>

static QxrdImageDataFormatHis fmt;

QxrdImageDataFormatHis::QxrdImageDataFormatHis(QString name)
  : QcepImageDataFormat<double>(name),
    SOURCE_IDENT("$Id: qxrdimagedataformathis.cpp,v 1.2 2010/09/13 20:00:40 jennings Exp $")
{
}

QxrdImageDataFormatHis::Priority QxrdImageDataFormatHis::priority() const
{
  return Specific;
}

typedef struct {
  quint16 FileType;
  quint16 unk;
  quint16 HeaderSize;		        // Size of this file header in Bytes
  quint32 FileSize;			// Size of the whole file in Bytes
  quint16 ImageHeaderSize;	        // Size of the image header in Bytes
  quint16 ULX, ULY, BRX, BRY;          // bounding rectangle of the image
  quint16 NrOfFrames;		        // self explanatory
  quint16 Correction;	  	        // 0 = none, 1 = offset, 2 = gain, 4 = bad pixel, (ored)
  double IntegrationTime;	        // frame time in microseconds
  quint16 TypeOfNumbers;		// short, long integer, float, signed/u};
} HISHeader;

QxrdImageDataFormatHis* QxrdImageDataFormatHis::canLoadFile(QString path)
{
//  printf("QxrdImageDataFormatHis::canLoadFile(%s)\n", qPrintable(path));

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

    fclose(file);
  }

  return res;
}

QxrdImageDataFormatHis* QxrdImageDataFormatHis::loadFile(QString path, QcepImageData<double> *img)
{
//  printf("QxrdImageDataFormatHis::loadFile(%s)\n", qPrintable(path));

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
          img -> setValue(x,(height-y-1),pix);
	}
      }
    } else if (h.TypeOfNumbers == 32) {
      quint32 pix;
      for (int y=0; y<height; y++) {
	for (int x=0; x<width; x++) {
          fread(&pix, 4, 1, file);
          img -> setValue(x,(height-y-1),pix);
	}
      }
    }

    fclose(file);

    img -> calculateRange();

    img -> setDefaultFileName(path);

    return this;
  }

  return NULL;
}

QxrdImageDataFormatHis* QxrdImageDataFormatHis::saveFile(QString /*path*/, QcepImageData<double> * /*img*/)
{
  return NULL;
}


/******************************************************************
*
*  $Log: qxrdimagedataformathis.cpp,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:39  jennings
*  Moving files into source subdirectory
*
*  Revision 1.11  2009/11/09 16:44:38  jennings
*  Reduced initial buffer size to 500M, fixed problem loading 32-bit his files
*
*  Revision 1.10  2009/09/23 21:16:58  jennings
*  .his files can be loaded again
*
*  Revision 1.9  2009/09/22 20:41:07  jennings
*  Set filename and title properties when loading data files
*
*  Revision 1.8  2009/09/10 13:53:23  jennings
*  Removed possibilitity of fclose(NULL)
*
*  Revision 1.7  2009/08/04 16:45:20  jennings
*  Moved mask data into separate class
*
*  Revision 1.6  2009/07/08 00:35:33  jennings
*  *** empty log message ***
*
*  Revision 1.5  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/


