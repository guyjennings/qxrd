#include "qxrdplotpreferencescommand.h"
#include <QToolButton>
#include "qxrdimageplot.h"

QxrdPlotPreferencesCommand::QxrdPlotPreferencesCommand(QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand("plotPreferencesCommand", "Edit Graph Preferences", plot, set)
{

}

QAction* QxrdPlotPreferencesCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Edit Graph Preferences...",
                          m_PlotWidget, &QxrdPlotWidget::editPreferences);
}
