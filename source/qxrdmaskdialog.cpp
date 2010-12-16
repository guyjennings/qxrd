#include "qxrdmaskdialog.h"
#include "ui_qxrdmaskdialog.h"

QxrdMaskDialog::QxrdMaskDialog(QxrdWindowPtr win, QxrdDataProcessorPtr proc, QWidget *parent) :
  QWidget(parent),
  ui(new Ui::QxrdMaskDialog),
  m_Window(win),
  m_Masks(NULL),
  m_Processor(proc)
{
  ui->setupUi(this);

  connect(ui->m_HideMaskRange, SIGNAL(clicked()), win, SLOT(doHideMaskRange()));
  connect(ui->m_ShowMaskRange, SIGNAL(clicked()), win, SLOT(doShowMaskRange()));

  connect(ui->m_HideMaskAll, SIGNAL(clicked()), win, SLOT(doHideMaskAll()));
  connect(ui->m_ShowMaskAll, SIGNAL(clicked()), win, SLOT(doShowMaskAll()));
  connect(ui->m_InvertMask, SIGNAL(clicked()), win, SLOT(doInvertMask()));
  connect(ui->m_AndMask, SIGNAL(clicked()), win, SLOT(doAndMask()));
  connect(ui->m_OrMask, SIGNAL(clicked()), win, SLOT(doOrMask()));
  connect(ui->m_XorMask, SIGNAL(clicked()), win, SLOT(doXorMask()));
  connect(ui->m_AndNotMask, SIGNAL(clicked()), win, SLOT(doAndNotMask()));
  connect(ui->m_OrNotMask, SIGNAL(clicked()), win, SLOT(doOrNotMask()));
  connect(ui->m_XorNotMask, SIGNAL(clicked()), win, SLOT(doXorNotMask()));
  connect(ui->m_ExchangeMask, SIGNAL(clicked()), win, SLOT(doExchangeMask()));
  connect(ui->m_RollUpMask, SIGNAL(clicked()), win, SLOT(doRollUpMask()));
  connect(ui->m_RollDownMask, SIGNAL(clicked()), win, SLOT(doRollDownMask()));
  connect(ui->m_NewMask, SIGNAL(clicked()), win, SLOT(doNewMask()));
  connect(ui->m_PushMask, SIGNAL(clicked()), win, SLOT(doPushMask()));
  connect(ui->m_ClearMask, SIGNAL(clicked()), win, SLOT(doClearMask()));
  connect(ui->m_ClearMaskTop, SIGNAL(clicked()), win, SLOT(doClearMaskTop()));
  connect(ui->m_UndoMask, SIGNAL(clicked()), win, SLOT(doUndoMask()));

  connect(ui->m_MaskCirclesRadio, SIGNAL(clicked()), win->m_ImageMaskCirclesButton, SLOT(click()));
  connect(ui->m_MaskPolygonsRadio, SIGNAL(clicked()), win->m_ImageMaskPolygonsButton, SLOT(click()));

  m_Processor -> prop_MaskMinimumValue() -> linkTo(ui->m_MaskMinimum);
  m_Processor -> prop_MaskMaximumValue() -> linkTo(ui->m_MaskMaximum);
  m_Processor -> prop_MaskCircleRadius() -> linkTo(ui->m_MaskCircleRadius);
  m_Processor -> prop_MaskSetPixels() -> linkTo(ui->m_MaskSetPixels);

  m_Masks = m_Processor->maskStack();
  m_MaskStackModel = new QxrdMaskStackModel(m_Masks);

  ui->m_MaskStackView -> setModel(m_MaskStackModel);
  ui->m_MaskStackView -> setMaskStack(m_Masks);
  ui->m_MaskStackView -> setMaskDialog(this);
}

QxrdMaskDialog::~QxrdMaskDialog()
{
  delete ui;
}

void QxrdMaskDialog::changeEvent(QEvent *e)
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

void QxrdMaskDialog::newMask()
{

}

void QxrdMaskDialog::enableMasks()
{

}

void QxrdMaskDialog::deleteMasks()
{

}

void QxrdMaskDialog::andMasks()
{

}

void QxrdMaskDialog::orMasks()
{

}

void QxrdMaskDialog::thresholdMasks()
{

}
