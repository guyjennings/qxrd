#include "qtestcepliblevmarmainwindow.h"
#include "ui_qtestcepliblevmarmainwindow.h"
#include <QFileDialog>
#include "qcepmutexlocker.h"
#include "qcepimagedata.h"
#include "qtestcepliblevmardocument.h"

QtestceplibLevMarMainWindow::QtestceplibLevMarMainWindow(QtestceplibLevMarDocument *doc, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::QtestceplibLevMarMainWindow),
  m_Document(doc),
  m_Mutex(QMutex::Recursive)
{
  ui->setupUi(this);

  connect(ui->m_ActionReadSettings, SIGNAL(triggered()), this, SLOT(doReadSettings()));
  connect(ui->m_ActionWriteSettings, SIGNAL(triggered()), this, SLOT(doWriteSettings()));
  connect(ui->m_ActionLoadImage, SIGNAL(triggered()), this, SLOT(doLoadImage()));
  connect(ui->m_ActionTestLevMar, SIGNAL(triggered()), this, SLOT(doTestLevMar()));

  ui->m_FileMenu->addAction(tr("QT Version %1").arg(qVersion()));
  ui->m_FileMenu->addAction(tr("QCEPLIB Version %1").arg(qceplibVersion()));
  ui->m_FileMenu->addAction(tr("LevMar Version %1").arg(STR(QCEPLIB_LEVMAR_VERSION)));
}

QtestceplibLevMarMainWindow::~QtestceplibLevMarMainWindow()
{
  delete ui;
}

void QtestceplibLevMarMainWindow::printMessage(QString msg)
{
  ui->m_Messages->appendPlainText(msg);
}

QString defPath;

void QtestceplibLevMarMainWindow::doReadSettings()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read Settings from...", defPath);

  if (theFile.length()) {
    readSettings(theFile);
    defPath = theFile;
  }
}

void QtestceplibLevMarMainWindow::doWriteSettings()
{
  QString theFile = QFileDialog::getSaveFileName(
        this, "Write Settings in...", defPath);

  if (theFile.length()) {
    writeSettings(theFile);
    defPath = theFile;
  }
}

void QtestceplibLevMarMainWindow::readSettings()
{
}

void QtestceplibLevMarMainWindow::writeSettings()
{
}

void QtestceplibLevMarMainWindow::readSettings(QString filePath)
{
  QSettings settings(filePath, QSettings::IniFormat);

  readSettings(&settings);
}

void QtestceplibLevMarMainWindow::writeSettings(QString filePath)
{
  QSettings settings(filePath, QSettings::IniFormat);

  writeSettings(&settings);
}

void QtestceplibLevMarMainWindow::readSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, settings);

  if (m_Document) {
    m_Document->readSettings(settings);
  }
}

void QtestceplibLevMarMainWindow::writeSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, settings);

  if (m_Document) {
    m_Document->writeSettings(settings);
  }
}

void QtestceplibLevMarMainWindow::doLoadImage()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read Image from...", defPath);

  if (theFile.length()) {
    QcepDoubleImageData *img = new QcepDoubleImageData("image", 1024,1024, 0);

    if (img->readImage(theFile)) {
      img->loadMetaData();

      printMessage(tr("Loaded image from %1").arg(theFile));
      printMessage(tr(" width %1, height %2").arg(img->get_Width()).arg(img->get_Height()));
    } else {
      printMessage(tr("Image load failed from %1").arg(theFile));
    }
  }
}

void QtestceplibLevMarMainWindow::doTestLevMar()
{
}

