#ifndef QCEPCONTEXTSEPARATORCOMMAND_H
#define QCEPCONTEXTSEPARATORCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QCEP_EXPORT QcepContextSeparatorCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QcepContextSeparatorCommand(QcepPlotWidget            *plot,
                              QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QCEPCONTEXTSEPARATORCOMMAND_H
