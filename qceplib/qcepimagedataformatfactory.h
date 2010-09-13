/******************************************************************
*
*  $Id: qcepimagedataformatfactory.h,v 1.2 2010/09/13 20:00:25 jennings Exp $
*
*******************************************************************/

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
  HEADER_IDENT("$Id: qcepimagedataformatfactory.h,v 1.2 2010/09/13 20:00:25 jennings Exp $");
};

template <typename T>
bool QcepImageDataFormatFactory<T>::s_Created = false;

template <typename T>
QcepImageDataFormatFactory<T>* QcepImageDataFormatFactory<T>::s_Factory = NULL;

template <typename T>
QcepImageDataFormatFactory<T>* QcepImageDataFormatFactory<T>::factory()
{
  if (! s_Created) {
    s_Factory = new QcepImageDataFormatFactory<T>();
    s_Created = true;
  }

  return s_Factory;
}

template <typename T>
QcepImageDataFormatFactory<T>::QcepImageDataFormatFactory()
{
}

template <typename T>
QcepImageDataFormatFactory<T>::~QcepImageDataFormatFactory()
{
  s_Created = false;
}

template <typename T>
void QcepImageDataFormatFactory<T>::insert(QcepImageDataFormat<T> *fmt)
{
  m_Formats.append(fmt);
}

template <typename T>
QcepImageDataFormat<T>* QcepImageDataFormatFactory<T>::canLoad(QString path)
{
  QcepImageDataFormat<T> *fmt;
  int priority = 0;
  QcepImageDataFormat<T> *best=NULL;

  foreach(fmt, m_Formats) {
    if (fmt->canLoadFile(path)) {
      if (fmt->priority() > priority) {
        best = fmt;
        priority = fmt->priority();
      }
    }
  }

  return best;
}

#endif

/******************************************************************
*
*  $Log: qcepimagedataformatfactory.h,v $
*  Revision 1.2  2010/09/13 20:00:25  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/04/13 19:29:12  jennings
*  Added qceplib to cvs
*
*  Revision 1.5  2009/06/27 22:50:33  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

