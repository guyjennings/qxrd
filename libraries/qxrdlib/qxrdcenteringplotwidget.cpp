#include "qxrdcenteringplotwidget.h"
#include "ui_qxrdcenteringplotwidget.h"

QxrdCenteringPlotWidget::QxrdCenteringPlotWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::QxrdCenteringPlotWidget)
{
  ui->setupUi(this);
}

QxrdCenteringPlotWidget::~QxrdCenteringPlotWidget()
{
  delete ui;
}

void QxrdCenteringPlotWidget::changeEvent(QEvent *e)
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
