#ifndef QXRDOUTPUTFILEFORMATTERSETTINGS_H
#define QXRDOUTPUTFILEFORMATTERSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qcepproperty.h"
#include "qcepimagedata-ptr.h"
#include "qxrdoutputfileformatter-ptr.h"

class QXRD_EXPORT QxrdOutputFileFormatterSettings : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  Q_INVOKABLE QxrdOutputFileFormatterSettings(QString name);
  ~QxrdOutputFileFormatterSettings();

  void initialize(QcepObjectWPtr parent);

  void saveImageData(QString              name,
                     QcepImageDataBasePtr img,
                     QcepImageDataBasePtr overflow,
                     int                  canOverwrite);

  void saveImageData(QcepImageDataBasePtr img,
                     QcepImageDataBasePtr overflow);

  void incBacklog();
  void decBacklog();

private:
  QxrdOutputFileFormatterPtr newOutputFormatter(QString name);

public:
  enum {
    OutputFormatTIFF,
    OutputFormatHDF
  };

  enum {
    OutputCompressionNone,
    OutputCompressionZIP,
    OutputCompressionGZIP,
    OutputCompressionBZIP2
  };

public: // Properties
  Q_PROPERTY(int outputFormat READ get_OutputFormat WRITE set_OutputFormat)
  QCEP_INTEGER_PROPERTY(OutputFormat)

  Q_PROPERTY(int compressFormat READ get_CompressFormat WRITE set_CompressFormat)
  QCEP_INTEGER_PROPERTY(CompressFormat)

  Q_PROPERTY(int compressLevel READ get_CompressLevel WRITE set_CompressLevel)
  QCEP_INTEGER_PROPERTY(CompressLevel)
};

#endif // QXRDOUTPUTFILEFORMATTERSETTINGS_H
