#include "qcepplotpreferencescommand.h"
#include <QToolButton>
#include "qcepimageplot.h"

QcepPlotPreferencesCommand::QcepPlotPreferencesCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("plotPreferencesCommand", "Edit Graph Preferences", plot, set)
{

}

QAction* QcepPlotPreferencesCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Edit Graph Preferences...",
                          m_PlotWidget, &QcepPlotWidget::editPreferences);
}
