#include "qxrdplotwidget.h"
#include "qxrdplotcommand.h"
#include <QLayout>
#include <QToolButton>
#include <QSpacerItem>
#include <QMenu>
#include "qxrdautoscalecommand.h"
#include "qxrdprintplotcommand.h"

QxrdPlotWidget::QxrdPlotWidget(QWidget *parent) :
  QWidget(parent)
{
  setupUi(this);

//  QLayout     *oldLayout = layout();

  QGridLayout *layout = new QGridLayout(m_PlotTools);

  layout -> setContentsMargins(0,0,0,0);
  layout -> setSpacing(2);

  m_PlotTools->setLayout(layout);

  m_Plot->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(m_Plot, &QWidget::customContextMenuRequested,
          this,   &QxrdPlotWidget::contextMenu);

  addPlotCommand(QxrdPlotCommandPtr(new QxrdAutoScaleCommand("Auto Scale", this)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdPrintPlotCommand("Print Graph...", this)));
}

QxrdPlotWidget::~QxrdPlotWidget()
{
}

void QxrdPlotWidget::addPlotCommand(QxrdPlotCommandPtr cmd)
{
  m_PlotCommands.append(cmd);

  if (cmd) {
    QToolButton *button = cmd->toolButton();

    if (button) {
      QLayout *layout = m_PlotTools->layout();

      if (layout) {
        layout->addWidget(button);
      }
    }
  }
}

void QxrdPlotWidget::addPlotCommandSpacer()
{
  QSpacerItem *spacer =
      new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

  QLayout *layout = m_PlotTools->layout();

  if (layout) {
    layout->addItem(spacer);
  }
}

void QxrdPlotWidget::contextMenu(const QPoint &pos)
{
  QMenu plotMenu(NULL, NULL);

  foreach (QxrdPlotCommandPtr cmd, m_PlotCommands) {
    if (cmd) {
      QAction* act = cmd->contextMenuAction(pos);

      if (act) {
        plotMenu.addAction(act);
//      } else if (cmd->contextMenuSeparator()) {
//        plotMenu.addSeparator();
      }
    }
  }

  plotMenu.exec(mapToGlobal(pos));
}

void QxrdPlotWidget::disableCommands()
{
  foreach (QxrdPlotCommandPtr cmd, m_PlotCommands) {
    if (cmd) {
      cmd->disable();
    }
  }
}
