#ifndef QCEPPOWDERPOINTSCOMMAND_H
#define QCEPPOWDERPOINTSCOMMAND_H

#include "qcepplotbuttoncommand.h"
#include "qceppowderpointpicker-ptr.h"

class QCEP_EXPORT QcepPowderPointsCommand : public QcepPlotButtonCommand
{
  Q_OBJECT

public:
  QcepPowderPointsCommand(QcepPlotWidget            *plot,
                          QcepPlotWidgetSettingsWPtr set);

private:
  void selected(const QPointF &pt);

private:
  //TODO: implement data store for powder points
  QcepPowderPointPicker *m_Picker;
};

#endif // QCEPPOWDERPOINTSCOMMAND_H
