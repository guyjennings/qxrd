#include "qtestceplibmainwindow.h"
#include "qtestceplibdocument.h"
#include "ui_qtestceplibmainwindow.h"
#include <QFileDialog>
#include "qcepmutexlocker.h"
#include "qtestimagedata.h"

QtestceplibMainWindow::QtestceplibMainWindow(QtestceplibDocument *doc, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::QtestceplibMainWindow),
  m_Document(doc),
  m_Mutex(QMutex::Recursive),
  m_ImageData(NULL)
{
  ui->setupUi(this);

  connect(ui->m_ActionReadSettings, SIGNAL(triggered()), this, SLOT(doReadSettings()));
  connect(ui->m_ActionWriteSettings, SIGNAL(triggered()), this, SLOT(doWriteSettings()));
  connect(ui->m_ActionLoadImage, SIGNAL(triggered()), this, SLOT(doLoadImage()));
  connect(ui->m_ActionNewImage, SIGNAL(triggered()), this, SLOT(doNewImage()));
  connect(ui->m_ActionSaveImage, SIGNAL(triggered()), this, SLOT(doSaveImage()));

  ui->m_FileMenu->addAction(tr("QT Version %1").arg(qVersion()));
  ui->m_FileMenu->addAction(tr("QCEPLIB Version %1").arg(STR(QCEPLIB_VERSION)));

  if (m_Document) {
    m_Document->prop_StrProp()->linkTo(ui->m_TextEdit);
  }
}

QtestceplibMainWindow::~QtestceplibMainWindow()
{
  delete ui;
}

void QtestceplibMainWindow::printMessage(QString msg, QDateTime ts)
{
  ui->m_Messages->appendPlainText(msg);
}

QString defPath;

void QtestceplibMainWindow::doReadSettings()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read Settings from...", defPath);

  if (theFile.length()) {
    readSettings(theFile);
    defPath = theFile;
  }
}

void QtestceplibMainWindow::doWriteSettings()
{
  QString theFile = QFileDialog::getSaveFileName(
        this, "Write Settings in...", defPath);

  if (theFile.length()) {
    writeSettings(theFile);
    defPath = theFile;
  }
}

void QtestceplibMainWindow::readSettings()
{
}

void QtestceplibMainWindow::writeSettings()
{
}

void QtestceplibMainWindow::readSettings(QString filePath)
{
  QSettings settings(filePath, QSettings::IniFormat);

  readSettings(&settings);
}

void QtestceplibMainWindow::writeSettings(QString filePath)
{
  QSettings settings(filePath, QSettings::IniFormat);

  writeSettings(&settings);
}

void QtestceplibMainWindow::readSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, "qtestceplib", settings);

  if (m_Document) {
    m_Document->readSettings(settings, "document");
  }
}


void QtestceplibMainWindow::writeSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, "qtestceplib", settings);

  if (m_Document) {
    m_Document->writeSettings(settings, "document");
  }
}

void QtestceplibMainWindow::doNewImage()
{
  if (m_ImageData) {
    delete m_ImageData;

    m_ImageData = NULL;
  }

  m_ImageData = new QTestImageData("testImage", 1024,1024, 0);

  printMessage("New image created");
}

void QtestceplibMainWindow::doLoadImage()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read Image from...", defPath);

  if (theFile.length()) {
    QTestImageData *img = new QTestImageData("testImage", 1024,1024, 0);

    if (img->readImage(theFile)) {
      img->loadMetaData();

      printMessage(tr("Loaded image from %1").arg(theFile));
      printMessage(tr(" width %1, height %2").arg(img->get_Width()).arg(img->get_Height()));

      if (m_ImageData) {
        delete m_ImageData;

        m_ImageData = NULL;
      }

      m_ImageData = img;
    } else {
      printMessage(tr("Image load failed from %1").arg(theFile));
    }
  }
}

void QtestceplibMainWindow::doSaveImage()
{
  if (m_ImageData) {
    QString theFile = QFileDialog::getSaveFileName(
          this, "Save image file to", defPath);

    if (theFile.length()) {
      if (m_ImageData->writeImage(theFile)) {
        m_ImageData->saveMetaData();

        printMessage(tr("Saved image to %1").arg(theFile));
      }
    }
  } else {
    printMessage("No image data to save");
  }
}
