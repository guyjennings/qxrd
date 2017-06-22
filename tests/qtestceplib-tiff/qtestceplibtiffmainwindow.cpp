#include "qtestceplibtiffmainwindow.h"
#include "ui_qtestceplibtiffmainwindow.h"
#include <QFileDialog>
#include "qcepmutexlocker.h"
#include "qcepimagedataformattiff.h"
#include "qcepimagedata.h"
#include "qtestceplibtiffdocument.h"

static QcepImageDataFormatTiff<quint16> rawfmt("raw");
static QcepImageDataFormatTiff<quint32> raw2fmt("raw2");
static QcepImageDataFormatTiff<short> maskfmt("mask");
static QcepImageDataFormatTiff<double> dblfmt("double");

QtestceplibTiffMainWindow::QtestceplibTiffMainWindow(QtestceplibTiffDocument *doc, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::QtestceplibTiffMainWindow),
  m_Document(doc),
  m_Mutex(QMutex::Recursive)
{
  ui->setupUi(this);

  connect(ui->m_ActionReadSettings, SIGNAL(triggered()), this, SLOT(doReadSettings()));
  connect(ui->m_ActionWriteSettings, SIGNAL(triggered()), this, SLOT(doWriteSettings()));
  connect(ui->m_ActionLoadImage, SIGNAL(triggered()), this, SLOT(doLoadImage()));
  connect(ui->m_ActionLoadTIFFImage, SIGNAL(triggered()), this, SLOT(doLoadTIFFImage()));

  ui->m_FileMenu->addAction(tr("QT Version %1").arg(qVersion()));
  ui->m_FileMenu->addAction(tr("QCEPLIB Version %1").arg(qceplibVersion()));
  ui->m_FileMenu->addAction(tr("TIFF Version %1").arg(qcepTIFFVersion()));
}

QtestceplibTiffMainWindow::~QtestceplibTiffMainWindow()
{
  delete ui;
}

void QtestceplibTiffMainWindow::printMessage(QString msg)
{
  ui->m_Messages->appendPlainText(msg);
}

QString defPath;

void QtestceplibTiffMainWindow::doReadSettings()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read Settings from...", defPath);

  if (theFile.length()) {
    readSettings(theFile);
    defPath = theFile;
  }
}

void QtestceplibTiffMainWindow::doWriteSettings()
{
  QString theFile = QFileDialog::getSaveFileName(
        this, "Write Settings in...", defPath);

  if (theFile.length()) {
    writeSettings(theFile);
    defPath = theFile;
  }
}

void QtestceplibTiffMainWindow::readSettings()
{
}

void QtestceplibTiffMainWindow::writeSettings()
{
}

void QtestceplibTiffMainWindow::readSettings(QString filePath)
{
  QSettings settings(filePath, QSettings::IniFormat);

  readSettings(&settings);
}

void QtestceplibTiffMainWindow::writeSettings(QString filePath)
{
  QSettings settings(filePath, QSettings::IniFormat);

  writeSettings(&settings);
}

void QtestceplibTiffMainWindow::readSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, "qtestceplib", settings);

  if (m_Document) {
    m_Document->readSettings(settings, "document");
  }
}

void QtestceplibTiffMainWindow::writeSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, "qtestceplib", settings);

  if (m_Document) {
    m_Document->writeSettings(settings, "document");
  }
}

void QtestceplibTiffMainWindow::doLoadImage()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read Image from...", defPath);

  if (theFile.length()) {
    QcepDoubleImageData *img = new QcepDoubleImageData("testTIFF", 1024,1024, 0);

    if (img->readImage(theFile)) {
      img->loadMetaData();

      printMessage(tr("Loaded image from %1").arg(theFile));
      printMessage(tr(" width %1, height %2").arg(img->get_Width()).arg(img->get_Height()));
    } else {
      printMessage(tr("Image load failed from %1").arg(theFile));
    }
  }
}

void QtestceplibTiffMainWindow::doLoadTIFFImage()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read TIFF Image from...", defPath);

  if (theFile.length()) {
    QcepImageDataFormatTiff<double> fmt("tiff");

    QcepDoubleImageData *img = new QcepDoubleImageData("testTIFF", 1024,1024, 0);

    fmt.loadFile(theFile, img);
  }
}
