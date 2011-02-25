#include "qxrdmaskdialog.h"
#include "ui_qxrdmaskdialog.h"

QxrdMaskDialog::QxrdMaskDialog(QxrdWindowPtr win, QxrdDataProcessorPtr proc, QWidget *parent) :
  QDockWidget(parent),
  m_Window(win),
  m_Processor(proc),
  m_Masks(NULL)
{
  setupUi(this);

  connect(this, SIGNAL(printMessage(QDateTime,QString)), win, SLOT(printMessage(QDateTime,QString)));
  connect(this, SIGNAL(statusMessage(QDateTime,QString)), win, SLOT(statusMessage(QDateTime,QString)));
  connect(this, SIGNAL(criticalMessage(QDateTime,QString)), win, SLOT(criticalMessage(QDateTime,QString)));

  connect(m_HideMaskAll, SIGNAL(clicked()), this, SLOT(doHideMaskAll()));
  connect(m_ShowMaskAll, SIGNAL(clicked()), this, SLOT(doShowMaskAll()));
  connect(m_InvertMask, SIGNAL(clicked()), this, SLOT(doInvertMask()));
  connect(m_AndMask, SIGNAL(clicked()), this, SLOT(doAndMask()));
  connect(m_OrMask, SIGNAL(clicked()), this, SLOT(doOrMask()));
  connect(m_XorMask, SIGNAL(clicked()), this, SLOT(doXorMask()));
  connect(m_AndNotMask, SIGNAL(clicked()), this, SLOT(doAndNotMask()));
  connect(m_OrNotMask, SIGNAL(clicked()), this, SLOT(doOrNotMask()));
  connect(m_XorNotMask, SIGNAL(clicked()), this, SLOT(doXorNotMask()));
  connect(m_ExchangeMask, SIGNAL(clicked()), this, SLOT(doExchangeMask()));
  connect(m_RollUpMask, SIGNAL(clicked()), this, SLOT(doRollUpMask()));
  connect(m_RollDownMask, SIGNAL(clicked()), this, SLOT(doRollDownMask()));
  connect(m_RollMask, SIGNAL(clicked()), this, SLOT(doRollMask()));
  connect(m_NewMask, SIGNAL(clicked()), this, SLOT(doNewMask()));
  connect(m_PushMask, SIGNAL(clicked()), this, SLOT(doPushMask()));
  connect(m_ClearMask, SIGNAL(clicked()), this, SLOT(doClearMask()));
  connect(m_ClearMaskTop, SIGNAL(clicked()), this, SLOT(doClearMaskTop()));
  connect(m_UndoMask, SIGNAL(clicked()), this, SLOT(doUndoMask()));

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
  QDockWidget::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}
int QxrdMaskDialog::maskStackSelectPopup()
{
  QMenu actions;

  QxrdMaskStackPtr m = m_Processor->maskStack();

  if (m) {
    int nmasks = m->size();

    for (int i=0; i<nmasks; i++) {
      QxrdMaskDataPtr p = m->at(i);
      if (p) {
        QString lvl = m->stackLevelName(i);
        actions.addAction(tr("%1: %2").arg(lvl).arg(p->get_Title()))->setData(i);
      }
    }
  }

  QAction *selected = actions.exec(QCursor::pos());

  if (selected) {
    return selected->data().toInt();
  } else {
    return -1;
  }
}

void QxrdMaskDialog::doHideMaskAll()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    m_Processor->hideMaskAllStack(n);
    emit statusMessage(QDateTime::currentDateTime(), "Mask Stack Hide All");
  }
}

void QxrdMaskDialog::doShowMaskAll()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    m_Processor->showMaskAllStack(n);
    emit statusMessage(QDateTime::currentDateTime(), "Mask Stack Show All");
  }
}

void QxrdMaskDialog::doHideMaskRange()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    m_Processor->hideMaskRangeStack(n);
    emit statusMessage(QDateTime::currentDateTime(), "Mask Stack Hide All");
  }
}

void QxrdMaskDialog::doShowMaskRange()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    m_Processor->showMaskRangeStack(n);
    emit statusMessage(QDateTime::currentDateTime(), "Mask Stack Show All");
  }
}

void QxrdMaskDialog::doInvertMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    m_Processor->invertMaskStack(n);
    emit statusMessage(QDateTime::currentDateTime(), "Mask Stack Invert");
  }
}

void QxrdMaskDialog::doAndMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    m_Processor->andMaskStack(n);
    emit statusMessage(QDateTime::currentDateTime(), "Mask Stack AND");
  }
}

void QxrdMaskDialog::doOrMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    m_Processor->orMaskStack(n);
    emit statusMessage(QDateTime::currentDateTime(), "Mask Stack OR");
  }
}

void QxrdMaskDialog::doXorMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    m_Processor->xorMaskStack(n);
    emit statusMessage(QDateTime::currentDateTime(), "Mask Stack XOR");
  }
}

void QxrdMaskDialog::doAndNotMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    m_Processor->andNotMaskStack(n);
    emit statusMessage(QDateTime::currentDateTime(), "Mask Stack AND NOT");
  }
}

void QxrdMaskDialog::doOrNotMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    m_Processor->orNotMaskStack(n);
    emit statusMessage(QDateTime::currentDateTime(), "Mask Stack OR NOT");
  }
}

void QxrdMaskDialog::doXorNotMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    m_Processor->xorNotMaskStack(n);
    emit statusMessage(QDateTime::currentDateTime(), "Mask Stack XOR NOT");
  }
}

void QxrdMaskDialog::doExchangeMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    m_Processor->exchangeMaskStack(n);
    emit statusMessage(QDateTime::currentDateTime(), "Mask Stack Exchanged");
  }
}

void QxrdMaskDialog::doRollMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    m_Processor->rollMaskStack(n);
  }
}

void QxrdMaskDialog::doRollUpMask()
{
  m_Processor->rollMaskStack(1);
  emit statusMessage(QDateTime::currentDateTime(), "Mask Stack Rolled Up");
}

void QxrdMaskDialog::doRollDownMask()
{
  m_Processor->rollMaskStack(-1);
  emit statusMessage(QDateTime::currentDateTime(), "Mask Stack Rolled Down");
}

void QxrdMaskDialog::doClearMask()
{
  m_Processor->clearMaskStack();
  emit statusMessage(QDateTime::currentDateTime(), "Mask Stack Cleared");
}

void QxrdMaskDialog::doClearMaskTop()
{
  m_Processor->clearMaskStackTop();
  emit statusMessage(QDateTime::currentDateTime(), "Top of Mask Stack Cleared");
}

void QxrdMaskDialog::doPushMask()
{
  m_Processor->pushMaskStack();
  emit statusMessage(QDateTime::currentDateTime(), "Mask Pushed");
}

void QxrdMaskDialog::doNewMask()
{
  m_Processor->newMaskStack();
  emit statusMessage(QDateTime::currentDateTime(), "New Mask");
}

void QxrdMaskDialog::doUndoMask()
{
  emit statusMessage(QDateTime::currentDateTime(), "Undo Not Implemented");
}
