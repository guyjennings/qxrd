#include "qtestceplibqwtmainwindow.h"
#include "ui_qtestceplibqwtmainwindow.h"
#include <QFileDialog>
#include "qcepmutexlocker.h"
#include "qcepimagedata.h"
#include "qtestceplibqwtdocument.h"

QtestceplibQwtMainWindow::QtestceplibQwtMainWindow(QtestceplibQwtDocument *doc, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::QtestceplibQwtMainWindow),
  m_Document(doc),
  m_Mutex(QMutex::Recursive)
{
  ui->setupUi(this);

  connect(ui->m_ActionReadSettings, SIGNAL(triggered()), this, SLOT(doReadSettings()));
  connect(ui->m_ActionWriteSettings, SIGNAL(triggered()), this, SLOT(doWriteSettings()));
  connect(ui->m_ActionLoadImage, SIGNAL(triggered()), this, SLOT(doLoadImage()));

  ui->m_FileMenu->addAction(tr("QT Version %1").arg(qVersion()));
  ui->m_FileMenu->addAction(tr("QCEPLIB Version %1").arg(STR(QCEPLIB_VERSION)));
  ui->m_FileMenu->addAction(tr("QWT Version %1").arg(STR(QCEPLIB_QWT_VERSION)));
}

QtestceplibQwtMainWindow::~QtestceplibQwtMainWindow()
{
  delete ui;
}

void QtestceplibQwtMainWindow::printMessage(QString msg)
{
  ui->m_Messages->appendPlainText(msg);
}

QString defPath;

void QtestceplibQwtMainWindow::doReadSettings()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read Settings from...", defPath);

  if (theFile.length()) {
    readSettings(theFile);
    defPath = theFile;
  }
}

void QtestceplibQwtMainWindow::doWriteSettings()
{
  QString theFile = QFileDialog::getSaveFileName(
        this, "Write Settings in...", defPath);

  if (theFile.length()) {
    writeSettings(theFile);
    defPath = theFile;
  }
}

void QtestceplibQwtMainWindow::readSettings()
{
}

void QtestceplibQwtMainWindow::writeSettings()
{
}

void QtestceplibQwtMainWindow::readSettings(QString filePath)
{
  QSettings settings(filePath, QSettings::IniFormat);

  readSettings(&settings);
}

void QtestceplibQwtMainWindow::writeSettings(QString filePath)
{
  QSettings settings(filePath, QSettings::IniFormat);

  writeSettings(&settings);
}

void QtestceplibQwtMainWindow::readSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, "qtestceplib", settings);

  if (m_Document) {
    m_Document->readSettings(settings, "document");
  }
}

void QtestceplibQwtMainWindow::writeSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, "qtestceplib", settings);

  if (m_Document) {
    m_Document->writeSettings(settings, "document");
  }
}

void QtestceplibQwtMainWindow::doLoadImage()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read Image from...", defPath);

  if (theFile.length()) {
    QcepDoubleImageData *img = new QcepDoubleImageData("image",1024,1024,0);

    if (img->readImage(theFile)) {
      img->loadMetaData();

      printMessage(tr("Loaded image from %1").arg(theFile));
      printMessage(tr(" width %1, height %2").arg(img->get_Width()).arg(img->get_Height()));
    } else {
      printMessage(tr("Image load failed from %1").arg(theFile));
    }
  }
}
