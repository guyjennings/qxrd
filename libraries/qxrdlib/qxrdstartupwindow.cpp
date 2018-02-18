#include "qxrdstartupwindow.h"

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

void QxrdStartupWindow::appendMessage(QString msg)
{
  m_Messages -> append(msg);
}
