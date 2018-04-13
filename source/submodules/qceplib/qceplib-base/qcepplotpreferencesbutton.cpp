#include "qcepplotpreferencesbutton.h"
#include <QToolButton>
#include "qcepplotwidget.h"

QcepPlotPreferencesButton::QcepPlotPreferencesButton(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotButtonCommand("plotPreferencesButton", "Edit Plot Preferences...", plot, set, ":/images/preferences.png", false)
{
  connect(m_ToolButton, &QToolButton::clicked, m_PlotWidget, &QcepPlotWidget::editPreferences);
}
