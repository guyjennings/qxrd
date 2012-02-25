#ifndef QCEPIMAGEDATAFORMAT_H
#define QCEPIMAGEDATAFORMAT_H

#include "qcepmacros.h"

#include <QObject>
#include <QString>

template <typename T> class QcepImageData;

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

private:
  QString m_Name;
};

template <typename T>
class QcepImageDataFormat : public QcepImageDataFormatBase
{
 public:
  QcepImageDataFormat(QString fmtname);

  virtual QcepImageDataFormat<T>* canLoadFile(QString path) = 0;
  virtual QcepImageDataFormat<T>* loadFile(QString path, QcepImageData<T> *img) = 0;
  virtual QcepImageDataFormat<T>* saveFile(QString path, QcepImageData<T> *img) = 0;
};

#endif
