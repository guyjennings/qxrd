#ifndef QXRDSLICECOMMAND_H
#define QXRDSLICECOMMAND_H

#include "qxrdplotbuttoncommand.h"
#include "qxrdplotslicer-ptr.h"

class QXRD_EXPORT QxrdSliceCommand : public QxrdPlotButtonCommand
{
  Q_OBJECT

public:
  QxrdSliceCommand(QxrdPlotWidget            *plot,
                   QxrdPlotWidgetSettingsWPtr set);

private:
  void selected(const QVector<QPointF> &p);

private:
  //TODO: ref slice plot
  QxrdPlotSlicer *m_Slicer;
};

#endif // QXRDSLICECOMMAND_H
