#ifndef QXRDPOWDERPOINTSCOMMAND_H
#define QXRDPOWDERPOINTSCOMMAND_H

#include "qxrdplotbuttoncommand.h"
#include "qxrdpowderpointpicker-ptr.h"

class QXRD_EXPORT QxrdPowderPointsCommand : public QxrdPlotButtonCommand
{
  Q_OBJECT

public:
  QxrdPowderPointsCommand(QxrdPlotWidget            *plot,
                          QxrdPlotWidgetSettingsWPtr set);

private:
  void selected(const QPointF &pt);

private:
  //TODO: implement data store for powder points
  QxrdPowderPointPicker *m_Picker;
};

#endif // QXRDPOWDERPOINTSCOMMAND_H
