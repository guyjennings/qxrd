#ifndef QCEPCOLORMAPLIBRARY_H
#define QCEPCOLORMAPLIBRARY_H

#include "qceplib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepcolormap-ptr.h"
#include "qcepmaskcolormap-ptr.h"
#include "qcepimageplotwidgetsettings-ptr.h"

class QCEP_EXPORT QcepColorMapLibrary : public QcepObject
{
  Q_OBJECT

public:
  Q_INVOKABLE QcepColorMapLibrary(QString name);

  static int colorMapCount();
  static QString colorMapName(int n);
  static QcepColorMap* newImageColorMap(int n, QcepImagePlotWidgetSettingsWPtr set);
  static QcepMaskColorMap* newOverflowColorMap(int n, QcepImagePlotWidgetSettingsWPtr set);
  static QcepMaskColorMap* newMaskColorMap(int n, QcepImagePlotWidgetSettingsWPtr set);
  static QPen trackerPen(int n);

private:
  static void colorMapStart(QcepColorMap *map,
                            QColor startColor,
                            QColor endColor);

  static void colorMapRange(QcepColorMap *map,
                            double value1,
                            QColor color1,
                            double value2,
                            QColor color2);

  static QcepColorMap* newGrayscaleImageColorMap(QcepImagePlotWidgetSettingsWPtr set);
  static QcepColorMap* newInverseGrayscaleImageColorMap(QcepImagePlotWidgetSettingsWPtr set);
  static QcepColorMap* newEarthTonesImageColorMap(QcepImagePlotWidgetSettingsWPtr set);
  static QcepColorMap* newSpectrumImageColorMap(QcepImagePlotWidgetSettingsWPtr set);
  static QcepColorMap* newFireImageColorMap(QcepImagePlotWidgetSettingsWPtr set);
  static QcepColorMap* newIceImageColorMap(QcepImagePlotWidgetSettingsWPtr set);
  static QcepColorMap* newDefaultImageColorMap(QcepImagePlotWidgetSettingsWPtr set);

  static QcepMaskColorMap* newGrayscaleOverflowColorMap(QcepImagePlotWidgetSettingsWPtr set);
  static QcepMaskColorMap* newInverseGrayscaleOverflowColorMap(QcepImagePlotWidgetSettingsWPtr set);
  static QcepMaskColorMap* newEarthTonesOverflowColorMap(QcepImagePlotWidgetSettingsWPtr set);
  static QcepMaskColorMap* newSpectrumOverflowColorMap(QcepImagePlotWidgetSettingsWPtr set);
  static QcepMaskColorMap* newFireImageOverflowMap(QcepImagePlotWidgetSettingsWPtr set);
  static QcepMaskColorMap* newIceImageOverflowMap(QcepImagePlotWidgetSettingsWPtr set);
  static QcepMaskColorMap* newDefaultImageOverflowMap(QcepImagePlotWidgetSettingsWPtr set);

  static QcepMaskColorMap* newGrayscaleMaskColorMap(QcepImagePlotWidgetSettingsWPtr set);
  static QcepMaskColorMap* newInverseGrayscaleMaskColorMap(QcepImagePlotWidgetSettingsWPtr set);
  static QcepMaskColorMap* newEarthTonesMaskColorMap(QcepImagePlotWidgetSettingsWPtr set);
  static QcepMaskColorMap* newSpectrumMaskColorMap(QcepImagePlotWidgetSettingsWPtr set);
  static QcepMaskColorMap* newFireMaskColorMap(QcepImagePlotWidgetSettingsWPtr set);
  static QcepMaskColorMap* newIceMaskColorMap(QcepImagePlotWidgetSettingsWPtr set);
  static QcepMaskColorMap* newDefaultMaskColorMap(QcepImagePlotWidgetSettingsWPtr set);
};

#endif // QCEPCOLORMAPLIBRARY_H
