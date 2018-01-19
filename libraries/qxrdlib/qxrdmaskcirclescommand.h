#ifndef QXRDMASKCIRCLESCOMMAND_H
#define QXRDMASKCIRCLESCOMMAND_H

#include "qxrdplotbuttoncommand.h"
#include "qxrdmaskstack-ptr.h"

class QXRD_EXPORT QxrdMaskCirclesCommand : public QxrdPlotButtonCommand
{
public:
  QxrdMaskCirclesCommand(QString                    name,
                         QxrdPlotWidget            *plot,
                         QxrdPlotWidgetSettingsWPtr set,
                         QxrdMaskStackWPtr          maskStack);

private:
  QxrdMaskStackWPtr m_MaskStack;
};

#endif // QXRDMASKCIRCLESCOMMAND_H
