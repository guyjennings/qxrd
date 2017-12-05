#ifndef QCEPFIXUPGAINMAPCOMMAND_H
#define QCEPFIXUPGAINMAPCOMMAND_H

#include "qceplib_global.h"
#include "qcepobject.h"
#include "qcepimagedata-ptr.h"
#include "qcepdataobject-ptr.h"

class QCEP_EXPORT QcepFixupGainMapCommand : public QcepObject
{
  Q_OBJECT

public:
  QcepFixupGainMapCommand();

public slots:
  void onImageAvailable(QcepDoubleImageDataPtr img);
  QcepDoubleImageDataPtr exec(QcepDoubleImageDataPtr img);

  int intermediateResultCount();
  QcepDataObjectPtr intermediateResult(int i);

signals:
  void processedImageAvailable(QcepDoubleImageDataPtr res);
  void progressMade(int pct);

private:
  QList<QcepDataObjectPtr> m_IntermediateResults;
  double                   m_NSteps;
  double                   m_Step;

  QcepDoubleImageDataPtr   averageSubTiles(QcepDoubleImageDataPtr img, double tileWidth, double tileHeight);
  QcepDoubleImageDataPtr   generateAveragedImage(QcepDoubleImageDataPtr avg, QcepDoubleImageDataPtr img);
  QcepDoubleImageDataPtr   generateAveragedDiff(QcepDoubleImageDataPtr avg, QcepDoubleImageDataPtr img);
  void                     fitParaboloid(QcepDoubleImageDataPtr img);
  QcepDoubleImageDataPtr   generateParaboloid(QcepDoubleImageDataPtr img);
  QcepDoubleImageDataPtr   generateParaboloidDiff(QcepDoubleImageDataPtr par, QcepDoubleImageDataPtr img);

  void clearIntermediateResults();
  void appendIntermediateResult(QcepDataObjectPtr obj);

  void performWork(int nSteps);

public:
  Q_PROPERTY(bool averageSubTiles READ get_AverageSubTiles WRITE set_AverageSubTiles)
  QCEP_BOOLEAN_PROPERTY(AverageSubTiles)

  Q_PROPERTY(double averageWidth READ get_AverageWidth WRITE set_AverageWidth)
  QCEP_DOUBLE_PROPERTY(AverageWidth)

  Q_PROPERTY(double averageHeight READ get_AverageHeight WRITE set_AverageHeight)
  QCEP_DOUBLE_PROPERTY(AverageHeight)

  Q_PROPERTY(bool generateAveragedImage READ get_GenerateAveragedImage WRITE set_GenerateAveragedImage)
  QCEP_BOOLEAN_PROPERTY(GenerateAveragedImage)

  Q_PROPERTY(bool generateAveragedDiff READ get_GenerateAveragedDiff WRITE set_GenerateAveragedDiff)
  QCEP_BOOLEAN_PROPERTY(GenerateAveragedDiff)

  Q_PROPERTY(bool fitParaboloid READ get_FitParaboloid WRITE set_FitParaboloid)
  QCEP_BOOLEAN_PROPERTY(FitParaboloid)

  Q_PROPERTY(bool generateParaboloid READ get_GenerateParaboloid WRITE set_GenerateParaboloid)
  QCEP_BOOLEAN_PROPERTY(GenerateParaboloid)

  Q_PROPERTY(bool generateParaboloidDiff READ get_GenerateParaboloidDiff WRITE set_GenerateParaboloidDiff)
  QCEP_BOOLEAN_PROPERTY(GenerateParaboloidDiff)

  Q_PROPERTY(double fittedOffset READ get_FittedOffset WRITE set_FittedOffset)
  QCEP_DOUBLE_PROPERTY(FittedOffset)

  Q_PROPERTY(double fittedCenterX READ get_FittedCenterX WRITE set_FittedCenterX)
  QCEP_DOUBLE_PROPERTY(FittedCenterX)

  Q_PROPERTY(double fittedCenterY READ get_FittedCenterY WRITE set_FittedCenterY)
  QCEP_DOUBLE_PROPERTY(FittedCenterY)

  Q_PROPERTY(double fittedWidth READ get_FittedWidth WRITE set_FittedWidth)
  QCEP_DOUBLE_PROPERTY(FittedWidth)

  Q_PROPERTY(double fittedHeight READ get_FittedHeight WRITE set_FittedHeight)
  QCEP_DOUBLE_PROPERTY(FittedHeight)
};

#endif // QCEPFIXUPGAINMAPCOMMAND_H
