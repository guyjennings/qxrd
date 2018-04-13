#ifndef QCEPPLOTBUTTONCOMMAND_H
#define QCEPPLOTBUTTONCOMMAND_H

#include "qcepplotcommand.h"
#include "qwt_plot_picker-ptr.h"

class QCEP_EXPORT QcepPlotButtonCommand : public QcepPlotCommand
{
  Q_OBJECT

private:
  typedef QcepPlotCommand inherited;

public:
  QcepPlotButtonCommand(QString                    name,
                        QString                    desc,
                        QcepPlotWidget            *plot,
                        QcepPlotWidgetSettingsWPtr set,
                        QString                    iconPath,
                        bool                       checkable);

  void initialize(QcepObjectWPtr parent);

  QAction     *contextMenuAction(const QPoint &pos);
  QToolButton *toolButton();

  void enable();
  void disable();
  void setPen(const QPen &pen);

protected:
  void appendMode(QString iconPath, QString desc);
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

#endif // QCEPPLOTBUTTONCOMMAND_H
