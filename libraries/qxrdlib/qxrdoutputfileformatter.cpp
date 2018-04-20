#include "qxrdoutputfileformatter.h"

QxrdOutputFileFormatter::QxrdOutputFileFormatter(QString name)
  : inherited(name)
{
}

QxrdOutputFileFormatter::~QxrdOutputFileFormatter()
{
}

void QxrdOutputFileFormatter::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}

void QxrdOutputFileFormatter::saveImageData(QxrdOutputFileFormatterSettingsPtr set,
                                            QString                            name,
                                            QcepImageDataBasePtr               img,
                                            QcepImageDataBasePtr               overflow,
                                            int                                canOverwrite)
{
}
