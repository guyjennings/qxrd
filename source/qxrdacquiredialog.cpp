/******************************************************************
*
*  $Id: qxrdacquiredialog.cpp,v 1.2 2010/09/13 20:00:38 jennings Exp $
*
*******************************************************************/

#include "qxrdacquiredialog.h"
#include "ui_qxrdacquiredialog.h"
#include "qxrdwindow.h"

QxrdAcquireDialog::QxrdAcquireDialog(QxrdWindowPtr win, QWidget *parent) :
    QWidget(parent),
    m_Window(win),
    SOURCE_IDENT("$Id: qxrdacquiredialog.cpp,v 1.2 2010/09/13 20:00:38 jennings Exp $")
{
  setupUi(this);
}

QxrdAcquireDialog::~QxrdAcquireDialog()
{
}

void QxrdAcquireDialog::changeEvent(QEvent *e)
{
  QWidget::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}

/******************************************************************
*
*  $Log: qxrdacquiredialog.cpp,v $
*  Revision 1.2  2010/09/13 20:00:38  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/09/09 19:57:31  jennings
*  Moved acquisition dialog into separate class
*  Reorganised acquisition dialog widgets to be more compact
*
*
*******************************************************************/

