#include "qxrdscripteditwidget.h"
#include <QMenu>
#include <QContextMenuEvent>
#include "qxrdexperiment.h"

QxrdScriptEditWidget::QxrdScriptEditWidget(QWidget *parent) :
  QTextEdit(parent),
  m_Highlighter(NULL)
{
  m_Highlighter = new QxrdHighlighter(document());
}

void QxrdScriptEditWidget::setExperiment(QxrdExperimentWPtr expt)
{
  m_Experiment = expt;
}

void QxrdScriptEditWidget::contextMenuEvent(QContextMenuEvent * event)
{
  QMenu *menu = createStandardContextMenu();

  QString sel = textCursor().selectedText();

  menu->addAction(tr("Evaluate %1").arg(sel), this, SLOT(executeSelectedText()));
  menu->exec(event->globalPos());

  delete menu;
}

void QxrdScriptEditWidget::executeSelectedText()
{
  QString sel = textCursor().selectedText();

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    expt->executeCommand(sel);
  }
}
