#ifndef QXRDSCRIPTEDITWIDGET_H
#define QXRDSCRIPTEDITWIDGET_H

#include "qxrdlib_global.h"
#include "qceptextedit.h"
#include "qxrdhighlighter.h"
#include "qxrdexperiment-ptr.h"

class QXRD_EXPORT QxrdScriptEditWidget : public QcepTextEdit
{
  Q_OBJECT

private:
  typedef QcepTextEdit inherited;

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
