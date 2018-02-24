#include "qxrdmaskingwindow.h"
#include "qxrdexperiment.h"
#include "qxrdmaskingwindowsettings.h"
#include "qxrdprocessor.h"
#include "qxrdmaskstack.h"
#include "qxrdzingerdialog.h"
#include "qxrdprocessor.h"
#include <QThread>

QxrdMaskingWindow::QxrdMaskingWindow(QString name) :
  inherited(name)
{
}

void QxrdMaskingWindow::initialize(QcepObjectWPtr parent)
{
  GUI_THREAD_CHECK;

  inherited::initialize(parent);

  setupUi(this);

  setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);

  m_Splitter->setStretchFactor(0, 1);
  m_Splitter->setStretchFactor(1, 5);
  m_Splitter->setStretchFactor(2, 1);

  QxrdExperimentPtr exp(QxrdExperiment::findExperiment(m_Parent));

  QxrdProcessorPtr proc(QxrdProcessor::findProcessor(m_Parent));

  if (exp) {
    proc = exp->processor();

    m_DatasetBrowserView -> setExperiment(exp);

    QcepDatasetModelPtr model(exp->dataset());

    m_DatasetBrowserView -> setDatasetModel(model);

    QxrdMaskingWindowSettingsPtr settings(
          qSharedPointerDynamicCast<QxrdMaskingWindowSettings>(m_Parent));

    if (settings && proc) {
      m_FileBrowserWidget -> initialize(settings->fileBrowserSettings(), exp, proc);
      m_ImagePlotWidget   -> initialize(settings->imagePlotWidgetSettings(), proc);
    }
  }

  if (proc) {
    m_MaskStack = proc->maskStack();

    connect(proc.data(), &QxrdProcessor::dataAvailable,
            m_ImagePlotWidget, &QxrdImagePlotWidget::newImage);

    connect(proc.data(), &QxrdProcessor::maskAvailable,
            m_ImagePlotWidget, &QxrdImagePlotWidget::newMask);

    m_MaskStackView -> setMaskStack(m_MaskStack);
    m_MaskStackView -> setProcessor(proc);
  }

  connect(m_HideMaskAll, &QAbstractButton::clicked,
          this, &QxrdMaskingWindow::doHideMaskAll);

  connect(m_ShowMaskAll, &QAbstractButton::clicked,
          this, &QxrdMaskingWindow::doShowMaskAll);

  connect(m_HideMaskRange, &QAbstractButton::clicked,
          this, &QxrdMaskingWindow::doHideMaskRange);

  connect(m_ShowMaskRange, &QAbstractButton::clicked,
          this, &QxrdMaskingWindow::doShowMaskRange);

  connect(m_InvertMask, &QAbstractButton::clicked,
          this, &QxrdMaskingWindow::doInvertMask);

  connect(m_GrowMask, &QAbstractButton::clicked,
          this, &QxrdMaskingWindow::doGrowMask);

  connect(m_ShrinkMask, &QAbstractButton::clicked,
          this, &QxrdMaskingWindow::doShrinkMask);

  connect(m_AndMask, &QAbstractButton::clicked,
          this, &QxrdMaskingWindow::doAndMask);

  connect(m_OrMask, &QAbstractButton::clicked,
          this, &QxrdMaskingWindow::doOrMask);

  connect(m_XorMask, &QAbstractButton::clicked,
          this, &QxrdMaskingWindow::doXorMask);

  connect(m_AndNotMask, &QAbstractButton::clicked,
          this, &QxrdMaskingWindow::doAndNotMask);

  connect(m_OrNotMask, &QAbstractButton::clicked,
          this, &QxrdMaskingWindow::doOrNotMask);

  connect(m_XorNotMask, &QAbstractButton::clicked,
          this, &QxrdMaskingWindow::doXorNotMask);

  connect(m_ExchangeMask, &QAbstractButton::clicked,
          this, &QxrdMaskingWindow::doExchangeMask);

  connect(m_RollUpMask, &QAbstractButton::clicked,
          this, &QxrdMaskingWindow::doRollUpMask);

  connect(m_RollDownMask, &QAbstractButton::clicked,
          this, &QxrdMaskingWindow::doRollDownMask);

  connect(m_RollMask, &QAbstractButton::clicked,
          this, &QxrdMaskingWindow::doRollMask);

  connect(m_NewMask, &QAbstractButton::clicked,
          this, &QxrdMaskingWindow::doNewMask);

  connect(m_PushMask, &QAbstractButton::clicked,
          this, &QxrdMaskingWindow::doPushMask);

  connect(m_ClearMask, &QAbstractButton::clicked,
          this, &QxrdMaskingWindow::doClearMask);

  connect(m_ClearMaskTop, &QAbstractButton::clicked,
          this, &QxrdMaskingWindow::doClearMaskTop);

  connect(m_UndoMask, &QAbstractButton::clicked,
          this, &QxrdMaskingWindow::doUndoMask);

  connect(m_ZingerMask, &QAbstractButton::clicked,
          this, &QxrdMaskingWindow::doZingersMask);
}

