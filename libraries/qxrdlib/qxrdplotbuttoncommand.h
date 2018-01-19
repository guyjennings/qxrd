#ifndef QXRDPLOTBUTTONCOMMAND_H
#define QXRDPLOTBUTTONCOMMAND_H

#include "qxrdplotcommand.h"

class QXRD_EXPORT QxrdPlotButtonCommand : public QxrdPlotCommand
{
public:
  QxrdPlotButtonCommand(QString                    name,
                        QxrdPlotWidget            *plot,
                        QxrdPlotWidgetSettingsWPtr set,
                        QString                    iconPath,
                        QString                    toolTip,
                        bool                       checkable);

  void appendMode(QString iconPath, QString toolTip);

  QAction     *contextMenuAction(const QPoint &pos);
  QToolButton *toolButton();

private:
  void contextMenu(const QPoint &pos);
  void changeMode(int i);

protected:
  QToolButton  *m_ToolButton;
  QStringList   m_IconPaths;
  QStringList   m_ToolTips;
  int           m_Index;
};

#endif // QXRDPLOTBUTTONCOMMAND_H
