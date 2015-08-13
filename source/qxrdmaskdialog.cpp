#include "qxrdmaskdialog.h"
#include "ui_qxrdmaskdialog.h"
#include "qxrdwindow.h"
#include "qxrddataprocessor.h"
#include "qxrdmaskstackmodel.h"
#include "qxrdapplication.h"
#include "qxrddebug.h"

QxrdMaskDialog::QxrdMaskDialog(QxrdDataProcessorWPtr procw, QWidget *parent) :
  QDockWidget(parent),
  m_Processor(procw),
  m_Masks(),
  m_MaskStackModel()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdMaskDialog::QxrdMaskDialog(%p)\n", this);
  }

  setupUi(this);

  connect(m_HideMaskAll, &QAbstractButton::clicked, this, &QxrdMaskDialog::doHideMaskAll);
  connect(m_ShowMaskAll, &QAbstractButton::clicked, this, &QxrdMaskDialog::doShowMaskAll);
  connect(m_HideMaskRange, &QAbstractButton::clicked, this, &QxrdMaskDialog::doHideMaskRange);
  connect(m_ShowMaskRange, &QAbstractButton::clicked, this, &QxrdMaskDialog::doShowMaskRange);
  connect(m_InvertMask, &QAbstractButton::clicked, this, &QxrdMaskDialog::doInvertMask);
  connect(m_GrowMask, &QAbstractButton::clicked, this, &QxrdMaskDialog::doGrowMask);
  connect(m_ShrinkMask, &QAbstractButton::clicked, this, &QxrdMaskDialog::doShrinkMask);
  connect(m_AndMask, &QAbstractButton::clicked, this, &QxrdMaskDialog::doAndMask);
  connect(m_OrMask, &QAbstractButton::clicked, this, &QxrdMaskDialog::doOrMask);
  connect(m_XorMask, &QAbstractButton::clicked, this, &QxrdMaskDialog::doXorMask);
  connect(m_AndNotMask, &QAbstractButton::clicked, this, &QxrdMaskDialog::doAndNotMask);
  connect(m_OrNotMask, &QAbstractButton::clicked, this, &QxrdMaskDialog::doOrNotMask);
  connect(m_XorNotMask, &QAbstractButton::clicked, this, &QxrdMaskDialog::doXorNotMask);
  connect(m_ExchangeMask, &QAbstractButton::clicked, this, &QxrdMaskDialog::doExchangeMask);
  connect(m_RollUpMask, &QAbstractButton::clicked, this, &QxrdMaskDialog::doRollUpMask);
  connect(m_RollDownMask, &QAbstractButton::clicked, this, &QxrdMaskDialog::doRollDownMask);
  connect(m_RollMask, &QAbstractButton::clicked, this, &QxrdMaskDialog::doRollMask);
  connect(m_NewMask, &QAbstractButton::clicked, this, &QxrdMaskDialog::doNewMask);
  connect(m_PushMask, &QAbstractButton::clicked, this, &QxrdMaskDialog::doPushMask);
  connect(m_ClearMask, &QAbstractButton::clicked, this, &QxrdMaskDialog::doClearMask);
  connect(m_ClearMaskTop, &QAbstractButton::clicked, this, &QxrdMaskDialog::doClearMaskTop);
  connect(m_UndoMask, &QAbstractButton::clicked, this, &QxrdMaskDialog::doUndoMask);

  QxrdDataProcessorPtr proc(m_Processor);

  if (proc) {
    proc -> prop_MaskMinimumValue() -> linkTo(m_MaskMinimum);
    proc -> prop_MaskMaximumValue() -> linkTo(m_MaskMaximum);
    proc -> prop_MaskCircleRadius() -> linkTo(m_MaskCircleRadius);
    proc -> prop_MaskSetPixels() -> linkTo(m_MaskSetPixels);

    m_Masks = proc->maskStack();
    m_MaskStackModel = QxrdMaskStackModelPtr(
          new QxrdMaskStackModel(m_Masks));

    m_MaskStackView -> setModel(m_MaskStackModel.data());
    m_MaskStackView -> setMaskStack(m_Masks);
    m_MaskStackView -> setProcessor(m_Processor);
    m_MaskStackView -> setMaskDialog(this);
  }
}

QxrdMaskDialog::~QxrdMaskDialog()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdMaskDialog::~QxrdMaskDialog(%p)\n", this);
  }
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

  QxrdDataProcessorPtr proc(m_Processor);

  if (proc) {
    QxrdMaskStackPtr m = proc->maskStack();

    if (m) {
      int nmasks = m->size();

      for (int i=0; i<nmasks; i++) {
        QcepMaskDataPtr p = m->at(i);
        if (p) {
          QString lvl = m->stackLevelName(i);
          actions.addAction(tr("%1: %2").arg(lvl).arg(p->get_Title()))->setData(i);
        }
      }
    }

    QAction *selected = actions.exec(QCursor::pos());

    if (selected) {
      return selected->data().toInt();
    }
  }

  return -1;
}

