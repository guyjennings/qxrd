#ifndef QXRDPROCESSOR_H
#define QXRDPROCESSOR_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"
#include "qxrdprocessorstep-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdfilesaver-ptr.h"
#include "qxrdmaskstack-ptr.h"
#include "qxrdzingerfinder-ptr.h"
#include "qxrdcenterfinder-ptr.h"

//TODO: merge QxrdDataProcessor and QxrdDetectorProcessor into QxrdProcessor
//TODO: separate processing steps into sub-objects
//TODO: need separate settings class for experiment thread

class QXRD_EXPORT QxrdProcessor : public QcepObject
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdProcessor(QString name);
  virtual ~QxrdProcessor();

  QxrdExperimentWPtr   experiment() const;
  QxrdFileSaverWPtr    fileSaver() const;
  QxrdCenterFinderWPtr centerFinder() const;

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  enum {
    NoOverwrite,
    CanOverwrite
  };

  // Data operations...
  void loadData(QString name);
  void saveData(QString name, int canOverwrite=NoOverwrite);
  void clearData();
  QcepImageDataBaseWPtr data() const;
  void newData(QcepImageDataBaseWPtr image);
  QString dataDirectory() const;
  QString filePathInDataDirectory(QString name) const;

  // dark operations...
  void loadDark(QString name);
  void saveDark(QString name, int canOverwrite=NoOverwrite);
  void clearDark();
  QcepDoubleImageDataWPtr dark() const;
  void newDark(QcepDoubleImageDataWPtr imageW);
  void newDarkInt16(QcepUInt16ImageDataWPtr imageW);
  void newDarkInt32(QcepUInt32ImageDataWPtr imageW);

  // mask operations...
  void loadMask(QString name);
  void saveMask(QString name, int canOverwrite=NoOverwrite);
  void clearMask();
  QcepMaskDataWPtr        mask() const;
  void newMask(QcepMaskDataWPtr mask);

  // bad pixel operations...
  void loadBadPixels(QString name);
  void saveBadPixels(QString name, int canOverwrite=NoOverwrite);
  void clearBadPixels();
  QcepDoubleImageDataWPtr badPixels() const;
  void newBadPixelsImage(QcepDoubleImageDataWPtr image);

  // gain map operations...
  void loadGainMap(QString name);
  void saveGainMap(QString name, int canOverwrite=NoOverwrite);
  void clearGainMap();
  QcepDoubleImageDataWPtr gainMap() const;
  void newGainMapImage(QcepDoubleImageDataWPtr image);

  // live data operations...
  QcepDoubleImageDataWPtr liveData() const;
  void newLiveData(QcepDoubleImageDataWPtr image);

  // overflow operations...
  QcepMaskDataWPtr        overflow() const;
  void newOverflow(QcepMaskDataWPtr ovf);

  void loadDefaultImages();

  QxrdMaskStackWPtr maskStack() const;

  void newEmptyMask();
  void duplicateMask();

  void showMaskRange();
  void hideMaskRange();
  void showMaskRange(double min, double max);
  void hideMaskRange(double min, double max);
  void showMaskAll();
  void hideMaskAll();
  void invertMask();
  void growMask();
  void shrinkMask();
  void maskCircle(QRectF pt);
  void maskPolygon(QVector<QPointF> poly);

  QxrdZingerFinderWPtr zingerFinder() const;

protected:
  void subtractDarkImage(QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark);
  void unsubtractDarkImage(QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark);

  void saveNamedImageData(QString name, QcepImageDataBasePtr image, QcepMaskDataPtr overflow, int canOverwrite=NoOverwrite);
  void saveNamedDoubleImageData(QString name, QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite=NoOverwrite);
  void saveNamedUInt16ImageData(QString name, QcepUInt16ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite=NoOverwrite);
  void saveNamedUInt32ImageData(QString name, QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite=NoOverwrite);
  void saveNamedRawImageData(QString name, QcepUInt16ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite=NoOverwrite);
  void saveNamedRawImageData(QString name, QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite=NoOverwrite);
  void saveNamedMaskData(QString name, QcepMaskDataPtr mask, int canOverwrite=NoOverwrite);
  void saveNamedImageDataAsText(QString name, QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite=NoOverwrite);

  void createMaskIfNeeded();
  int newMaskWidth() const;
  int newMaskHeight() const;

