#include "qxrdplotpreferencesbutton.h"
#include <QToolButton>
#include "qxrdplotwidget.h"

QxrdPlotPreferencesButton::QxrdPlotPreferencesButton(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotButtonCommand(name, plot, set, ":/images/preferences.png", "Edit Plot Preferences...", false)
{
  connect(m_ToolButton, &QToolButton::clicked, m_PlotWidget, &QxrdPlotWidget::editPreferences);
}
