/******************************************************************
*
*  $Id: qcepimagedata.cpp,v 1.2 2010/09/13 20:00:25 jennings Exp $
*
*******************************************************************/

#include "qcepimagedata.h"
#include <tiffio.h>

#include <QSettings>
#include <QFileInfo>

//static int allocCount = 0;

QcepImageDataBase::QcepImageDataBase(int width, int height)
  : QObject(),
    m_DataType(this, "dataType", UndefinedData),
    m_FileName(this, "fileName", ""),
    m_Title(this, "title", ""),
    m_ReadoutMode(this, "readoutMode", 0),
    m_ExposureTime(this, "exposureTime", 0),
    m_SummedExposures(this, "summedExposures", 0),
    m_ImageNumber(this, "imageNumber", 0),
    m_DateTime(this, "dateTime", QDateTime::currentDateTime()),
    m_HBinning(this, "hBinning", 1),
    m_VBinning(this, "vBinning", 1),
    m_CameraGain(this, "cameraGain", 0),
    m_Triggered(this, "triggered", 0),
    m_UserComment1(this,"userComment1",""),
    m_UserComment2(this,"userComment2",""),
    m_UserComment3(this,"userComment3",""),
    m_UserComment4(this,"userComment4",""),
    m_ImageSaved(this,"imageSaved",0),
    m_Width(width),
    m_Height(height),
    m_Mutex(QMutex::Recursive),
    SOURCE_IDENT("$Id: qcepimagedata.cpp,v 1.2 2010/09/13 20:00:25 jennings Exp $")
{
//    printf("QcepImageDataBase::QcepImageDataBase allocate %d\n", allocCount++);
}

QcepImageDataBase::~QcepImageDataBase()
{
//    printf("QcepImageDataBase::QcepImageDataBase deallocate %d\n", allocCount--);
}

QMutex *QcepImageDataBase::mutex()
{
  return &m_Mutex;
}

void QcepImageDataBase::copyProperties(QcepImageDataBase *dest)
{
  dest -> set_DataType(get_DataType());
  dest -> set_FileName(get_FileName());
  dest -> set_Title(get_Title());
  dest -> set_ReadoutMode(get_ReadoutMode());
  dest -> set_ExposureTime(get_ExposureTime());
  dest -> set_SummedExposures(get_SummedExposures());
  dest -> set_ImageNumber(get_ImageNumber());
  dest -> set_DateTime(get_DateTime());
  dest -> set_HBinning(get_HBinning());
  dest -> set_VBinning(get_VBinning());
  dest -> set_CameraGain(get_CameraGain());
  dest -> set_Triggered(get_Triggered());
  dest -> set_UserComment1(get_UserComment1());
  dest -> set_UserComment2(get_UserComment2());
  dest -> set_UserComment3(get_UserComment3());
  dest -> set_UserComment4(get_UserComment4());
  dest -> set_ImageSaved(get_ImageSaved());

  QByteArray name;

  foreach(name, dynamicPropertyNames()) {
    dest -> setProperty(name.data(), property(name.data()));
  }
}

void QcepImageDataBase::copyPropertiesFrom(QSharedPointer<QcepImageDataBase> src)
{
  set_DataType(src -> get_DataType());
  set_FileName(src -> get_FileName());
  set_Title(src -> get_Title());
  set_ReadoutMode(src -> get_ReadoutMode());
  set_ExposureTime(src -> get_ExposureTime());
  set_SummedExposures(src -> get_SummedExposures());
  set_ImageNumber(src -> get_ImageNumber());
  set_DateTime(src -> get_DateTime());
  set_HBinning(src -> get_HBinning());
  set_VBinning(src -> get_VBinning());
  set_CameraGain(src -> get_CameraGain());
  set_Triggered(src -> get_Triggered());
  set_UserComment1(src -> get_UserComment1());
  set_UserComment2(src -> get_UserComment2());
  set_UserComment3(src -> get_UserComment3());
  set_UserComment4(src -> get_UserComment4());
  set_ImageSaved(src -> get_ImageSaved());

  QByteArray name;

  foreach(name, src -> dynamicPropertyNames()) {
    setProperty(name.data(), src -> property(name.data()));
  }
}

