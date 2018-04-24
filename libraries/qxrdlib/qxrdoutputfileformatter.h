#ifndef QXRDOUTPUTFILEFORMATTER_H
#define QXRDOUTPUTFILEFORMATTER_H

#include "qxrdlib_global.h"
#include "qcepobject.h"

#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"
#include "qxrdoutputfileformattersettings-ptr.h"

class QXRD_EXPORT QxrdOutputFileFormatter : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  QxrdOutputFileFormatter(QString name);
  ~QxrdOutputFileFormatter();

  void initialize(QcepObjectWPtr parent);

  virtual void saveImageData(QxrdOutputFileFormatterSettingsPtr set,
                            QString                             name,
                            QcepImageDataBasePtr                img,
                            QcepImageDataBasePtr                overflow,
                            int                                 canOverwrite);

  virtual void saveImageData(QxrdOutputFileFormatterSettingsPtr set,
                            QcepImageDataBasePtr                img,
                            QcepImageDataBasePtr                overflow) = 0;

protected:
  void    mkPath           (QString filePath);
  QString uniqueFileName   (QcepImageDataBasePtr      data);
};

#endif // QXRDOUTPUTFILEFORMATTER_H
