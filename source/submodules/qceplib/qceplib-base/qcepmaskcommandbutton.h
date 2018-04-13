#ifndef QCEPMASKCOMMANDBUTTON_H
#define QCEPMASKCOMMANDBUTTON_H

#include "qcepplotbuttoncommand.h"
#include "qcepmaskstack-ptr.h"
#include "qcepmaskpicker-ptr.h"

class QCEP_EXPORT QcepMaskCommandButton : public QcepPlotButtonCommand
{
  Q_OBJECT

public:
  QcepMaskCommandButton(QString                    name,
                        QcepPlotWidget            *plot,
                        QcepPlotWidgetSettingsWPtr set,
                        QcepMaskStackWPtr maskStack);

protected:
  void changeMode(int i);

private:
  void selectedRect(const QRectF& r);
  void selectedPolygon(const QVector<QPointF> &p);

private:
  QcepMaskStackWPtr          m_MaskStack;
  QcepRectangularMaskPicker *m_RectanglePicker;
  QcepCircularMaskPicker    *m_CirclePicker;
  QcepPolygonalMaskPicker   *m_PolygonPicker;
};

Q_DECLARE_METATYPE(QcepMaskCommandButton*)

#endif // QCEPMASKCOMMANDBUTTON_H
