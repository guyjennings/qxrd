#ifndef QCEPIMAGEDATAFORMAT_H
#define QCEPIMAGEDATAFORMAT_H

#include "qcepmacros.h"

#include <QObject>
#include <QString>

#include "qcepimagedata-ptr.h"

class QcepImageDataFormatBase
{
 public:
  QcepImageDataFormatBase(QString name)
    : m_Name(name)
  {
  }

  virtual ~QcepImageDataFormatBase()
  {
  }

  enum Priority {
    Unable = 0,
    CatchAll = 1,
    Versatile = 2,
    Specific = 3
  };

  virtual Priority priority() const = 0;

  QString name() const
  {
    return m_Name;
  }

  void setName(QString name)
  {
    m_Name = name;
  }

  enum {
    NoOverwrite,
    CanOverwrite
  };

public:
  void mkPath(QString filePath);
  QString uniqueFileName(QString name);

private:
  QString m_Name;
};

template <typename T>
class QcepImageDataFormat : public QcepImageDataFormatBase
{
 public:
  QcepImageDataFormat(QString fmtname);
  virtual ~QcepImageDataFormat();

  virtual QcepImageDataFormat<T>* canLoadFile(QString path) = 0;
  virtual QcepImageDataFormat<T>* canSaveFile(QString path) = 0;
  virtual QcepImageDataFormat<T>* loadFile(QString path, QcepImageData<T> *img) = 0;
  virtual QcepImageDataFormat<T>* saveFile(QString path, QcepImageData<T> *img, int canOverwrite) = 0;
};

#endif