void QcepImageDataBase::setTiffMetaData(TIFF *tif)
{
  TIFFSetField(tif, TIFFTAG_DOCUMENTNAME, qPrintable(get_FileName()));
  TIFFSetField(tif, TIFFTAG_DATETIME, qPrintable(get_DateTime().toString("yyyy:MM:dd hh:mm:ss")));
}

void QcepImageDataBase::getTiffMetaData(TIFF */*tif*/)
{
}

void QcepImageDataBase::loadMetaData()
{
//  printf("QcepImageDataBase::loadMetaData for file %s\n", qPrintable(get_FileName()));

  QTime tic;
  tic.start();

  {
    QMutexLocker lock(mutex());

    QSettings settings(get_FileName()+".metadata", QSettings::IniFormat);

    QcepProperty::readSettings(this, metaObject(), "metadata", settings);
  }
//
//  printf("QcepImageDataBase::loadMetaData for file %s took %d msec\n",  qPrintable(get_FileName()), tic.elapsed());
}

void QcepImageDataBase::saveMetaData()
{
  saveMetaData(get_FileName());
}

void QcepImageDataBase::saveMetaData(QString name)
{
//  printf("QcepImageDataBase::saveMetaData for file %s\n", qPrintable(name));

  QTime tic;
  tic.start();

  {
    QMutexLocker lock(mutex());

    QSettings settings(name+".metadata", QSettings::IniFormat);

    QcepProperty::writeSettings(this, metaObject(), "metadata", settings);
  }
//
//  printf("QcepImageDataBase::saveMetaData for file %s took %d msec\n",  qPrintable(name), tic.elapsed());
}

void QcepImageDataBase::setDefaultFileName(QString path)
{
  set_FileName(path);
  set_Title(QFileInfo(path).fileName());
}

/******************************************************************
*
*  $Log: qcepimagedata.cpp,v $
*  Revision 1.2  2010/09/13 20:00:25  jennings
*  Merged
*
*  Revision 1.1.2.3  2010/06/17 18:35:33  jennings
*  Added copyPropertiesFrom method
*
*  Revision 1.1.2.2  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.1.2.1  2010/04/13 19:29:12  jennings
*  Added qceplib to cvs
*
*  Revision 1.22  2010/03/02 22:15:44  jennings
*  Removed some debug output, files for windows installer
*
*  Revision 1.21  2009/12/11 17:53:09  jennings
*  Added ImageSaved property to image data
*
*  Revision 1.20  2009/11/13 20:16:05  jennings
*  *** empty log message ***
*
*  Revision 1.19  2009/09/26 04:57:31  jennings
*  Removed some commented out sections
*
*  Revision 1.18  2009/09/25 16:09:02  jennings
*  Added copyImage function to QcepImageData<>
*
*  Revision 1.17  2009/09/22 20:41:07  jennings
*  Set filename and title properties when loading data files
*
*  Revision 1.16  2009/09/22 20:37:23  jennings
*  Dynamic properties attached to the acquisition object are propagated through to saved images
*
*  Revision 1.15  2009/09/21 18:12:55  jennings
*  Added 'triggered', 'usercomment{1-4}' properties to data
*
*  Revision 1.14  2009/09/20 21:18:53  jennings
*  Removed 'printf' messages
*  Added printMessage, statusMessage and criticalMessage functiosn for major classes.
*
*  Revision 1.13  2009/09/14 19:09:33  jennings
*  Added hBinning, vBinning and cameraGain properties to images
*
*  Revision 1.12  2009/09/12 14:44:53  jennings
*  Moved lock to base class, made into mutex
*  Added DataType property
*
*  Revision 1.11  2009/09/09 22:32:12  jennings
*  Started to add TIFF metadata support
*
*  Revision 1.10  2009/08/26 20:53:04  jennings
*  Added copyProperties function to facilitate conversion between image data types
*
*  Revision 1.9  2009/08/25 18:49:19  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.8  2009/08/04 16:45:20  jennings
*  Moved mask data into separate class
*
*  Revision 1.7  2009/08/03 13:27:25  jennings
*  Moved mask-related routines into non-templated base class
*  Added maskCircle routine
*
*  Revision 1.6  2009/06/28 11:52:49  jennings
*  Optimized image operations by special case handling of width and height properties
*
*  Revision 1.5  2009/06/27 22:50:33  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

