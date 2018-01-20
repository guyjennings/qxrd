#ifndef QXRDMEASURECOMMAND_H
#define QXRDMEASURECOMMAND_H

#include "qxrdplotbuttoncommand.h"
#include "qxrdimageplotmeasurer-ptr.h"

class QXRD_EXPORT QxrdMeasureCommand : public QxrdPlotButtonCommand
{
public:
  QxrdMeasureCommand(QString                    name,
                     QxrdPlotWidget            *plot,
                     QxrdPlotWidgetSettingsWPtr set);

private:
  void selected(const QVector<QPointF> &p);

private:
  QxrdImagePlotMeasurer *m_Measurer;
};

#endif // QXRDMEASURECOMMAND_H
