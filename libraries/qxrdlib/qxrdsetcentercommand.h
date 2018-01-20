#ifndef QXRDSETCENTERCOMMAND_H
#define QXRDSETCENTERCOMMAND_H

#include "qxrdplotbuttoncommand.h"
#include "qxrdcenterfinderpicker-ptr.h"

class QXRD_EXPORT QxrdSetCenterCommand : public QxrdPlotButtonCommand
{
public:
  QxrdSetCenterCommand(QString                    name,
                       QxrdPlotWidget            *plot,
                       QxrdPlotWidgetSettingsWPtr set);

private:
  void selected(const QPointF &pt);

private:
  //TODO: ref center finder
  QxrdCenterFinderPicker *m_Picker;
};

#endif // QXRDSETCENTERCOMMAND_H
