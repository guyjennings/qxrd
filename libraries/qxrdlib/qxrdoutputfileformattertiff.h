#ifndef QXRDOUTPUTFILEFORMATTERTIFF_H
#define QXRDOUTPUTFILEFORMATTERTIFF_H

#include "qxrdlib_global.h"
#include "qcepobject.h"

#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"
#include "qxrdoutputfileformattersettings-ptr.h"
#include "qxrdoutputfileformatter.h"
#include "tiffio.h"
#include <sstream>

class QXRD_EXPORT QxrdOutputFileFormatterTIFF : public QxrdOutputFileFormatter
{
  Q_OBJECT

private:
  typedef QxrdOutputFileFormatter inherited;

public:
  Q_INVOKABLE QxrdOutputFileFormatterTIFF(QString name);

  void saveImageData(QxrdOutputFileFormatterSettingsPtr set,
                     QcepImageDataBasePtr               img,
                     QcepImageDataBasePtr               overflow);

private:
  QTime                              m_Tic;
  int                                m_Compression;
  QxrdOutputFileFormatterSettingsPtr m_Settings;
  QString                            m_FileName;
  std::ostringstream                 m_OutputStream;
  TIFF*                              m_OutputTIFF;
  QcepImageDataBasePtr               m_Image;
  int                                m_NRows;
  int                                m_NCols;

  void tiffCheck(int stat, const char* file, int line);

  void beginOutputData(int nBits, int pixFormat);

  void saveDoubleData(QxrdOutputFileFormatterSettingsPtr set,
                      QcepDoubleImageDataPtr             dimg,
                      QcepImageDataBasePtr               overflow);

  void saveInt32Data (QxrdOutputFileFormatterSettingsPtr set,
                      QcepUInt32ImageDataPtr             i32img,
                      QcepImageDataBasePtr               overflow);

  void saveInt16Data (QxrdOutputFileFormatterSettingsPtr set,
                      QcepUInt16ImageDataPtr             i16img,
                      QcepImageDataBasePtr               overflow);

  void endOutputData();
};

#endif // QXRDOUTPUTFILEFORMATTERTIFF_H
