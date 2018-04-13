#include "qxrdstartupwindow.h"
#include <QDateTime>
#include <QStyle>
#include <QDesktopWidget>
#include "qxrdstartupwindowsettings.h"
#include <QThread>
#include "qxrdappcommon.h"

QxrdStartupWindow::QxrdStartupWindow(QString name) :
  inherited(name)
{
}

void QxrdStartupWindow::initialize(QcepObjectWPtr parent)
{
  GUI_THREAD_CHECK;

  setupUi(this);

  inherited::initialize(parent);

  setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);

  QGridLayout *gl = qobject_cast<QGridLayout*>(m_Layout -> layout());

  if (gl) {
    gl -> setRowStretch(0, 0);
    gl -> setRowStretch(1, 1);

    gl -> setColumnStretch(0, 0);
    gl -> setColumnStretch(1, 1);
  }

  QxrdStartupWindowSettingsPtr s(
        qSharedPointerDynamicCast<QxrdStartupWindowSettings>(m_Parent));

  if (s) {
    QSizeF sz = s->get_WindowRect().size();

    if (sz.width() < 256 ||
        sz.height() < 128) {

      sz = size();
    }

    setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            QSize(sz.width(), sz.height()),
            qApp->desktop()->availableGeometry()
        )
    );

    QxrdAppCommonPtr a(QxrdAppCommon::findApplication(m_Parent));

    if (a) {
      connect(a->prop_MessageWindowLines(), &QcepIntProperty::valueChanged,
              this, &QxrdStartupWindow::onMessageWindowLinesChanged);

      onMessageWindowLinesChanged(a->get_MessageWindowLines());
    }
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
  m_Description -> setText(desc);
}

void QxrdStartupWindow::appendSplashMessage(QString msg, QDateTime dt)
{
  GUI_THREAD_CHECK;

  appendMessage(msg, "red", dt);

  activateWindow();
  raise();
}

void QxrdStartupWindow::appendMessage(QString msg,
                                      QString color,
                                      QDateTime dt)
{
  GUI_THREAD_CHECK;

  QString day1 = m_LastDateTime.toString("[ yyyy/MM/dd hh:mm ]");
  QString day2 = dt.toString("[ yyyy/MM/dd hh:mm ]");

  if (day1 != day2) {
    m_Messages -> append("<span style=\""
                         "color:blue;font-family:monospace;font-style:italic"
                         "\">"+
                         day2+
                         "</span>");
  }

  QString secs = dt.toString("[  +ss.zzz]");

  m_Messages -> append("<span style=\""
                       "color:blue;font-family:monospace;font-style:italic"
                       "\">"+
                       secs +
                       "</span> : " +
                       "<span style=\"" +
                       "color:" +
                       color +
                       "\">" +
                       msg +
                       "</span>");

  m_LastDateTime = dt;

//  if (g_Application) {
//    g_Application -> processEvents();
//  }
}

void QxrdStartupWindow::onMessageWindowLinesChanged(int newVal)
{
  appendMessage(tr("Message lines set to %1").arg(newVal), "green");

  m_Messages -> document() -> setMaximumBlockCount(newVal);
}
