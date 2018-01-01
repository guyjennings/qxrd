#include "qxrdplotpreferencesbutton.h"
#include <QToolButton>
#include "qxrdplotwidget.h"

QxrdPlotPreferencesButton::QxrdPlotPreferencesButton(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotButtonCommand(name, plot, set)
{

}

QToolButton* QxrdPlotPreferencesButton::toolButton()
{
  QToolButton* res = new QToolButton();

  res->setObjectName(get_Name());
  QIcon icon;

  icon.addFile(QStringLiteral(":/images/preferences.png"), QSize(), QIcon::Normal, QIcon::Off);
  res->setIcon(icon);
  res->setIconSize(QSize(24,24));
//  res->setCheckable(true);
//  res->setAutoExclusive(true);
  res->setToolTip(tr("Edit Plot Preferences..."));

  connect(res, &QToolButton::clicked, m_PlotWidget, &QxrdPlotWidget::editPreferences);

  return res;
}
