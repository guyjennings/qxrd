#include "qxrdplotpreferencescommand.h"
#include <QToolButton>
#include "qxrdimageplot.h"

QxrdPlotPreferencesCommand::QxrdPlotPreferencesCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotButtonCommand(name, plot, set)
{

}

QToolButton* QxrdPlotPreferencesCommand::toolButton()
{
  QToolButton* res = new QToolButton();

  res->setObjectName(get_Name());
  QIcon icon;

  icon.addFile(QStringLiteral(":/images/preferences.png"), QSize(), QIcon::Normal, QIcon::Off);
  res->setIcon(icon);
  res->setIconSize(QSize(24,24));
//  res->setCheckable(true);
//  res->setAutoExclusive(true);
  res->setToolTip(tr("Edit Plot Preferences"));

  connect(res, &QToolButton::clicked, m_Plot, &QxrdImagePlot::editPreferences);

  return res;
}
