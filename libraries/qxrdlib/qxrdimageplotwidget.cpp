#include "qxrdimageplotwidget.h"
#include "ui_qxrdimageplotwidget.h"

QxrdImagePlotWidget::QxrdImagePlotWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::QxrdImagePlotWidget)
{
  ui->setupUi(this);
}

QxrdImagePlotWidget::~QxrdImagePlotWidget()
{
  delete ui;
}

void QxrdImagePlotWidget::changeEvent(QEvent *e)
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
