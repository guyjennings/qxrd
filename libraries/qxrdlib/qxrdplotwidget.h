#ifndef QXRDPLOTWIDGET_H
#define QXRDPLOTWIDGET_H

#include <QWidget>
#include <QTimer>
#include "ui_qxrdplotwidget.h"
#include "qxrdplotcommand-ptr.h"
#include "qxrdplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdPlotWidget : public QWidget, public Ui::QxrdPlotWidget
{
  Q_OBJECT

public:
  explicit QxrdPlotWidget(QWidget *parent = 0);
  ~QxrdPlotWidget();
  void initialize(QxrdPlotWidgetSettingsWPtr settings);

  void addPlotCommand(QxrdPlotCommandPtr cmd);
  void addPlotCommandSpacer();

  void disableCommands();

public slots:
  void onProcessedImageAvailable(QcepDoubleImageDataPtr img);

private slots:
  void contextMenu(const QPoint &pos);
  void updateDisplayedImage();

private:
  QTimer                      m_Timer;
  QVector<QxrdPlotCommandPtr> m_PlotCommands;

  QcepDoubleImageDataPtr      m_NewImageData;
  QcepDoubleImageDataPtr      m_ImageData;
};

#endif // QXRDPLOTWIDGET_H
