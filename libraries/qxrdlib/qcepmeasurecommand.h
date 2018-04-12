#ifndef QCEPMEASURECOMMAND_H
#define QCEPMEASURECOMMAND_H

#include "qcepplotbuttoncommand.h"
#include "qcepimageplotmeasurer-ptr.h"

class QCEP_EXPORT QcepMeasureCommand : public QcepPlotButtonCommand
{
  Q_OBJECT

public:
  QcepMeasureCommand(QcepPlotWidget            *plot,
                     QcepPlotWidgetSettingsWPtr set);

private:
  void selected(const QVector<QPointF> &p);

private:
  QcepImagePlotMeasurer *m_Measurer;
};

#endif // QCEPMEASURECOMMAND_H
