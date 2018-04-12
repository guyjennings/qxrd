#ifndef QXRDPOWDERPOINTSCOMMAND_H
#define QXRDPOWDERPOINTSCOMMAND_H

#include "qcepplotbuttoncommand.h"
#include "qxrdpowderpointpicker-ptr.h"

class QXRD_EXPORT QxrdPowderPointsCommand : public QcepPlotButtonCommand
{
  Q_OBJECT

public:
  QxrdPowderPointsCommand(QcepPlotWidget            *plot,
                          QcepPlotWidgetSettingsWPtr set);

private:
  void selected(const QPointF &pt);

private:
  //TODO: implement data store for powder points
  QxrdPowderPointPicker *m_Picker;
};

#endif // QXRDPOWDERPOINTSCOMMAND_H
