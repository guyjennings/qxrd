#ifndef QXRDWELCOMEWINDOW_H
#define QXRDWELCOMEWINDOW_H

#include "qxrdlib_global.h"
#include <QMainWindow>
#include <QSettings>
#include <QLabel>
#include <QTimer>
#include "ui_qxrdwelcomewindow.h"

#include "qxrdappcommon-ptr.h"

class QXRD_EXPORT QxrdWelcomeWindow : public QMainWindow, public Ui::QxrdWelcomeWindow
{
  Q_OBJECT

public:
  explicit QxrdWelcomeWindow(QxrdAppCommonWPtr appw);
  ~QxrdWelcomeWindow();

  void appendRecentExperiment(QString title);

  void possiblyClose();
  bool wantToClose();
  void closeEvent (QCloseEvent * event);

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

public slots:
  void displayMessage(QString msg);
  void clearStatusMessage();

protected:
  void changeEvent(QEvent *e);

private slots:
  void populateRecentExperimentsMenu();
  void openMostRecent();

private:
  void setupRecentExperimentsMenu(QAction *action);

private:
  QxrdAppCommonWPtr      m_Application;
  int                    m_InsertRow;
  QLabel                *m_StatusMsg;
  QTimer                 m_StatusTimer;

  QMenu                 *m_RecentExperimentsMenu;
};

#endif // QXRDWELCOMEWINDOW_H
