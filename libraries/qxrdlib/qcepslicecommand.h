#ifndef QCEPSLICECOMMAND_H
#define QCEPSLICECOMMAND_H

#include "qcepplotbuttoncommand.h"
#include "qcepplotslicer-ptr.h"

class QCEP_EXPORT QcepSliceCommand : public QcepPlotButtonCommand
{
  Q_OBJECT

public:
  QcepSliceCommand(QcepPlotWidget            *plot,
                   QcepPlotWidgetSettingsWPtr set);

private:
  void selected(const QVector<QPointF> &p);

private:
  //TODO: ref slice plot
  QcepPlotSlicer *m_Slicer;
};

#endif // QCEPSLICECOMMAND_H
