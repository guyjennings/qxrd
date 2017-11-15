#include "qxrdplotcommand.h"

QxrdPlotCommand::QxrdPlotCommand(QString name, QxrdImagePlot *plot)
  : QcepSerializableObject(name),
    m_Plot(plot)
{

}
