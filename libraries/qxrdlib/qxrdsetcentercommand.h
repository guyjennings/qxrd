#ifndef QXRDSETCENTERCOMMAND_H
#define QXRDSETCENTERCOMMAND_H

#include "qxrdplotbuttoncommand.h"
#include "qxrdcenterfinderpicker-ptr.h"
#include "qxrdcenterfinder-ptr.h"

class QXRD_EXPORT QxrdSetCenterCommand : public QxrdPlotButtonCommand
{
  Q_OBJECT

public:
  QxrdSetCenterCommand(QxrdPlotWidget            *plot,
                       QxrdPlotWidgetSettingsWPtr set,
                       QxrdCenterFinderWPtr       cf);

private:
//  void selected(const QPointF &pt);

private:
  QxrdCenterFinderWPtr    m_CenterFinder;
  QxrdCenterFinderPicker *m_Picker;
};

#endif // QXRDSETCENTERCOMMAND_H
