#include "qxrdintegratedplotwidget.h"
#include "ui_qxrdintegratedplotwidget.h"

QxrdIntegratedPlotWidget::QxrdIntegratedPlotWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::QxrdIntegratedPlotWidget)
{
  ui->setupUi(this);
}

QxrdIntegratedPlotWidget::~QxrdIntegratedPlotWidget()
{
  delete ui;
}

void QxrdIntegratedPlotWidget::changeEvent(QEvent *e)
{
  QWidget::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    ui->retranslateUi(this);
    break;
  default:
    break;
  }
}
