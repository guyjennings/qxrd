#define _CRT_SECURE_NO_WARNINGS

#include "qcepimagedataformatmar345.h"
#include "qcepimagedata.h"

#include <QtCore>

//static QcepImageDataFormatMar345 fmt;

template <typename T>
QcepImageDataFormatMar345<T>::QcepImageDataFormatMar345(QString name)
  : QcepImageDataFormat<T>(name)
{
}

template <typename T>
QcepImageDataFormatBase::Priority QcepImageDataFormatMar345<T>::priority() const
{
  return QcepImageDataFormatBase::Specific;
}

template <typename T>
QcepImageDataFormat<T>* QcepImageDataFormatMar345<T>::canLoadFile(QString path)
{
//  printf("QcepImageDataFormatMar345<T>::canLoadFile(%s)\n", qPrintable(path));

  FILE *file = fopen(qPrintable(path), "rb");

  QcepImageDataFormat<T>* res = checkMar345Header(file);

  if (file) {
    fclose(file);
  }

  return res;
}

template <typename T>
QcepImageDataFormat<T>* QcepImageDataFormatMar345<T>::canSaveFile(QString /*path*/)
{
  return NULL;
}

template <typename T>
QcepImageDataFormat<T>* QcepImageDataFormatMar345<T>::loadFile(QString path, QcepImageData<T> *img)
{
  FILE *file = fopen(qPrintable(path), "rb");

  if (file) {
    img -> set_FileName(path);
    img -> set_Name(QFileInfo(path).fileName());

    readMar345Header(file, img);

    fclose(file);

    img -> calculateRangeInCircle();

    img -> setDefaultFileName(path);

    return this;
  } else {
    return NULL;
  }
}


template <typename T>
QcepImageDataFormat<T>* QcepImageDataFormatMar345<T>::saveFile(QString /*path*/, QcepImageData<T> * /*img*/, int /*canOverwrite*/)
{
  return NULL;
}

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))
#define abs(x) (((x) < 0) ? (-(x)) : (x))

#define PACKIDENTIFIER "CCP4 packed image, X: %04d, Y: %04d\n"

const long setbits[33] = {0x00000000L, 0x00000001L, 0x00000003L, 0x00000007L,
                          0x0000000FL, 0x0000001FL, 0x0000003FL, 0x0000007FL,
                          0x000000FFL, 0x000001FFL, 0x000003FFL, 0x000007FFL,
                          0x00000FFFL, 0x00001FFFL, 0x00003FFFL, 0x00007FFFL,
                          0x0000FFFFL, 0x0001FFFFL, 0x0003FFFFL, 0x0007FFFFL,
                          0x000FFFFFL, 0x001FFFFFL, 0x003FFFFFL, 0x007FFFFFL,
                          0x00FFFFFFL, 0x01FFFFFFL, 0x03FFFFFFL, 0x07FFFFFFL,
                          0x0FFFFFFFL, 0x1FFFFFFFL, 0x3FFFFFFFL, 0x7FFFFFFFL,
                          0xFFFFFFFFL};

#define shift_left(x, n)  (((x) & setbits[32 - (n)]) << (n))
#define shift_right(x, n) (((x) >> (n)) & setbits[32 - (n)])

static void
swaplong(char *data, int nbytes)
{
  int i, t1, t2, t3, t4;

  for(i=nbytes/4;i--;) {
    t1 = data[i*4+3];
    t2 = data[i*4+2];
    t3 = data[i*4+1];
    t4 = data[i*4+0];
    data[i*4+0] = t1;
    data[i*4+1] = t2;
    data[i*4+2] = t3;
    data[i*4+3] = t4;
  }
}

template <typename T>
QcepImageDataFormat<T>* QcepImageDataFormatMar345<T>::readMar345Header(FILE *file, QcepImageData<T> *img)
{
  qint32 head[32];
  char buf[128];
  int nx, n32, mar345, byteswap;

  if (file == NULL) {
    return NULL;
  }

  fseek(file, 0, SEEK_SET);

  if (fread(head, sizeof(qint32), 32, file) < 32) {
    return NULL;
  }

//   if (head[0] != 1234) {
//     return;
//   }

  if ((head[0] < 5000) && (head[0] > 100)) {
    byteswap = 0;
  } else {
    byteswap = 1;
    swaplong( (char*) head, 10*sizeof(qint32) );

    /* Was byte swapping successful */
    if ((head[0] > 5000) || (head[0] < 100)) {
      printf("ERROR> Cannot byteswap header of %s\n",qPrintable(img->get_FileName()));
      return NULL;
    }
  }

  if (head[0] == 1234) {
    nx = head[1];
    n32 = head[2];
    mar345 = 1;
  } else {
    nx = head[0];
    n32 = head[4];
    mar345 = 0;
  }

  int fpos = fseek(file, 192, SEEK_SET);

  while (fgets(buf, 64, file)) {
    fpos += 64;
    int res = fseek(file, fpos, SEEK_SET);

    if (strstr(buf, "END OF HEADER")) {
      break;
    } else if (strstr(buf, "SKIP")) {
      continue;
    }

    if (strlen(buf)<2) continue;

    if (buf[0]=='#' || buf[0]=='!') continue;
  }

  readMar345Image(file, img);

  if (n32) {
//     printf("%d overflow pixels\n", n32);
    int res;

    if (mar345) {
      res = fseek(file, 4096, SEEK_SET);
    } else {
      res = fseek(file, 2*nx, SEEK_SET);
    }

    if (res < 0) {
      printf("ERROR> Cannot seek when reading MAR345 file\n");
    }

    T *p = img->data();

    for (int i=0; i<n32; i++ ) {
      size_t n;
      int ihigh, high[2], j, row, col;

      if ( ( n=fread(high, sizeof(qint32), 2, file) )!= 2 ) {
        printf("ERROR> Cannot read pixel %d from %s\n",
               i, qPrintable(img->get_FileName()));
      }

      if (byteswap) {
        swaplong((char*) high, 2 * sizeof(int));
      }

      if ( mar345 ) {
// 	j   = high[0];
        row = high[0]/nx;
        col = high[0]%nx;
        j = row*nx + col - 1;
      } else {
        row = high[0]/nx;
        col = high[0]%nx;
        j = row*nx + col - 1;
      }

      ihigh = high[1];
//       ihigh = min(ihigh, 262128);
      p[j]  = ihigh;

//       if (i < 32) {
// 	printf("%d: high[0]=%d high[1]=%d: col=%d: row=%d: img[%d]=%d\n",
// 	       i, high[0], high[1], col, row, j, i32);
//       }
    }
  }

  return this;
}

