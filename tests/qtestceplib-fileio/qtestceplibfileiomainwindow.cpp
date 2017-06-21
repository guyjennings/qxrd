#include "qtestceplibfileiomainwindow.h"
#include "ui_qtestceplibfileiomainwindow.h"
#include <QFileDialog>
#include "qcepmutexlocker.h"
#include "qcepimagedataformattiff.h"
#include "qcepimagedata.h"
#include "qtestceplibfileiodocument.h"
#include "qcepfileformatter.h"

static QcepImageDataFormatTiff<quint16> rawfmt("raw");
static QcepImageDataFormatTiff<quint32> raw2fmt("raw2");
static QcepImageDataFormatTiff<short> maskfmt("mask");
static QcepImageDataFormatTiff<double> dblfmt("double");

QtestceplibFileIOMainWindow::QtestceplibFileIOMainWindow(QtestceplibFileIODocument *doc, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::QtestceplibFileIOMainWindow),
  m_Document(doc),
  m_Mutex(QMutex::Recursive)
{
  ui->setupUi(this);

  connect(ui->m_ActionReadSettings, SIGNAL(triggered()), this, SLOT(doReadSettings()));
  connect(ui->m_ActionWriteSettings, SIGNAL(triggered()), this, SLOT(doWriteSettings()));
  connect(ui->m_ActionLoadImage, SIGNAL(triggered()), this, SLOT(doLoadImage()));
  connect(ui->m_ActionLoadTIFFImage, SIGNAL(triggered()), this, SLOT(doLoadTIFFImage()));

  connect(ui->m_ActionReadDataObjects, SIGNAL(triggered(bool)), this, SLOT(doReadDataObjects()));
  connect(ui->m_ActionWriteDataObjects, SIGNAL(triggered(bool)), this, SLOT(doWriteDataObjects()));

  ui->m_FileMenu->addAction(tr("QT Version %1").arg(qVersion()));
  ui->m_FileMenu->addAction(tr("QCEPLIB Version %1").arg(STR(QCEPLIB_VERSION)));
  ui->m_FileMenu->addAction(tr("TIFF Version %1").arg(STR(QCEPLIB_TIFF_VERSION)));
}

QtestceplibFileIOMainWindow::~QtestceplibFileIOMainWindow()
{
  delete ui;
}

void QtestceplibFileIOMainWindow::printMessage(QString msg)
{
  ui->m_Messages->appendPlainText(msg);
}

QString defPath;

void QtestceplibFileIOMainWindow::doReadSettings()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read Settings from...", defPath);

  if (theFile.length()) {
    readSettings(theFile);
    defPath = theFile;
  }
}

void QtestceplibFileIOMainWindow::doWriteSettings()
{
  QString theFile = QFileDialog::getSaveFileName(
        this, "Write Settings in...", defPath);

  if (theFile.length()) {
    writeSettings(theFile);
    defPath = theFile;
  }
}

void QtestceplibFileIOMainWindow::readSettings()
{
}

void QtestceplibFileIOMainWindow::writeSettings()
{
}

void QtestceplibFileIOMainWindow::readSettings(QString filePath)
{
  QSettings settings(filePath, QSettings::IniFormat);

  readSettings(&settings);
}

void QtestceplibFileIOMainWindow::writeSettings(QString filePath)
{
  QSettings settings(filePath, QSettings::IniFormat);

  writeSettings(&settings);
}

void QtestceplibFileIOMainWindow::readSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, "qtestceplib", settings);

  if (m_Document) {
    m_Document->readSettings(settings, "document");
  }
}

void QtestceplibFileIOMainWindow::writeSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, "qtestceplib", settings);

  if (m_Document) {
    m_Document->writeSettings(settings, "document");
  }
}

void QtestceplibFileIOMainWindow::doLoadImage()
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

void QtestceplibFileIOMainWindow::doLoadTIFFImage()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read TIFF Image from...", defPath);

  if (theFile.length()) {
    QcepImageDataFormatTiff<double> fmt("tiff");

    QcepDoubleImageData *img = new QcepDoubleImageData("testTIFF", 1024,1024, 0);

    fmt.loadFile(theFile, img);
  }
}

void QtestceplibFileIOMainWindow::doReadDataObjects()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read Data Objects from...", defPath);

  if (theFile.length()) {
    readDataObjects(theFile);
    defPath = theFile;
  }
}

void QtestceplibFileIOMainWindow::doWriteDataObjects()
{
  QString theFile = QFileDialog::getSaveFileName(
        this, "Write Data Objects in...", defPath);

  if (theFile.length()) {
    writeDataObjects(theFile);
    defPath = theFile;
  }
}

void QtestceplibFileIOMainWindow::readDataObjects(QString filePath)
{
  QFileInfo info(filePath);

  QString exten = info.suffix();

  QcepFileFormatterPtr fmt = QcepFileFormatter::defaultFormatter(filePath, exten);

  QcepObjectPtr obj = QcepObject::readDataObject(fmt);
}

void QtestceplibFileIOMainWindow::writeDataObjects(QString filePath)
{
  QFileInfo info(filePath);

  QString exten = info.suffix();

  QcepFileFormatterPtr fmt = QcepFileFormatter::defaultFormatter(filePath, exten);

  m_Document->writeObject(fmt);
}
