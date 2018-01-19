#ifndef QXRDZINGERDIALOG_H
#define QXRDZINGERDIALOG_H

#include "qxrdlib_global.h"
#include <QDialog>
#include "ui_qxrdzingerdialog.h"

#include "qxrdprocessor-ptr.h"
#include "qxrdzingerfinder-ptr.h"

class QXRD_EXPORT QxrdZingerDialog : public QDialog, public Ui::QxrdZingerDialog
{
  Q_OBJECT

public:
  explicit QxrdZingerDialog(QxrdProcessorWPtr procw,
                            QxrdZingerFinderWPtr zf,
                            QWidget *parent = 0);
  ~QxrdZingerDialog();

  void accept();

private:
  QxrdProcessorWPtr    m_Processor;
  QxrdZingerFinderWPtr m_ZingerFinder;
};

#endif // QXRDZINGERDIALOG_H
