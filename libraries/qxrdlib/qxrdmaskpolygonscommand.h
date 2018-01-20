#ifndef QXRDMASKPOLYGONSCOMMAND_H
#define QXRDMASKPOLYGONSCOMMAND_H

#include "qxrdplotbuttoncommand.h"
#include "qxrdmaskstack-ptr.h"
#include "qxrdmaskpicker-ptr.h"

class QXRD_EXPORT QxrdMaskPolygonsCommand : public QxrdPlotButtonCommand
{
public:
  QxrdMaskPolygonsCommand(QString                    name,
                          QxrdPlotWidget            *plot,
                          QxrdPlotWidgetSettingsWPtr set,
                          QxrdMaskStackWPtr          maskStack);

private:
  void selected(const QVector<QPointF> &p);

private:
  QxrdMaskStackWPtr        m_MaskStack;
  QxrdPolygonalMaskPicker *m_Picker;
};

#endif // QXRDMASKPOLYGONSCOMMAND_H
