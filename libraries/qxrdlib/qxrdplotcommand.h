#ifndef QXRDPLOTCOMMAND_H
#define QXRDPLOTCOMMAND_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qxrdplotwidget-ptr.h"
#include "qxrdimageplot-ptr.h"
#include "qxrdplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdPlotCommand : public QcepObject
{
  Q_OBJECT

public:
  QxrdPlotCommand(QString                    name,
                  QString                    desc,
                  QxrdPlotWidget            *plot,
                  QxrdPlotWidgetSettingsWPtr set);

  virtual QToolButton *toolButton() = 0;
  virtual QAction     *contextMenuAction(const QPoint &pos) = 0;

  virtual void         enable();
  virtual void         disable();

  virtual void         setPen(const QPen& pen);

  QxrdPlotWidgetSettingsWPtr settings() const;

protected:
  QString                    m_Description;
  QxrdPlotWidget            *m_PlotWidget;
  QxrdImagePlot             *m_Plot;
  QxrdPlotWidgetSettingsWPtr m_Settings;
};

#endif // QXRDPLOTCOMMAND_H
