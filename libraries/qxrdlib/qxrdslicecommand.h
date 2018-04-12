#ifndef QXRDSLICECOMMAND_H
#define QXRDSLICECOMMAND_H

#include "qcepplotbuttoncommand.h"
#include "qxrdplotslicer-ptr.h"

class QXRD_EXPORT QxrdSliceCommand : public QcepPlotButtonCommand
{
  Q_OBJECT

public:
  QxrdSliceCommand(QcepPlotWidget            *plot,
                   QcepPlotWidgetSettingsWPtr set);

private:
  void selected(const QVector<QPointF> &p);

private:
  //TODO: ref slice plot
  QxrdPlotSlicer *m_Slicer;
};

#endif // QXRDSLICECOMMAND_H
