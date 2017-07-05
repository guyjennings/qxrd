#include "qtestceplibmar345mainwindow.h"
#include "ui_qtestceplibmar345mainwindow.h"
#include <QFileDialog>
#include "qcepmutexlocker.h"
#include "qcepimagedataformatmar345.h"
#include "qcepimagedata.h"
#include "qtestceplibmar345document.h"

static QcepImageDataFormatMar345<quint16> rawfmt("raw");
static QcepImageDataFormatMar345<quint32> raw2fmt("raw2");
static QcepImageDataFormatMar345<short> maskfmt("mask");
static QcepImageDataFormatMar345<double> dblfmt("double");

QtestceplibMar345MainWindow::QtestceplibMar345MainWindow(QtestceplibMar345Document *doc, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::QtestceplibMar345MainWindow),
  m_Document(doc),
  m_Mutex(QMutex::Recursive)
{
  ui->setupUi(this);

  connect(ui->m_ActionReadSettings, SIGNAL(triggered()), this, SLOT(doReadSettings()));
  connect(ui->m_ActionWriteSettings, SIGNAL(triggered()), this, SLOT(doWriteSettings()));
  connect(ui->m_ActionLoadImage, SIGNAL(triggered()), this, SLOT(doLoadImage()));

  ui->m_FileMenu->addAction(tr("QT Version %1").arg(qVersion()));
  ui->m_FileMenu->addAction(tr("QCEPLIB Version %1").arg(qceplibVersion()));
  ui->m_FileMenu->addAction(tr("MAR345 Version %1").arg(STR(QCEPLIB_MAR345_VERSION)));
}

QtestceplibMar345MainWindow::~QtestceplibMar345MainWindow()
{
  delete ui;
}

void QtestceplibMar345MainWindow::printMessage(QString msg)
{
  ui->m_Messages->appendPlainText(msg);
}

QString defPath, defHDFPath, defNexusPath, defCBFPath;

void QtestceplibMar345MainWindow::doReadSettings()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read Settings from...", defPath);

  if (theFile.length()) {
    readSettings(theFile);
    defPath = theFile;
  }
}

void QtestceplibMar345MainWindow::doWriteSettings()
{
  QString theFile = QFileDialog::getSaveFileName(
        this, "Write Settings in...", defPath);

  if (theFile.length()) {
    writeSettings(theFile);
    defPath = theFile;
  }
}

void QtestceplibMar345MainWindow::readSettings()
{
}

void QtestceplibMar345MainWindow::writeSettings()
{
}

void QtestceplibMar345MainWindow::readSettings(QString filePath)
{
  QSettings settings(filePath, QSettings::IniFormat);

  readSettings(&settings);
}

void QtestceplibMar345MainWindow::writeSettings(QString filePath)
{
  QSettings settings(filePath, QSettings::IniFormat);

  writeSettings(&settings);
}

void QtestceplibMar345MainWindow::readSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, settings);

  if (m_Document) {
    m_Document->readSettings(settings);
  }
}

void QtestceplibMar345MainWindow::writeSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, settings);

  if (m_Document) {
    m_Document->writeSettings(settings);
  }
}

void QtestceplibMar345MainWindow::doLoadImage()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read Image from...", defPath);

  if (theFile.length()) {
    QcepDoubleImageData *img = new QcepDoubleImageData("testMar345", 1024,1024, 0);

    if (img->readImage(theFile)) {
      img->loadMetaData();

      printMessage(tr("Loaded image from %1").arg(theFile));
      printMessage(tr(" width %1, height %2").arg(img->get_Width()).arg(img->get_Height()));
    } else {
      printMessage(tr("Image load failed from %1").arg(theFile));
    }
  }
}