QxrdMaskingWindow::~QxrdMaskingWindow()
{
}

void QxrdMaskingWindow::changeEvent(QEvent *e)
{
  inherited::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}

int QxrdMaskingWindow::maskStackSelectPopup()
{
  QMenu actions;

  QxrdProcessorPtr proc(QxrdProcessor::findProcessor(m_Parent));

  if (proc) {
    QxrdMaskStackPtr m = proc->maskStack();

    if (m) {
      int nmasks = m->maskCount();

      for (int i=0; i<nmasks; i++) {
        QcepMaskDataPtr p = m->mask(i);
        if (p) {
          QString lvl = m->stackLevelName(i);
          actions.addAction(tr("%1: %2").arg(lvl).arg(p->get_Name()))->setData(i);
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

void QxrdMaskingWindow::doHideMaskAll()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdMaskStackPtr m(m_MaskStack);

    if (m) {
      m->hideMaskAllStack(n);
      m->statusMessage("Mask Stack Hide All");
    }
  }
}

void QxrdMaskingWindow::doShowMaskAll()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdMaskStackPtr m(m_MaskStack);

    if (m) {
      m->showMaskAllStack(n);
      m->statusMessage("Mask Stack Show All");
    }
  }
}

void QxrdMaskingWindow::doHideMaskRange()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdProcessorPtr p(QxrdProcessor::findProcessor(m_Parent));
    QxrdMaskStackPtr m(m_MaskStack);

    if (m && p) {
      m->hideMaskRangeStack(n,
                            p->data(),
                            p->get_MaskMinimumValue(),
                            p->get_MaskMaximumValue());

      m->statusMessage("Mask Stack Hide In Range");
    }
  }
}

void QxrdMaskingWindow::doShowMaskRange()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdProcessorPtr p(QxrdProcessor::findProcessor(m_Parent));
    QxrdMaskStackPtr m(m_MaskStack);

    if (m && p) {
      m->showMaskRangeStack(n,
                            p->data(),
                            p->get_MaskMinimumValue(),
                            p->get_MaskMaximumValue());

      m->statusMessage("Mask Stack Show In Range");
    }
  }
}

void QxrdMaskingWindow::doZingersMask()
{
//  int n = maskStackSelectPopup();

//  if (n >= 0) {
//    QxrdProcessorPtr proc(m_Processor);

//    if (proc) {
//      proc->zingersStack(n);
//      proc->statusMessage("Find Zingers");
//    }
//  }

  QxrdProcessorPtr proc(QxrdProcessor::findProcessor(m_Parent));

  if (proc) {
    QxrdZingerFinderWPtr zf = proc->zingerFinder();

    QxrdZingerDialog dlg(proc, zf);

    dlg.exec();
  }
}

void QxrdMaskingWindow::doInvertMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdMaskStackPtr m(m_MaskStack);

    if (m) {
      m->invertMaskStack(n);
      m->statusMessage("Mask Stack Invert");
    }
  }
}

