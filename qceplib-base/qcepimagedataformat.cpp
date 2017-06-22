#include "qcepimagedataformat.h"
#include "qcepimagedataformatfactory.h"
#include <QFileInfo>
#include <QDir>

void QcepImageDataFormatBase::mkPath(QString filePath)
{
  QFileInfo f(filePath);
  QDir dir = f.dir();

  if (!dir.exists()) {
    dir.mkpath(dir.absolutePath());
  }
}

QString QcepImageDataFormatBase::uniqueFileName(QString name)
{
  QFileInfo f(name);

  if (f.exists()) {
    QDir dir = f.dir();
    QString base = f.baseName();
    QString suff = f.completeSuffix();

    int width = 5;

    for (int i=1; ; i++) {
      QString newname = dir.filePath(base+QString().sprintf("-%0*d.",width,i)+suff);
      QFileInfo f(newname);

      if (!f.exists()) {
        return newname;
      }
    }
  } else {
    return name;
  }
}

template <typename T>
QcepImageDataFormat<T>::QcepImageDataFormat(QString fmtname)
  : QcepImageDataFormatBase(fmtname)
{
//    printf("QcepImageDataFormatFactory<T>::factory()->insert(%s)\n", qPrintable(name()));
//
  QcepImageDataFormatFactory<T>::factory()->insert(this);
}

template <typename T>
QcepImageDataFormat<T>::~QcepImageDataFormat()
{
  QcepImageDataFormatFactory<T>::factory()->remove(this);
}

template class QcepImageDataFormat<unsigned short>;
template class QcepImageDataFormat<short>;
template class QcepImageDataFormat<unsigned int>;
template class QcepImageDataFormat<int>;
template class QcepImageDataFormat<double>;
