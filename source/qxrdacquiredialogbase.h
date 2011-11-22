#ifndef QXRDACQUIREDIALOGBASE_H
#define QXRDACQUIREDIALOGBASE_H

#include <QDockWidget>

class QxrdDocument;
class QxrdWindow;
class QxrdAcquisition;
class QxrdDataProcessor;
#include <QMenu>

class QxrdAcquireDialogBase : public QDockWidget
{
    Q_OBJECT
public:
    explicit QxrdAcquireDialogBase(QxrdDocument *doc,
                                   QxrdWindow *win,
                                   QxrdAcquisition *acq,
                                   QxrdDataProcessor *proc,
                                   QWidget *parent = 0);

public:
  virtual void setupAcquireMenu(QMenu *menu) = 0;
  virtual void onAcquisitionInit() = 0;
  virtual void acquisitionReady() = 0;

protected:
  QxrdDocument      *m_Document;
  QxrdWindow        *m_Window;
  QxrdAcquisition   *m_Acquisition;
  QxrdDataProcessor *m_DataProcessor;
};

#endif // QXRDACQUIREDIALOGBASE_H
