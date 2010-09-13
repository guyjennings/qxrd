/******************************************************************
*
*  $Id: qcepimagedataformat.h,v 1.2 2010/09/13 20:00:25 jennings Exp $
*
*******************************************************************/

#ifndef QCEPIMAGEDATAFORMAT_H
#define QCEPIMAGEDATAFORMAT_H

#include "qcepmacros.h"

#include <QObject>
#include <QString>

#include "qcepimagedata.h"
#include "qcepimagedataformatfactory.h"

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
  HEADER_IDENT("$Id: qcepimagedataformat.h,v 1.2 2010/09/13 20:00:25 jennings Exp $");
};

template <typename T>
class QcepImageDataFormat : public QcepImageDataFormatBase
{
 public:
  QcepImageDataFormat(QString fmtname)
    : QcepImageDataFormatBase(fmtname)
  {
//    printf("QcepImageDataFormatFactory<T>::factory()->insert(%s)\n", qPrintable(name()));
//
    QcepImageDataFormatFactory<T>::factory()->insert(this);
  }

  virtual QcepImageDataFormat<T>* canLoadFile(QString path) = 0;
  virtual QcepImageDataFormat<T>* loadFile(QString path, QcepImageData<T> *img) = 0;
  virtual QcepImageDataFormat<T>* saveFile(QString path, QcepImageData<T> *img) = 0;
};

#endif

/******************************************************************
*
*  $Log: qcepimagedataformat.h,v $
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

