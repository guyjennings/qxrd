#include "qxrdplotpreferencescommand.h"
#include <QToolButton>
#include "qxrdimageplot.h"

QxrdPlotPreferencesCommand::QxrdPlotPreferencesCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand(name, plot, set)
{

}

QAction* QxrdPlotPreferencesCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Edit Graph Preferences...",
                          m_PlotWidget, &QxrdPlotWidget::editPreferences);
}
