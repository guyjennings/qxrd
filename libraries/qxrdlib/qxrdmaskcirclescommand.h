#ifndef QXRDMASKCIRCLESCOMMAND_H
#define QXRDMASKCIRCLESCOMMAND_H

#include "qcepplotbuttoncommand.h"
#include "qxrdmaskstack-ptr.h"
#include "qxrdmaskpicker-ptr.h"

class QXRD_EXPORT QxrdMaskCirclesCommand : public QcepPlotButtonCommand
{
  Q_OBJECT

public:
  QxrdMaskCirclesCommand(QcepPlotWidget            *plot,
                         QcepPlotWidgetSettingsWPtr set,
                         QxrdMaskStackWPtr          maskStack);

private:
  void selected(const QRectF& r);

private:
  QxrdMaskStackWPtr       m_MaskStack;
  QxrdCircularMaskPicker *m_Picker;
};

Q_DECLARE_METATYPE(QxrdMaskCirclesCommand*)

#endif // QXRDMASKCIRCLESCOMMAND_H