void QxrdMaskDialog::doHideMaskAll()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdDataProcessorPtr proc(m_Processor);

    if (proc) {
      proc->hideMaskAllStack(n);
      proc->statusMessage("Mask Stack Hide All");
    }
  }
}

void QxrdMaskDialog::doShowMaskAll()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdDataProcessorPtr proc(m_Processor);

    if (proc) {
      proc->showMaskAllStack(n);
      proc->statusMessage("Mask Stack Show All");
    }
  }
}

void QxrdMaskDialog::doHideMaskRange()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdDataProcessorPtr proc(m_Processor);

    if (proc) {
      proc->hideMaskRangeStack(n);
      proc->statusMessage("Mask Stack Hide In Range");
    }
  }
}

void QxrdMaskDialog::doShowMaskRange()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdDataProcessorPtr proc(m_Processor);

    if (proc) {
      proc->showMaskRangeStack(n);
      proc->statusMessage("Mask Stack Show In Range");
    }
  }
}

void QxrdMaskDialog::doInvertMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdDataProcessorPtr proc(m_Processor);

    if (proc) {
      proc->invertMaskStack(n);
      proc->statusMessage("Mask Stack Invert");
    }
  }
}

void QxrdMaskDialog::doGrowMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdDataProcessorPtr proc(m_Processor);

    if (proc) {
      proc->growMaskStack(n);
      proc->statusMessage("Mask Stack Invert");
    }
  }
}

void QxrdMaskDialog::doShrinkMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdDataProcessorPtr proc(m_Processor);

    if (proc) {
      proc->shrinkMaskStack(n);
      proc->statusMessage("Mask Stack Invert");
    }
  }
}

void QxrdMaskDialog::doAndMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdDataProcessorPtr proc(m_Processor);

    if (proc) {
      proc->andMaskStack(n);
      proc->statusMessage("Mask Stack AND");
    }
  }
}

void QxrdMaskDialog::doOrMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdDataProcessorPtr proc(m_Processor);

    if (proc) {
      proc->orMaskStack(n);
      proc->statusMessage("Mask Stack OR");
    }
  }
}

void QxrdMaskDialog::doXorMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdDataProcessorPtr proc(m_Processor);

    if (proc) {
      proc->xorMaskStack(n);
      proc->statusMessage("Mask Stack XOR");
    }
  }
}

void QxrdMaskDialog::doAndNotMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdDataProcessorPtr proc(m_Processor);

    if (proc) {
      proc->andNotMaskStack(n);
      proc->statusMessage("Mask Stack AND NOT");
    }
  }
}

void QxrdMaskDialog::doOrNotMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdDataProcessorPtr proc(m_Processor);

    if (proc) {
      proc->orNotMaskStack(n);
      proc->statusMessage("Mask Stack OR NOT");
    }
  }
}

void QxrdMaskDialog::doXorNotMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdDataProcessorPtr proc(m_Processor);

    if (proc) {
      proc->xorNotMaskStack(n);
      proc->statusMessage("Mask Stack XOR NOT");
    }
  }
}

void QxrdMaskDialog::doExchangeMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdDataProcessorPtr proc(m_Processor);

    if (proc) {
      proc->exchangeMaskStack(n);
      proc->statusMessage("Mask Stack Exchanged");
    }
  }
}

void QxrdMaskDialog::doRollMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdDataProcessorPtr proc(m_Processor);

    if (proc) {
      proc->rollMaskStack(n);
    }
  }
}

void QxrdMaskDialog::doRollUpMask()
{
  QxrdDataProcessorPtr proc(m_Processor);

  if (proc) {
    proc->rollMaskStack(1);
    proc->statusMessage("Mask Stack Rolled Up");
  }
}

void QxrdMaskDialog::doRollDownMask()
{
  QxrdDataProcessorPtr proc(m_Processor);

  if (proc) {
    proc->rollMaskStack(-1);
    proc->statusMessage("Mask Stack Rolled Down");
  }
}

void QxrdMaskDialog::doClearMask()
{
  QxrdDataProcessorPtr proc(m_Processor);

  if (proc) {
    proc->clearMaskStack();
    proc->statusMessage("Mask Stack Cleared");
  }
}

void QxrdMaskDialog::doClearMaskTop()
{
  QxrdDataProcessorPtr proc(m_Processor);

  if (proc) {
    proc->clearMaskStackTop();
    proc->statusMessage("Top of Mask Stack Cleared");
  }
}

void QxrdMaskDialog::doPushMask()
{
  QxrdDataProcessorPtr proc(m_Processor);

  if (proc) {
    proc->pushMaskStack();
    proc->statusMessage("Mask Pushed");
  }
}

void QxrdMaskDialog::doNewMask()
{
  QxrdDataProcessorPtr proc(m_Processor);

  if (proc) {
    proc->newMaskStack();
    proc->statusMessage("New Mask");
  }
}

void QxrdMaskDialog::doUndoMask()
{
  QxrdDataProcessorPtr proc(m_Processor);

  if (proc) {
    proc->statusMessage("Undo Not Implemented");
  }
}