signals:
  void dataAvailable      (QcepDoubleImageDataPtr img);
  void darkAvailable      (QcepDoubleImageDataPtr img);
  void maskAvailable      (QcepMaskDataPtr        mask);
  void badPixelsAvailable (QcepDoubleImageDataPtr img);
  void gainMapAvailable   (QcepDoubleImageDataPtr map);
  void liveDataAvailable  (QcepDoubleImageDataPtr img);
  void overflowAvailable  (QcepMaskDataPtr        ovf);

public:
  // Properties...

  Q_PROPERTY(QString dataPath   READ get_DataPath WRITE set_DataPath STORED false)
  QCEP_STRING_PROPERTY(DataPath)

  //TODO: rename darkImage
  Q_PROPERTY(QString darkImagePath   READ get_DarkImagePath WRITE set_DarkImagePath)
  QCEP_STRING_PROPERTY(DarkImagePath)

  Q_PROPERTY(QString badPixelsPath   READ get_BadPixelsPath WRITE set_BadPixelsPath)
  QCEP_STRING_PROPERTY(BadPixelsPath)

  Q_PROPERTY(QString gainMapPath     READ get_GainMapPath WRITE set_GainMapPath)
  QCEP_STRING_PROPERTY(GainMapPath)

  Q_PROPERTY(QString maskPath     READ get_MaskPath WRITE set_MaskPath)
  QCEP_STRING_PROPERTY(MaskPath)

  Q_PROPERTY(QString scriptPath   READ get_ScriptPath WRITE set_ScriptPath)
  QCEP_STRING_PROPERTY(ScriptPath)

  Q_PROPERTY(double     average    READ get_Average WRITE set_Average STORED false)
  QCEP_DOUBLE_PROPERTY(Average)

  Q_PROPERTY(double     averageDark    READ get_AverageDark WRITE set_AverageDark STORED false)
  QCEP_DOUBLE_PROPERTY(AverageDark)

  Q_PROPERTY(double     averageRaw    READ get_AverageRaw WRITE set_AverageRaw STORED false)
  QCEP_DOUBLE_PROPERTY(AverageRaw)

  Q_PROPERTY(double maskMinimumValue READ get_MaskMinimumValue WRITE set_MaskMinimumValue)
  QCEP_DOUBLE_PROPERTY(MaskMinimumValue)

  Q_PROPERTY(double maskMaximumValue READ get_MaskMaximumValue WRITE set_MaskMaximumValue)
  QCEP_DOUBLE_PROPERTY(MaskMaximumValue)

  Q_PROPERTY(double maskCircleRadius READ get_MaskCircleRadius WRITE set_MaskCircleRadius)
  QCEP_DOUBLE_PROPERTY(MaskCircleRadius)

  Q_PROPERTY(bool maskSetPixels READ get_MaskSetPixels WRITE set_MaskSetPixels)
  QCEP_BOOLEAN_PROPERTY(MaskSetPixels)

protected:
    QcepImageDataBasePtr   m_Data;
    QcepDoubleImageDataPtr m_Dark;
    QcepDoubleImageDataPtr m_BadPixels;
    QcepDoubleImageDataPtr m_GainMap;
    QcepDoubleImageDataPtr m_LiveData;
    QcepMaskDataPtr        m_Overflow;

    QxrdMaskStackPtr       m_MaskStack;
    QxrdZingerFinderPtr    m_ZingerFinder;

    QVector<QxrdProcessorStepPtr> m_ProcessorSteps;

private:
    QxrdCenterFinderPtr    m_CenterFinder;
};

#endif // QXRDPROCESSOR_H
