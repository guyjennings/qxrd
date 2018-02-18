#include "qxrdstartupwindow.h"
#include <QDateTime>

QxrdStartupWindow::QxrdStartupWindow(QWidget *parent) :
  QMainWindow(parent)
{
  setupUi(this);

  QGridLayout *gl = qobject_cast<QGridLayout*>(m_Layout -> layout());

  if (gl) {
    gl -> setRowStretch(0, 0);
    gl -> setRowStretch(1, 1);

    gl -> setColumnStretch(0, 0);
    gl -> setColumnStretch(1, 1);
  }
}

QxrdStartupWindow::~QxrdStartupWindow()
{
}

void QxrdStartupWindow::setApplicationIcon(QIcon icon)
{
  m_Icon -> setPixmap(icon.pixmap(128,128));
}

void QxrdStartupWindow::setApplicationDescription(QString desc)
{
  m_Description -> append(desc);
}

void QxrdStartupWindow::appendMessage(QString msg, QDateTime dt)
{
  QString day1 = m_LastDateTime.toString("[ yyyy/MM/dd hh:mm ]");
  QString day2 = dt.toString("[ yyyy/MM/dd hh:mm ]");

  if (day1 != day2) {
    m_Messages -> append("<span style=\"color:blue\">"+
                         day2+
                         "</span>");
  }

  QString secs = dt.toString("[  +ss.zzz]");

  m_Messages -> append("<span style=\"color:blue\">"+
                       secs +
                       "</span> : " +
                       msg);

  m_LastDateTime = dt;
}
