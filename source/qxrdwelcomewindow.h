#ifndef QXRDWELCOMEWINDOW_H
#define QXRDWELCOMEWINDOW_H

#include <QMainWindow>

class QxrdApplication;

namespace Ui {
class QxrdWelcomeWindow;
}

class QxrdWelcomeWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit QxrdWelcomeWindow(QxrdApplication *app);
  ~QxrdWelcomeWindow();

  void appendRecentDocument(QString title);

protected:
  void changeEvent(QEvent *e);

private:
  Ui::QxrdWelcomeWindow *ui;
  QxrdApplication       *m_Application;
  int                    m_InsertRow;
};

#endif // QXRDWELCOMEWINDOW_H
