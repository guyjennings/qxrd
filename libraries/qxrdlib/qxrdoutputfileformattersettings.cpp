#include "qxrdoutputfileformattersettings.h"
#include "qxrdoutputfileformatter.h"
#include "qxrdoutputfileformatter-ptr.h"
#include <QtConcurrentRun>
#include "qcepimagedata.h"
#include "qxrdoutputfileformatterhdf.h"
#include "qxrdoutputfileformattertiff.h"

QxrdOutputFileFormatterSettings::QxrdOutputFileFormatterSettings(QString name)
  : inherited(name),
    m_OutputFormat  (this, "outputFormat",   OutputFormatTIFF,      "Output Format"),
    m_CompressFormat(this, "compressFormat", OutputCompressionNone, "Output Compression Format"),
    m_CompressLevel (this, "compressLevel",  3,                     "Output Compression Level")
{
}

QxrdOutputFileFormatterSettings::~QxrdOutputFileFormatterSettings()
{
}

void QxrdOutputFileFormatterSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}

void QxrdOutputFileFormatterSettings::saveImageData(QString              name,
                                                    QcepImageDataBasePtr img,
                                                    QcepImageDataBasePtr overflow,
                                                    int                  canOverwrite)
{
  QxrdOutputFileFormatterSettingsPtr set =
      qSharedPointerDynamicCast<QxrdOutputFileFormatterSettings>(sharedFromThis());

  QxrdOutputFileFormatterPtr fmt = newOutputFormatter("formatter");

  incBacklog();

  QtConcurrent::run([=]() { fmt -> saveImageData(set, name, img, overflow, canOverwrite); } );
}

void QxrdOutputFileFormatterSettings::saveImageData(QcepImageDataBasePtr img,
                                                    QcepImageDataBasePtr overflow)
{
  QxrdOutputFileFormatterSettingsPtr set =
      qSharedPointerDynamicCast<QxrdOutputFileFormatterSettings>(sharedFromThis());

  QxrdOutputFileFormatterPtr fmt = newOutputFormatter("formatter");

  incBacklog();

  QtConcurrent::run([=]() { fmt -> saveImageData(set, img, overflow); } );
}

QxrdOutputFileFormatterPtr QxrdOutputFileFormatterSettings::newOutputFormatter(QString name)
{
  QxrdOutputFileFormatterPtr res;

  switch (get_OutputFormat()) {
  case OutputFormatTIFF:
    res = QxrdOutputFileFormatterPtr(
          new QxrdOutputFileFormatterTIFF(name),
          &QObject::deleteLater);
    break;

  case OutputFormatHDF:
    res = QxrdOutputFileFormatterPtr(
          new QxrdOutputFileFormatterHDF(name),
          &QObject::deleteLater);
    break;
  }

  if (res) {
    res -> initialize(sharedFromThis());
  }

  return res;
}

void QxrdOutputFileFormatterSettings::incBacklog()
{
}

void QxrdOutputFileFormatterSettings::decBacklog()
{
}
