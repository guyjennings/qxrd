#include "qxrdplotwidget.h"
#include "qxrdplotcommand.h"
#include <QLayout>
#include <QToolButton>
#include <QSpacerItem>

QxrdPlotWidget::QxrdPlotWidget(QWidget *parent) :
  QWidget(parent)
{
  setupUi(this);

  QGridLayout *layout = new QGridLayout();

  layout -> setContentsMargins(0,0,0,0);
  layout -> setSpacing(2);

  m_PlotTools->setLayout(layout);
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
