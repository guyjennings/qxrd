#ifndef QCEPIMAGEDATAFORMATCBF_H
#define QCEPIMAGEDATAFORMATCBF_H

#include "qcepmacros.h"
#include "qcepimagedataformat.h"

template <typename T>
class QcepImageDataFormatCBF : public QcepImageDataFormat<T>
{
public:
  explicit QcepImageDataFormatCBF(QString name);

  QcepImageDataFormat<T>* canLoadFile(QString path);
  QcepImageDataFormat<T>* canSaveFile(QString path);
  QcepImageDataFormat<T>* loadFile(QString path, QcepImageData<T> *img);
  QcepImageDataFormat<T>* saveFile(QString path, QcepImageData<T> *img, int canOverwrite);
  QcepImageDataFormatBase::Priority priority() const;
};

#endif // QCEPIMAGEDATAFORMATCBF_H
