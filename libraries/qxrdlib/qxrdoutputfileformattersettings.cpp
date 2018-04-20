#include "qxrdoutputfileformattersettings.h"
#include "qxrdoutputfileformatter.h"
#include <QtConcurrentRun>
#include "qcepimagedata.h"

QxrdOutputFileFormatterSettings::QxrdOutputFileFormatterSettings(QString name)
  : inherited(name)
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

  QtConcurrent::run(&QxrdOutputFileFormatter::saveImageData, set, name, img, overflow, canOverwrite);
}
