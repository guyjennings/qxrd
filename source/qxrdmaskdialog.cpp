#include "qxrdmaskdialog.h"
#include "ui_qxrdmaskdialog.h"

QxrdMaskDialog::QxrdMaskDialog(QxrdWindowPtr win, QxrdDataProcessorPtr proc, QWidget *parent) :
  QDockWidget(parent),
  m_Window(win),
  m_Processor(proc),
  m_Masks(NULL)
{
  setupUi(this);

  connect(m_HideMaskRange, SIGNAL(clicked()), win, SLOT(doHideMaskRange()));
  connect(m_ShowMaskRange, SIGNAL(clicked()), win, SLOT(doShowMaskRange()));

  connect(m_HideMaskAll, SIGNAL(clicked()), win, SLOT(doHideMaskAll()));
  connect(m_ShowMaskAll, SIGNAL(clicked()), win, SLOT(doShowMaskAll()));
  connect(m_InvertMask, SIGNAL(clicked()), win, SLOT(doInvertMask()));
  connect(m_AndMask, SIGNAL(clicked()), win, SLOT(doAndMask()));
  connect(m_OrMask, SIGNAL(clicked()), win, SLOT(doOrMask()));
  connect(m_XorMask, SIGNAL(clicked()), win, SLOT(doXorMask()));
  connect(m_AndNotMask, SIGNAL(clicked()), win, SLOT(doAndNotMask()));
  connect(m_OrNotMask, SIGNAL(clicked()), win, SLOT(doOrNotMask()));
  connect(m_XorNotMask, SIGNAL(clicked()), win, SLOT(doXorNotMask()));
  connect(m_ExchangeMask, SIGNAL(clicked()), win, SLOT(doExchangeMask()));
  connect(m_RollUpMask, SIGNAL(clicked()), win, SLOT(doRollUpMask()));
  connect(m_RollDownMask, SIGNAL(clicked()), win, SLOT(doRollDownMask()));
  connect(m_NewMask, SIGNAL(clicked()), win, SLOT(doNewMask()));
  connect(m_PushMask, SIGNAL(clicked()), win, SLOT(doPushMask()));
  connect(m_ClearMask, SIGNAL(clicked()), win, SLOT(doClearMask()));
  connect(m_ClearMaskTop, SIGNAL(clicked()), win, SLOT(doClearMaskTop()));
  connect(m_UndoMask, SIGNAL(clicked()), win, SLOT(doUndoMask()));

  connect(m_MaskCirclesRadio, SIGNAL(clicked()), win->m_ImageMaskCirclesButton, SLOT(click()));
  connect(m_MaskPolygonsRadio, SIGNAL(clicked()), win->m_ImageMaskPolygonsButton, SLOT(click()));

  m_Processor -> prop_MaskMinimumValue() -> linkTo(m_MaskMinimum);
  m_Processor -> prop_MaskMaximumValue() -> linkTo(m_MaskMaximum);
  m_Processor -> prop_MaskCircleRadius() -> linkTo(m_MaskCircleRadius);
  m_Processor -> prop_MaskSetPixels() -> linkTo(m_MaskSetPixels);

  m_Masks = m_Processor->maskStack();
  m_MaskStackModel = new QxrdMaskStackModel(m_Masks);

  m_MaskStackView -> setModel(m_MaskStackModel);
  m_MaskStackView -> setMaskStack(m_Masks);
  m_MaskStackView -> setProcessor(m_Processor);
  m_MaskStackView -> setMaskDialog(this);
}

QxrdMaskDialog::~QxrdMaskDialog()
{
}

void QxrdMaskDialog::changeEvent(QEvent *e)
{
  QWidget::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
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
