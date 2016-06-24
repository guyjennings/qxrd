#ifndef QXRDSCRIPTEDITWIDGET_H
#define QXRDSCRIPTEDITWIDGET_H

#include <QTextEdit>
#include "qxrdhighlighter.h"
#include "qxrdexperiment-ptr.h"

class QxrdScriptEditWidget : public QTextEdit
{
  Q_OBJECT
public:
  explicit QxrdScriptEditWidget(QWidget *parent = 0);

  void setExperiment(QxrdExperimentWPtr expt);
  void contextMenuEvent(QContextMenuEvent *event);

signals:

public slots:
  void executeSelectedText();
  void executeSelectedTextJS();

private:
  QxrdExperimentWPtr           m_Experiment;
  QxrdHighlighter             *m_Highlighter;
};

#endif // QXRDSCRIPTEDITWIDGET_H
