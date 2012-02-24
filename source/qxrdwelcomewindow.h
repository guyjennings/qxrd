#ifndef QXRDWELCOMEWINDOW_H
#define QXRDWELCOMEWINDOW_H

#include <QMainWindow>
#include <QSignalMapper>
#include <QSettings>
#include <QLabel>
#include <QTimer>
#include "qxrdapplication.h"
#include "ui_qxrdwelcomewindow.h"

class QxrdWelcomeWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit QxrdWelcomeWindow(QxrdApplication *app);
  ~QxrdWelcomeWindow();

  void appendRecentExperiment(QString title);

  void possiblyClose();
  bool wantToClose();
  void closeEvent (QCloseEvent * event);

  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

public slots:
  void displayMessage(QString msg);
  void clearStatusMessage();

protected:
  void changeEvent(QEvent *e);

private slots:
  void populateRecentExperimentsMenu();

private:
  void setupRecentExperimentsMenu(QAction *action);

private:
  Ui::QxrdWelcomeWindow *ui;
  QxrdApplication       *m_Application;
  int                    m_InsertRow;
  QSignalMapper          m_SignalMapper;
  QLabel                *m_StatusMsg;
  QTimer                 m_StatusTimer;

  QMenu                 *m_RecentExperimentsMenu;
};

#endif // QXRDWELCOMEWINDOW_H

class QxrdWelcomeWindow;
