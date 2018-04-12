#ifndef QXRDMASKPOLYGONSCOMMAND_H
#define QXRDMASKPOLYGONSCOMMAND_H

#include "qcepplotbuttoncommand.h"
#include "qxrdmaskstack-ptr.h"
#include "qxrdmaskpicker-ptr.h"

class QXRD_EXPORT QxrdMaskPolygonsCommand : public QcepPlotButtonCommand
{
  Q_OBJECT

public:
  QxrdMaskPolygonsCommand(QcepPlotWidget            *plot,
                          QcepPlotWidgetSettingsWPtr set,
                          QxrdMaskStackWPtr          maskStack);

private:
  void selected(const QVector<QPointF> &p);

private:
  QxrdMaskStackWPtr        m_MaskStack;
  QxrdPolygonalMaskPicker *m_Picker;
};

#endif // QXRDMASKPOLYGONSCOMMAND_H
