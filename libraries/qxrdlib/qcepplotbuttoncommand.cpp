#include "qcepplotbuttoncommand.h"
#include <QToolButton>
#include <QMenu>
#include <qwt_plot_picker.h>

QcepPlotButtonCommand::QcepPlotButtonCommand(QString                    name,
                                             QString                    desc,
                                             QcepPlotWidget            *plot,
                                             QcepPlotWidgetSettingsWPtr set,
                                             QString                    iconPath,
                                             bool                       checkable)
  : inherited(name, desc, plot, set),
    m_ToolButton(NULL),
    m_PlotPicker(NULL),
    m_IconPaths(),
    m_ToolTips(),
    m_Index(0)
{
  if (!iconPath.isEmpty()) {
    m_IconPaths.append(iconPath);
    m_ToolTips.append(desc);
  }

  m_ToolButton =  new QToolButton();

  m_ToolButton->setIcon(commandIcon(m_Index));
  m_ToolButton->setIconSize(QSize(24,24));
  m_ToolButton->setToolTip(desc);

  if (checkable) {
    m_ToolButton->setCheckable(checkable);
    m_ToolButton->setAutoExclusive(checkable);
  }

  connect(m_ToolButton,  &QWidget::customContextMenuRequested,
          this,          &QcepPlotButtonCommand::contextMenu);

  connect(m_ToolButton,  &QToolButton::toggled,
          this,          &QcepPlotButtonCommand::toggled);

  connect(m_ToolButton,  &QToolButton::clicked,
          this,          &QcepPlotButtonCommand::clicked);
}

void QcepPlotButtonCommand::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  m_ToolButton -> setObjectName(get_Name());
}

void QcepPlotButtonCommand::appendMode(QString iconPath, QString desc)
{
  m_IconPaths.append(iconPath);
  m_ToolTips.append(desc);

  m_ToolButton -> setContextMenuPolicy(Qt::CustomContextMenu);
}

void QcepPlotButtonCommand::setPlotPicker(QwtPlotPicker *pick)
{
  bool en = false;

  if (m_PlotPicker) {
    en = m_PlotPicker -> isEnabled();
    m_PlotPicker -> setEnabled(false);
  }

  m_PlotPicker = pick;
  m_PlotPicker -> setEnabled(en);
}

void QcepPlotButtonCommand::toggled(bool on)
{
  if (on) {
    enable();
  } else {
    disable();
  }
}

void QcepPlotButtonCommand::enable()
{
  inherited::enable();

  if (m_PlotPicker) {
    m_PlotPicker->setEnabled(true);
  }
}

void QcepPlotButtonCommand::disable()
{
  inherited::disable();

  if (m_PlotPicker) {
    m_PlotPicker->setEnabled(false);
  }
}

void QcepPlotButtonCommand::setPen(const QPen &pen)
{
  inherited::setPen(pen);

  if (m_PlotPicker) {
    m_PlotPicker -> setTrackerPen(pen);
    m_PlotPicker -> setRubberBandPen(pen);
  }
}

QAction* QcepPlotButtonCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return NULL;
}

void QcepPlotButtonCommand::contextMenu(const QPoint &pos)
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

void QcepPlotButtonCommand::clicked(bool checked)
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

void QcepPlotButtonCommand::changeMode(int i)
{
  m_Index = i;

  QIcon icon;
  icon.addFile(m_IconPaths.value(i), QSize(), QIcon::Normal, QIcon::Off);
  m_ToolButton->setIcon(icon);

  m_ToolButton->setToolTip(m_ToolTips.value(i));
}

QToolButton* QcepPlotButtonCommand::toolButton()
{
  return m_ToolButton;
}

QIcon QcepPlotButtonCommand::commandIcon(int i)
{
  QIcon icon;
  icon.addFile(m_IconPaths.value(i), QSize(), QIcon::Normal, QIcon::Off);

  return icon;
}
