#include "qxrdmaskstackview.h"
#include <QListView>
#include <QMenu>
#include <QContextMenuEvent>
#include "qcepmaskstack.h"
#include "qcepmaskstackmodel.h"
#include "qxrdprocessor.h"

QxrdMaskStackView::QxrdMaskStackView(QWidget *parent) :
  QTableView(parent),
  m_MaskStack(NULL),
  m_MaskStackModel(NULL),
  m_Processor()
{
}

void QxrdMaskStackView::setMaskStack(QcepMaskStackWPtr stk)
{
  m_MaskStack = stk;

  QcepMaskStackPtr maskStack(m_MaskStack);

  if (maskStack) {
    m_MaskStackModel = QcepMaskStackModelPtr(
          NEWPTR(QcepMaskStackModel(m_MaskStack)));

    setModel(m_MaskStackModel.data());
  }
}

void QxrdMaskStackView::setProcessor(QxrdProcessorWPtr proc)
{
  m_Processor = proc;
}

void QxrdMaskStackView::contextMenuEvent(QContextMenuEvent *event)
{
  QMenu menu(NULL, NULL);

  QModelIndexList selected = selectedIndexes();
  QcepMaskStack  *maskStack = m_MaskStack.data();
  QxrdProcessor  *proc      = m_Processor.data();

  if (proc) {
    menu.addAction("New Mask Layer",
                   [=]() {proc->newEmptyMask();});
  } else {
    menu.addAction("New Mask Layer") -> setEnabled(false);
  }

  if (maskStack) {
    menu.addAction("Enable Mask Layer(s)",
                   [=]() {maskStack->enableMasks(selected);});

    menu.addAction("Disable Mask Layer(s)",
                   [=]() {maskStack->disableMasks(selected);});

    menu.addAction("Delete Mask Layer(s)",
                   [=]() {maskStack->deleteMasks(selected);});

    menu.addAction("AND Mask Layers",
                   [=]() {maskStack->andMasks(selected);});

    menu.addAction("OR Mask Layers",
                   [=]() {maskStack->orMasks(selected);});
  } else {
    menu.addAction("Enable Mask Layer(s)") -> setEnabled(false);

    menu.addAction("Disable Mask Layer(s)") -> setEnabled(false);

    menu.addAction("Delete Mask Layer(s)") -> setEnabled(false);

    menu.addAction("AND Mask Layers") -> setEnabled(false);

    menu.addAction("OR Mask Layers") -> setEnabled(false);
  }

  //TODO: reimplement
  menu.addAction("Threshold Mask...") -> setEnabled(false);

  menu.exec(event->globalPos());

  event->accept();
}