template <typename T>
QcepImageDataFormat<T>* QcepImageDataFormatMar345<T>::readMar345Image(FILE *file, QcepImageData<T> *img)
{
  int x=0, y=0;
  char header[BUFSIZ];

  //   fseek(file, 0, SEEK_SET);

  while (fgets(header, BUFSIZ, file)) {
//     printf("Header: %-40s\n", header);
    if (sscanf(header, PACKIDENTIFIER, &x, &y) == 2) {
      break;
    }
  }

//   printf("Image dimensions %d x %d\n", x,y);

  img -> resize(x, y);

  T *p = img->data();

  int valids = 0;
  int spillbits = 0;
  int usedbits, total=x*y;
  long window=0, spill=0, nextint, bitnum, pixnum;
  static int 	bitdecode[8] = {0, 4, 5, 6, 7, 8, 16, 32};

  for (long pixel=0; pixel<total; ) {
    if (valids < 6) {
      if (spillbits > 0) {
        window |= shift_left(spill, valids);
        valids += spillbits;
        spillbits = 0;
      } else {
        spill = (long) getc(file);
        spillbits = 8;
      }
    } else {
      pixnum = 1 << (window & setbits[3]);
      window = shift_right(window, 3);
      bitnum = bitdecode[window & setbits[3]];
      window = shift_right(window, 3);
      valids -= 6;
      while ((pixnum > 0) && (pixel < total)) {
        if (valids < bitnum) {
          if (spillbits > 0) {
            window |= shift_left(spill, valids);
            if ((32 - valids) > spillbits) {
              valids += spillbits;
              spillbits = 0;
            } else {
              usedbits = 32 - valids;
              spill = shift_right(spill, usedbits);
              spillbits -= usedbits;
              valids = 32;
            }
          } else {
            spill = (long) getc(file);
            spillbits = 8;
          }
        } else {
          --pixnum;
          if (bitnum == 0) {
            nextint = 0;
          } else {
            nextint = window & setbits[bitnum];
            valids -= bitnum;
            window = shift_right(window, bitnum);
            if ((nextint & (1 << (bitnum - 1))) != 0) {
              nextint |= ~setbits[bitnum];
            }
          }
          if (pixel > x) {
            p[pixel] = (qint16)( nextint +
                                ((qint16)(p[pixel-1]) +
                                 (qint16)(p[pixel-x+1]) +
                                 (qint16)(p[pixel-x]) +
                                 (qint16)(p[pixel-x-1]) + 2) / 4);
            ++pixel;
          } else if (pixel != 0) {
            p[pixel] = (qint16)(p[pixel - 1] + nextint);
            ++pixel;
          } else {
            p[pixel++] = (qint16)nextint;
          }
        }
      }
    }
  }

  for (int i=0; i<total; i++) {
    p[i] = (quint16)((qint16) p[i]);
  }

  return this;
}

template <typename T>
QcepImageDataFormat<T>* QcepImageDataFormatMar345<T>::checkMar345Header(FILE *file)
{
  qint32 head[32];
  char buf[128];
//  int byteswap;

  if (file == NULL) {
    return NULL;
  }

  fseek(file, 0, SEEK_SET);

  if (fread(head, sizeof(qint32), 32, file) < 32) {
    return NULL;
  }

  if ((head[0] < 5000) && (head[0] > 100)) {
//    byteswap = 0;
  } else {
//    byteswap = 1;
    swaplong((char*) head, 10*sizeof(qint32));
  }

  if (head[0] == 1234) {
    /* mar345 format */
  } else if (head[0] == 1200 || head[0] == 2000) {
    /* old mar ip format */
  } else {
    return NULL;
  }

  if (fread(buf, 1, 64, file) < 64) {
    return NULL;
  }

  if (strncmp(buf, "mar research", 12) == 0) {
    return this;
  } else {
    return NULL;
  }
}

template class QcepImageDataFormatMar345<unsigned short>;
template class QcepImageDataFormatMar345<short>;
template class QcepImageDataFormatMar345<unsigned int>;
template class QcepImageDataFormatMar345<int>;
template class QcepImageDataFormatMar345<double>;
