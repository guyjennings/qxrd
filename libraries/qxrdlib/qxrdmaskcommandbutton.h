#ifndef QXRDMASKCOMMANDBUTTON_H
#define QXRDMASKCOMMANDBUTTON_H

#include "qxrdplotbuttoncommand.h"
#include "qxrdmaskstack-ptr.h"
#include "qxrdmaskpicker-ptr.h"

class QXRD_EXPORT QxrdMaskCommandButton : public QxrdPlotButtonCommand
{
  Q_OBJECT

public:
  QxrdMaskCommandButton(QString                    name,
                        QxrdPlotWidget            *plot,
                        QxrdPlotWidgetSettingsWPtr set,
                        QxrdMaskStackWPtr          maskStack);

protected:
  void changeMode(int i);

private:
  void selectedRect(const QRectF& r);
  void selectedPolygon(const QVector<QPointF> &p);

private:
  QxrdMaskStackWPtr          m_MaskStack;
  QxrdRectangularMaskPicker *m_RectanglePicker;
  QxrdCircularMaskPicker    *m_CirclePicker;
  QxrdPolygonalMaskPicker   *m_PolygonPicker;
};

#endif // QXRDMASKCOMMANDBUTTON_H
