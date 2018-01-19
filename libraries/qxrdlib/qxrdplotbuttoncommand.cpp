#include "qxrdplotbuttoncommand.h"
#include <QToolButton>
#include <QMenu>

QxrdPlotButtonCommand::QxrdPlotButtonCommand(QString                    name,
                                             QxrdPlotWidget            *plot,
                                             QxrdPlotWidgetSettingsWPtr set,
                                             QString                    iconPath,
                                             QString                    toolTip,
                                             bool                       checkable)
  : QxrdPlotCommand(name, plot, set),
    m_IconPaths(iconPath),
    m_ToolTips(toolTip),
    m_Index(0)
{
  m_ToolButton =  new QToolButton();
  m_ToolButton -> setObjectName(get_Name());

  QIcon icon;
  icon.addFile(iconPath, QSize(), QIcon::Normal, QIcon::Off);
  m_ToolButton->setIcon(icon);

  m_ToolButton->setIconSize(QSize(24,24));
  m_ToolButton->setToolTip(toolTip);

  if (checkable) {
    m_ToolButton->setCheckable(checkable);
    m_ToolButton->setAutoExclusive(checkable);
  }

  connect(m_ToolButton,  &QWidget::customContextMenuRequested,
          this,          &QxrdPlotButtonCommand::contextMenu);
}

void QxrdPlotButtonCommand::appendMode(QString iconPath, QString toolTip)
{
  m_IconPaths.append(iconPath);
  m_ToolTips.append(toolTip);

  m_ToolButton -> setContextMenuPolicy(Qt::CustomContextMenu);
}

QAction* QxrdPlotButtonCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return NULL;
}

void QxrdPlotButtonCommand::contextMenu(const QPoint &pos)
{
  QMenu menu(NULL, NULL);

  for (int i=0; i<m_ToolTips.count(); i++) {
    menu.addAction(m_ToolTips.value(i), [=]() {changeMode(i);});
  }

  menu.exec(m_ToolButton->mapToGlobal(pos));
}

void QxrdPlotButtonCommand::changeMode(int i)
{
  m_Index = i;

  QIcon icon;
  icon.addFile(m_IconPaths.value(i), QSize(), QIcon::Normal, QIcon::Off);
  m_ToolButton->setIcon(icon);

  m_ToolButton->setToolTip(m_ToolTips.value(i));
}

QToolButton* QxrdPlotButtonCommand::toolButton()
{
  return m_ToolButton;
}
