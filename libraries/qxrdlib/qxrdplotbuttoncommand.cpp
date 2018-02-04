#include "qxrdplotbuttoncommand.h"
#include <QToolButton>
#include <QMenu>
#include <qwt_plot_picker.h>

QxrdPlotButtonCommand::QxrdPlotButtonCommand(QString                    name,
                                             QxrdPlotWidget            *plot,
                                             QxrdPlotWidgetSettingsWPtr set,
                                             QString                    iconPath,
                                             QString                    toolTip,
                                             bool                       checkable)
  : inherited(name, plot, set),
    m_ToolButton(NULL),
    m_PlotPicker(NULL),
    m_IconPaths(),
    m_ToolTips(),
    m_Index(0)
{
  if (!iconPath.isEmpty()) {
    m_IconPaths.append(iconPath);
    m_ToolTips.append(toolTip);
  }

  m_ToolButton =  new QToolButton();

  m_ToolButton->setIcon(commandIcon(m_Index));
  m_ToolButton->setIconSize(QSize(24,24));
  m_ToolButton->setToolTip(toolTip);

  if (checkable) {
    m_ToolButton->setCheckable(checkable);
    m_ToolButton->setAutoExclusive(checkable);
  }

  connect(m_ToolButton,  &QWidget::customContextMenuRequested,
          this,          &QxrdPlotButtonCommand::contextMenu);

  connect(m_ToolButton,  &QToolButton::toggled,
          this,          &QxrdPlotButtonCommand::toggled);

  connect(m_ToolButton,  &QToolButton::clicked,
          this,          &QxrdPlotButtonCommand::clicked);
}

void QxrdPlotButtonCommand::initialize(QObjectWPtr parent)
{
  inherited::initialize(parent);

  m_ToolButton -> setObjectName(get_Name());
}

void QxrdPlotButtonCommand::appendMode(QString iconPath, QString toolTip)
{
  m_IconPaths.append(iconPath);
  m_ToolTips.append(toolTip);

  m_ToolButton -> setContextMenuPolicy(Qt::CustomContextMenu);
}

void QxrdPlotButtonCommand::setPlotPicker(QwtPlotPicker *pick)
{
  bool en = false;

  if (m_PlotPicker) {
    en = m_PlotPicker -> isEnabled();
    m_PlotPicker -> setEnabled(false);
  }

  m_PlotPicker = pick;
  m_PlotPicker -> setEnabled(en);
}

void QxrdPlotButtonCommand::toggled(bool on)
{
  if (on) {
    enable();
  } else {
    disable();
  }
}

void QxrdPlotButtonCommand::enable()
{
  inherited::enable();

  if (m_PlotPicker) {
    m_PlotPicker->setEnabled(true);
  }
}

void QxrdPlotButtonCommand::disable()
{
  inherited::disable();

  if (m_PlotPicker) {
    m_PlotPicker->setEnabled(false);
  }
}

void QxrdPlotButtonCommand::setPen(const QPen &pen)
{
  inherited::setPen(pen);

  if (m_PlotPicker) {
    m_PlotPicker -> setTrackerPen(pen);
    m_PlotPicker -> setRubberBandPen(pen);
  }
}

QAction* QxrdPlotButtonCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return NULL;
}

void QxrdPlotButtonCommand::contextMenu(const QPoint &pos)
{
  QMenu menu(NULL, NULL);

  for (int i=0; i<m_ToolTips.count(); i++) {
    QAction *act = menu.addAction(m_ToolTips.value(i), [=]() {changeMode(i);});
    act->setIcon(commandIcon(i));

    if (i == m_Index) {
      act->setCheckable(true);
      act->setChecked(true);
    }
  }

  menu.exec(m_ToolButton->mapToGlobal(pos));
}

void QxrdPlotButtonCommand::clicked(bool checked)
{
  if (checked && (m_ToolTips.count() > 1)) {
    QMenu menu(NULL, NULL);

    for (int i=0; i<m_ToolTips.count(); i++) {
      QAction *act = menu.addAction(m_ToolTips.value(i), [=]() {changeMode(i);});
      act->setIcon(commandIcon(i));

      if (i == m_Index) {
        act->setCheckable(true);
        act->setChecked(true);
      }
    }

    menu.exec(QCursor::pos());
  }
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

QIcon QxrdPlotButtonCommand::commandIcon(int i)
{
  QIcon icon;
  icon.addFile(m_IconPaths.value(i), QSize(), QIcon::Normal, QIcon::Off);

  return icon;
}
