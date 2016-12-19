#ifndef QXRDWELCOMEWINDOW_H
#define QXRDWELCOMEWINDOW_H

#include <QMainWindow>
#include <QSignalMapper>
#include <QSettings>
#include <QLabel>
#include <QTimer>
#include "ui_qxrdwelcomewindow.h"

#include "qxrdapplication-ptr.h"

class QxrdWelcomeWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit QxrdWelcomeWindow(QxrdApplicationWPtr appw);
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
  Ui::QxrdWelcomeWindow *ui;
  QxrdApplicationWPtr    m_Application;
  int                    m_InsertRow;
  QSignalMapper          m_SignalMapper;
  QLabel                *m_StatusMsg;
  QTimer                 m_StatusTimer;

  QMenu                 *m_RecentExperimentsMenu;
};

#endif // QXRDWELCOMEWINDOW_H
