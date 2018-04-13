#ifndef QCEPMASKPOLYGONSCOMMAND_H
#define QCEPMASKPOLYGONSCOMMAND_H

#include "qcepplotbuttoncommand.h"
#include "qcepmaskstack-ptr.h"
#include "qcepmaskpicker-ptr.h"

class QCEP_EXPORT QcepMaskPolygonsCommand : public QcepPlotButtonCommand
{
  Q_OBJECT

public:
  QcepMaskPolygonsCommand(QcepPlotWidget            *plot,
                          QcepPlotWidgetSettingsWPtr set,
                          QcepMaskStackWPtr maskStack);

private:
  void selected(const QVector<QPointF> &p);

private:
  QcepMaskStackWPtr        m_MaskStack;
  QcepPolygonalMaskPicker *m_Picker;
};

#endif // QCEPMASKPOLYGONSCOMMAND_H
