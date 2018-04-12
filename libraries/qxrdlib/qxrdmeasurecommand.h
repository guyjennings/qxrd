#ifndef QXRDMEASURECOMMAND_H
#define QXRDMEASURECOMMAND_H

#include "qcepplotbuttoncommand.h"
#include "qxrdimageplotmeasurer-ptr.h"

class QXRD_EXPORT QxrdMeasureCommand : public QcepPlotButtonCommand
{
  Q_OBJECT

public:
  QxrdMeasureCommand(QcepPlotWidget            *plot,
                     QcepPlotWidgetSettingsWPtr set);

private:
  void selected(const QVector<QPointF> &p);

private:
  QxrdImagePlotMeasurer *m_Measurer;
};

#endif // QXRDMEASURECOMMAND_H
