#ifndef QXRDPLOTBUTTONCOMMAND_H
#define QXRDPLOTBUTTONCOMMAND_H

#include "qxrdplotcommand.h"
#include "qwt_plot_picker-ptr.h"

class QXRD_EXPORT QxrdPlotButtonCommand : public QxrdPlotCommand
{
  Q_OBJECT

private:
  typedef QxrdPlotCommand inherited;

public:
  QxrdPlotButtonCommand(QString                    name,
                        QxrdPlotWidget            *plot,
                        QxrdPlotWidgetSettingsWPtr set,
                        QString                    iconPath,
                        QString                    toolTip,
                        bool                       checkable);

  void initialize(QcepObjectWPtr parent);

  QAction     *contextMenuAction(const QPoint &pos);
  QToolButton *toolButton();

  void enable();
  void disable();
  void setPen(const QPen &pen);

protected:
  void appendMode(QString iconPath, QString toolTip);
  void setPlotPicker(QwtPlotPicker* pick);
  void toggled(bool on);
  void clicked(bool checked);
  virtual void changeMode(int i);

private:
  void  contextMenu(const QPoint &pos);
  QIcon commandIcon(int i);

protected:
  QToolButton     *m_ToolButton;
  QwtPlotPicker   *m_PlotPicker;
  QStringList      m_IconPaths;
  QStringList      m_ToolTips;
  int              m_Index;
};

#endif // QXRDPLOTBUTTONCOMMAND_H
