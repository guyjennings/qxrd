#ifndef QXRDCORRECTIONDIALOGPAGE_H
#define QXRDCORRECTIONDIALOGPAGE_H

#include <QWidget>
#include "ui_qxrdcorrectiondialogpage.h"
#include "qxrddetectorsettings-ptr.h"
#include "qxrdprocessor-ptr.h"

class QxrdCorrectionDialogPage : public QWidget, public Ui::QxrdCorrectionDialogPage
{
  Q_OBJECT

public:
  explicit QxrdCorrectionDialogPage(int i,
                                    QxrdDetectorSettingsWPtr detw);
  ~QxrdCorrectionDialogPage();

  void accept();
  void enablePage(bool e);

private:
  QxrdDetectorSettingsWPtr m_Detector;
  QxrdProcessorWPtr        m_Processor;
};

#endif // QXRDCORRECTIONDIALOGPAGE_H
