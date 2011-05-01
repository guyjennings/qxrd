#ifndef QCEPIMAGEDATAFORMATFACTORY_H
#define QCEPIMAGEDATAFORMATFACTORY_H

#include "qcepmacros.h"

#include <QObject>
#include <QString>
#include <QList>

template <typename T> class QcepImageDataFormat;

template <typename T>
class QcepImageDataFormatFactory
{
 private:
  static bool s_Created;
  static QcepImageDataFormatFactory<T> *s_Factory;

  QcepImageDataFormatFactory();

 public:
  static QcepImageDataFormatFactory<T> *factory();
  ~QcepImageDataFormatFactory();

  void insert(QcepImageDataFormat<T> *f);
  QcepImageDataFormat<T>* canLoad(QString path);

 private:
  QList<QcepImageDataFormat<T>*> m_Formats;
};

#endif
