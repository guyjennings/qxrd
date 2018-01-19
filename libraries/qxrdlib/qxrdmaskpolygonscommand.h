#ifndef QXRDMASKPOLYGONSCOMMAND_H
#define QXRDMASKPOLYGONSCOMMAND_H

#include "qxrdplotbuttoncommand.h"
#include "qxrdmaskstack-ptr.h"

class QXRD_EXPORT QxrdMaskPolygonsCommand : public QxrdPlotButtonCommand
{
public:
  QxrdMaskPolygonsCommand(QString                    name,
                          QxrdPlotWidget            *plot,
                          QxrdPlotWidgetSettingsWPtr set,
                          QxrdMaskStackWPtr          maskStack);

private:
  QxrdMaskStackWPtr m_MaskStack;
};

#endif // QXRDMASKPOLYGONSCOMMAND_H
