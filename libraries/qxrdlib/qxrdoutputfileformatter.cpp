#include "qxrdoutputfileformatter.h"
#include "qxrdoutputfileformattersettings.h"
#include "qxrdoutputfileformattertiff.h"
#include "qxrdoutputfileformatterhdf.h"

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

//void QxrdOutputFileFormatter::saveImageData(QxrdOutputFileFormatterSettingsPtr set,
//                                            QcepImageDataBasePtr               img,
//                                            QcepImageDataBasePtr               overflow)
//{
//  if (set) {
//    switch (set->get_OutputFormat()) {
//    case QxrdOutputFileFormatterSettings::OutputFormatTIFF:
//      QxrdOutputFileFormatterTIFF::saveImageData(set, img, overflow);
//      break;

//    case QxrdOutputFileFormatterSettings::OutputFormatHDF:
//      QxrdOutputFileFormatterHDF::saveImageData(set, img, overflow);
//      break;
//    }
//  }
//}
