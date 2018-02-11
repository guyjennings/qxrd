#include "qxrdplotpreferencesbutton.h"
#include <QToolButton>
#include "qxrdplotwidget.h"

QxrdPlotPreferencesButton::QxrdPlotPreferencesButton(QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotButtonCommand("plotPreferencesButton", "Edit Plot Preferences...", plot, set, ":/images/preferences.png", false)
{
  connect(m_ToolButton, &QToolButton::clicked, m_PlotWidget, &QxrdPlotWidget::editPreferences);
}
