#ifndef QCEPIMAGEDATAFORMATMAR345_H
#define QCEPIMAGEDATAFORMATMAR345_H

#include "qceplib_global.h"
#include "qcepmacros.h"

#include "qcepimagedataformat.h"

#include <QFileInfo>

template <typename T>
class QCEP_EXPORT QcepImageDataFormatMar345 : public QcepImageDataFormat<T>
{
 public:
  QcepImageDataFormatMar345(QString name);

  virtual QcepImageDataFormat<T>* canLoadFile(QString path);
  virtual QcepImageDataFormat<T>* canSaveFile(QString path);
  virtual QcepImageDataFormat<T>* loadFile(QString path, QcepImageData<T> *img);
  virtual QcepImageDataFormat<T>* saveFile(QString path, QcepImageData<T> *img, int canOverwrite);
  QcepImageDataFormatBase::Priority priority() const;

 private:
  QcepImageDataFormat<T>* checkMar345Header(FILE *f);
  QcepImageDataFormat<T>* readMar345Header(FILE *f, QcepImageData<T> *img);
  QcepImageDataFormat<T>* readMar345Image(FILE *f, QcepImageData<T> *img);
};

#endif
