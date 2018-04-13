#ifndef QCEPPLOTCOMMAND_H
#define QCEPPLOTCOMMAND_H

#include "qceplib_global.h"
#include "qcepobject.h"
#include "qcepplotwidget-ptr.h"
#include "qcepimageplot-ptr.h"
#include "qcepplotwidgetsettings-ptr.h"

class QCEP_EXPORT QcepPlotCommand : public QcepObject
{
  Q_OBJECT

public:
  QcepPlotCommand(QString                    name,
                  QString                    desc,
                  QcepPlotWidget            *plot,
                  QcepPlotWidgetSettingsWPtr set);

  virtual QToolButton *toolButton() = 0;
  virtual QAction     *contextMenuAction(const QPoint &pos) = 0;

  virtual void         enable();
  virtual void         disable();

  virtual void         setPen(const QPen& pen);

  QcepPlotWidgetSettingsWPtr settings() const;

protected:
  QString                    m_Description;
  QcepPlotWidget            *m_PlotWidget;
  QcepImagePlot             *m_Plot;
  QcepPlotWidgetSettingsWPtr m_Settings;
};

#endif // QCEPPLOTCOMMAND_H
