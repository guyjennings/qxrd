#include "qxrdmaskstackview.h"
#include <QListView>
#include <QMenu>
#include <QContextMenuEvent>

QxrdMaskStackView::QxrdMaskStackView(QWidget *parent) :
  QTableView(parent),
  m_Dialog(NULL),
  m_MaskStack(NULL),
  m_Processor(NULL)
{
}

void QxrdMaskStackView::setMaskDialog(QxrdMaskDialog *dlg)
{
  m_Dialog = dlg;
}

void QxrdMaskStackView::setMaskStack(QxrdMaskStackPtr stk)
{
  m_MaskStack = stk;
}

void QxrdMaskStackView::setProcessor(QxrdDataProcessor *proc)
{
  m_Processor = proc;
}

void QxrdMaskStackView::contextMenuEvent(QContextMenuEvent *event)
{
  QMenu menu(NULL, NULL);

  QAction *newMask = menu.addAction("New Mask Layer");
  QAction *enbMask = menu.addAction("Enable Mask Layer(s)");
  QAction *dsbMask = menu.addAction("Disable Mask Layer(s)");
  QAction *delMask = menu.addAction("Delete Mask Layer(s)");
  QAction *andMask = menu.addAction("AND Mask Layers");
  QAction *orMask  = menu.addAction("OR Mask Layers");
  QAction *thrMask = menu.addAction("Threshold Mask...");

//  xLog->setCheckable(true);
//  yLog->setCheckable(true);
//  xLog->setChecked(get_XAxisLog());
//  yLog->setChecked(get_YAxisLog());

  QAction *action = menu.exec(event->globalPos());
  QModelIndexList selected = selectedIndexes();

  if (m_Dialog) {
    if (action == newMask) {
      m_Processor->newMaskStack();
    } else if (action == enbMask) {
      m_MaskStack->enableMasks(selected);
    } else if (action == dsbMask) {
      m_MaskStack->disableMasks(selected);
    } else if (action == delMask) {
      m_MaskStack->deleteMasks(selected);
    } else if (action == andMask) {
      m_MaskStack->andMasks(selected);
    } else if (action == orMask) {
      m_MaskStack->orMasks(selected);
    } else if (action == thrMask) {
//      m_MaskStack->thresholdMasks(selected);
    }
  }

  event->accept();
}