void QxrdMaskingWindow::doGrowMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdMaskStackPtr m(m_MaskStack);

    if (m) {
      m->growMaskStack(n);
      m->statusMessage("Mask Stack Invert");
    }
  }
}

void QxrdMaskingWindow::doShrinkMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdMaskStackPtr m(m_MaskStack);

    if (m) {
      m->shrinkMaskStack(n);
      m->statusMessage("Mask Stack Invert");
    }
  }
}

void QxrdMaskingWindow::doAndMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdMaskStackPtr m(m_MaskStack);

    if (m) {
      m->andMaskStack(n);
      m->statusMessage("Mask Stack AND");
    }
  }
}

void QxrdMaskingWindow::doOrMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdMaskStackPtr m(m_MaskStack);

    if (m) {
      m->orMaskStack(n);
      m->statusMessage("Mask Stack OR");
    }
  }
}

void QxrdMaskingWindow::doXorMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdMaskStackPtr m(m_MaskStack);

    if (m) {
      m->xorMaskStack(n);
      m->statusMessage("Mask Stack XOR");
    }
  }
}

void QxrdMaskingWindow::doAndNotMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdMaskStackPtr m(m_MaskStack);

    if (m) {
      m->andNotMaskStack(n);
      m->statusMessage("Mask Stack AND NOT");
    }
  }
}

void QxrdMaskingWindow::doOrNotMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdMaskStackPtr m(m_MaskStack);

    if (m) {
      m->orNotMaskStack(n);
      m->statusMessage("Mask Stack OR NOT");
    }
  }
}

void QxrdMaskingWindow::doXorNotMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdMaskStackPtr m(m_MaskStack);

    if (m) {
      m->xorNotMaskStack(n);
      m->statusMessage("Mask Stack XOR NOT");
    }
  }
}

void QxrdMaskingWindow::doExchangeMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdMaskStackPtr m(m_MaskStack);

    if (m) {
      m->exchangeMaskStack(n);
      m->statusMessage("Mask Stack Exchanged");
    }
  }
}

void QxrdMaskingWindow::doRollMask()
{
  int n = maskStackSelectPopup();

  if (n >= 0) {
    QxrdMaskStackPtr m(m_MaskStack);

    if (m) {
      m->rollMaskStack(n);
    }
  }
}

void QxrdMaskingWindow::doRollUpMask()
{
  QxrdMaskStackPtr m(m_MaskStack);

  if (m) {
    m->rollMaskStack(1);
    m->statusMessage("Mask Stack Rolled Up");
  }
}

void QxrdMaskingWindow::doRollDownMask()
{
  QxrdMaskStackPtr m(m_MaskStack);

  if (m) {
    m->rollMaskStack(-1);
    m->statusMessage("Mask Stack Rolled Down");
  }
}

void QxrdMaskingWindow::doClearMask()
{
  QxrdMaskStackPtr m(m_MaskStack);

  if (m) {
    m->clearMaskStack();
    m->statusMessage("Mask Stack Cleared");
  }
}

void QxrdMaskingWindow::doClearMaskTop()
{
  QxrdMaskStackPtr m(m_MaskStack);

  if (m) {
    m->clearMaskStackTop();
    m->statusMessage("Top of Mask Stack Cleared");
  }
}

void QxrdMaskingWindow::doPushMask()
{
  QxrdProcessorPtr proc(QxrdProcessor::findProcessor(m_Parent));

  if (proc) {
    proc->duplicateMask();
    proc->statusMessage("Mask Pushed");
  }
}

void QxrdMaskingWindow::doNewMask()
{
  QxrdProcessorPtr proc(QxrdProcessor::findProcessor(m_Parent));

  if (proc) {
    proc->newEmptyMask();
    proc->statusMessage("New Mask");
  }
}

void QxrdMaskingWindow::doUndoMask()
{
  QxrdProcessorPtr proc(QxrdProcessor::findProcessor(m_Parent));

  if (proc) {
    proc->statusMessage("Undo Not Implemented");
  }
}
