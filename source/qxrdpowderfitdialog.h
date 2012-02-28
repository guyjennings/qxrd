#ifndef QXRDPOWDERFITDIALOG_H
#define QXRDPOWDERFITDIALOG_H

#include "qxrdpowderfitresultspage.h"

class QxrdPowderFitDialog : public QxrdPowderFitResultsPage
{
  Q_OBJECT
public:
  QxrdPowderFitDialog(QxrdSettingsSaverWPtr saver, QxrdDataProcessorWPtr proc, QWidget *parent);

public slots:
  void displayPageChanged(int page);

protected:
};

#endif // QXRDPOWDERFITDIALOG_H
