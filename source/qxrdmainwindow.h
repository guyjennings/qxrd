#ifndef QXRDMAINWINDOW_H
#define QXRDMAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QDateTime>

class QxrdMainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit QxrdMainWindow(QWidget *parent = 0);

  void possiblyClose();
  bool wantToClose();

signals:

public slots:
  void shrinkPanels(int fontSize, int spacing);
  void setFontSize(int fontSize);
  void setSpacing(int spacing);
  virtual void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) = 0;

protected:
  void shrinkDockWidget(QDockWidget *dockWidget, int fontSize, int spacing);
  void shrinkObject(QObject *obj, int fontSize, int spacing);
  void setObjectSpacing(QObject *obj, int spacing);
};

#endif // QXRDMAINWINDOW_H
