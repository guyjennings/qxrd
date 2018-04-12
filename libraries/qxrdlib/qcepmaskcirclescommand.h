#ifndef QCEPMASKCIRCLESCOMMAND_H
#define QCEPMASKCIRCLESCOMMAND_H

#include "qcepplotbuttoncommand.h"
#include "qcepmaskstack-ptr.h"
#include "qcepmaskpicker-ptr.h"

class QCEP_EXPORT QcepMaskCirclesCommand : public QcepPlotButtonCommand
{
  Q_OBJECT

public:
  QcepMaskCirclesCommand(QcepPlotWidget            *plot,
                         QcepPlotWidgetSettingsWPtr set,
                         QcepMaskStackWPtr maskStack);

private:
  void selected(const QRectF& r);

private:
  QcepMaskStackWPtr       m_MaskStack;
  QcepCircularMaskPicker *m_Picker;
};

Q_DECLARE_METATYPE(QcepMaskCirclesCommand*)

#endif // QCEPMASKCIRCLESCOMMAND_H
