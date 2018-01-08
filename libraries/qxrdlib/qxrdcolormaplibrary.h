#ifndef QXRDCOLORMAPLIBRARY_H
#define QXRDCOLORMAPLIBRARY_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"
#include "qxrdcolormap-ptr.h"
#include "qxrdmaskcolormap-ptr.h"
#include "qxrdimageplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdColorMapLibrary : public QcepObject
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdColorMapLibrary(QString name);

  static int colorMapCount();
  static QString colorMapName(int n);
  static QxrdColorMap* newImageColorMap(int n, QxrdImagePlotWidgetSettingsWPtr set);
  static QxrdMaskColorMap* newOverflowColorMap(int n, QxrdImagePlotWidgetSettingsWPtr set);
  static QxrdMaskColorMap* newMaskColorMap(int n, QxrdImagePlotWidgetSettingsWPtr set);

private:
  static void colorMapStart(QxrdColorMap *map,
                            QColor startColor,
                            QColor endColor);

  static void colorMapRange(QxrdColorMap *map,
                            double value1,
                            QColor color1,
                            double value2,
                            QColor color2);

  static QxrdColorMap* newGrayscaleImageColorMap(QxrdImagePlotWidgetSettingsWPtr set);
  static QxrdColorMap* newInverseGrayscaleImageColorMap(QxrdImagePlotWidgetSettingsWPtr set);
  static QxrdColorMap* newEarthTonesImageColorMap(QxrdImagePlotWidgetSettingsWPtr set);
  static QxrdColorMap* newSpectrumImageColorMap(QxrdImagePlotWidgetSettingsWPtr set);
  static QxrdColorMap* newFireImageColorMap(QxrdImagePlotWidgetSettingsWPtr set);
  static QxrdColorMap* newIceImageColorMap(QxrdImagePlotWidgetSettingsWPtr set);
  static QxrdColorMap* newDefaultImageColorMap(QxrdImagePlotWidgetSettingsWPtr set);

  static QxrdMaskColorMap* newGrayscaleOverflowColorMap(QxrdImagePlotWidgetSettingsWPtr set);
  static QxrdMaskColorMap* newInverseGrayscaleOverflowColorMap(QxrdImagePlotWidgetSettingsWPtr set);
  static QxrdMaskColorMap* newEarthTonesOverflowColorMap(QxrdImagePlotWidgetSettingsWPtr set);
  static QxrdMaskColorMap* newSpectrumOverflowColorMap(QxrdImagePlotWidgetSettingsWPtr set);
  static QxrdMaskColorMap* newFireImageOverflowMap(QxrdImagePlotWidgetSettingsWPtr set);
  static QxrdMaskColorMap* newIceImageOverflowMap(QxrdImagePlotWidgetSettingsWPtr set);
  static QxrdMaskColorMap* newDefaultImageOverflowMap(QxrdImagePlotWidgetSettingsWPtr set);

  static QxrdMaskColorMap* newGrayscaleMaskColorMap(QxrdImagePlotWidgetSettingsWPtr set);
  static QxrdMaskColorMap* newInverseGrayscaleMaskColorMap(QxrdImagePlotWidgetSettingsWPtr set);
  static QxrdMaskColorMap* newEarthTonesMaskColorMap(QxrdImagePlotWidgetSettingsWPtr set);
  static QxrdMaskColorMap* newSpectrumMaskColorMap(QxrdImagePlotWidgetSettingsWPtr set);
  static QxrdMaskColorMap* newFireMaskColorMap(QxrdImagePlotWidgetSettingsWPtr set);
  static QxrdMaskColorMap* newIceMaskColorMap(QxrdImagePlotWidgetSettingsWPtr set);
  static QxrdMaskColorMap* newDefaultMaskColorMap(QxrdImagePlotWidgetSettingsWPtr set);
};

#endif // QXRDCOLORMAPLIBRARY_H
