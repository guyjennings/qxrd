#include "qtestceplibcbfmainwindow.h"
#include "ui_qtestceplibcbfmainwindow.h"
#include <QFileDialog>
#include "qcepmutexlocker.h"
#include "qcepimagedataformatcbf.h"
#include "qcepimagedata.h"

#include "cbf.h"

static QcepImageDataFormatCBF<double> dblcbf("dblcbf");

QtestceplibCbfMainWindow::QtestceplibCbfMainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::QtestceplibCbfMainWindow),
  m_Mutex(QMutex::Recursive),
  m_IntProp(QcepSettingsSaverWPtr(), this, "intProp", 42, "Integer Property"),
  m_DblProp(QcepSettingsSaverWPtr(), this, "dblProp", 42.0, "Double Property"),
  m_StrProp(QcepSettingsSaverWPtr(), this, "strProp", "42", "String Property"),
  m_SListProp(QcepSettingsSaverWPtr(), this, "sListProp", QcepStringList(), "String List Property"),
  m_Vec3dPropA(QcepSettingsSaverWPtr(), this, "vec3dPropA", QcepVector3D(1,2,3), "Vector 3D Property A"),
  m_Vec3dPropB(QcepSettingsSaverWPtr(), this, "vec3dPropB", 1,2,3, "Vector 3D Property B"),
  m_Mat3x3PropA(QcepSettingsSaverWPtr(), this, "mat3x3PropA", QcepMatrix3x3(), "Matrix 3x3 Property A"),
  m_Mat3x3PropB(QcepSettingsSaverWPtr(), this, "mat3x3PropB",
                1, 0, 0,
                0, 1, 0,
                0, 0, 1,
                "Matrix 3x3 Property B")
{
  ui->setupUi(this);

  connect(ui->m_ActionReadSettings, SIGNAL(triggered()), this, SLOT(doReadSettings()));
  connect(ui->m_ActionWriteSettings, SIGNAL(triggered()), this, SLOT(doWriteSettings()));
  connect(ui->m_ActionLoadImage, SIGNAL(triggered()), this, SLOT(doLoadImage()));
  connect(ui->m_ActionLoadCBFImage, SIGNAL(triggered()), this, SLOT(doLoadCBFImage()));
  connect(ui->m_ActionTestCBF, SIGNAL(triggered()), this, SLOT(doTestCBFLibrary()));
}

QtestceplibCbfMainWindow::~QtestceplibCbfMainWindow()
{
  delete ui;
}

void QtestceplibCbfMainWindow::printMessage(QString msg)
{
  ui->m_Messages->appendPlainText(msg);
}

QString defPath, defCBFPath;

void QtestceplibCbfMainWindow::doReadSettings()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read Settings from...", defPath);

  if (theFile.length()) {
    readSettings(theFile);
    defPath = theFile;
  }
}

void QtestceplibCbfMainWindow::doWriteSettings()
{
  QString theFile = QFileDialog::getSaveFileName(
        this, "Write Settings in...", defPath);

  if (theFile.length()) {
    writeSettings(theFile);
    defPath = theFile;
  }
}

void QtestceplibCbfMainWindow::readSettings()
{
}

void QtestceplibCbfMainWindow::writeSettings()
{
}

void QtestceplibCbfMainWindow::readSettings(QString filePath)
{
  QSettings settings(filePath, QSettings::IniFormat);

  readSettings(&settings);
}

void QtestceplibCbfMainWindow::writeSettings(QString filePath)
{
  QSettings settings(filePath, QSettings::IniFormat);

  writeSettings(&settings);
}

void QtestceplibCbfMainWindow::readSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, "qtestceplib", settings);
}

void QtestceplibCbfMainWindow::writeSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, "qtestceplib", settings);
}

void QtestceplibCbfMainWindow::doLoadImage()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read Image from...", defPath);

  if (theFile.length()) {
    QcepImageData<double> *img = new QcepImageData<double>(QcepSettingsSaverWPtr(), 1024,1024);

    if (img->readImage(theFile)) {
      img->loadMetaData();

      printMessage(tr("Loaded image from %1").arg(theFile));
      printMessage(tr(" width %1, height %2").arg(img->get_Width()).arg(img->get_Height()));
    } else {
      printMessage(tr("Image load failed from %1").arg(theFile));
    }
  }
}

void QtestceplibCbfMainWindow::doLoadCBFImage()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read CBF Image from...", defPath);

  if (theFile.length()) {
    QcepImageDataFormatCBF<double> fmt("cbf");

    QcepImageData<double> *img = new QcepImageData<double>(QcepSettingsSaverWPtr(), 1024,1024);

    fmt.loadFile(theFile, img);
  }
}

int QtestceplibCbfMainWindow::cbfCheck(int status,const char *file, int line)
{
  if (status != 0) {
    printMessage(tr("CBF Library error %1 in %2:line %3").arg(status).arg(file).arg(line));
  }

  return status;
}

#define CBF_CHECK(a) cbfCheck(a, __FILE__, __LINE__)

void QtestceplibCbfMainWindow::doTestCBFLibrary()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read CBF File...", defCBFPath);

  if (theFile.length()) {
    FILE *f;
    cbf_handle ch;
    int status;
    unsigned int m;
    char *array_id;
    int i, index, dimension[2], precedence[2];
    const char *direction[2];

    printMessage(tr("cbf test on file %1").arg(theFile));

    f = fopen(qPrintable(theFile), "rb");

    cbf_make_handle(&ch);

    status = CBF_CHECK(cbf_read_widefile(ch, f, MSG_DIGEST));
    printMessage(tr("read_widefile (%1)").arg(status));

    status = CBF_CHECK(cbf_count_datablocks(ch, &m));
    printMessage(tr("count_dbs (%1) = %2").arg(status).arg(m));

    status = CBF_CHECK(cbf_rewind_datablock(ch));

    if (cbf_find_category(ch, "diffrn_frame_data") !=0 ) {
      status = CBF_CHECK(cbf_find_category(ch, "diffrn_data_frame"));
    }

    status = CBF_CHECK(cbf_find_column(ch, "array_id"));

    status = CBF_CHECK(cbf_rewind_row(ch));

    status = CBF_CHECK(cbf_get_value(ch, (const char **) &array_id));

    status = CBF_CHECK(cbf_find_category(ch, "array_structure_list"));

    status = CBF_CHECK(cbf_rewind_row(ch));

    status = CBF_CHECK(cbf_find_column(ch, "array_id"));

    dimension[0] = dimension[1] = 0;

    while (cbf_find_nextrow(ch, array_id) == 0) {
      status = CBF_CHECK(cbf_find_column(ch, "index"));
      status = CBF_CHECK(cbf_get_integervalue(ch, &index));

      i = index;

      status = CBF_CHECK(cbf_find_column(ch, "precedence"));
      status = CBF_CHECK(cbf_get_integervalue(ch, &index));

      if (index >= 1 && index <= 2) {
        precedence[i-1] = index;

        status = CBF_CHECK(cbf_find_column(ch, "dimension"));
        status = CBF_CHECK(cbf_get_integervalue(ch, &dimension[i-1]));

        status = CBF_CHECK(cbf_find_column(ch, "direction"));
        status = CBF_CHECK(cbf_get_value(ch, &direction[i-1]));

        status = CBF_CHECK(cbf_find_column(ch, "array_id"));
      }
    }

    printMessage(tr("Image Dimensions [%1,%2]").arg(dimension[0]).arg(dimension[1]));

    status = CBF_CHECK(cbf_rewind_datablock(ch));

    if (CBF_CHECK(cbf_find_tag(ch, "_array_data.data")) == 0) {
      qint32 *array;
      int binary_id, elsigned, elunsigned;
      size_t elements,elements_read, elsize;
      int minelement, maxelement;
      unsigned int cifcompression;
      int realarray;
      const char *byteorder;
      size_t dim1, dim2, dim3, padding;

      status = CBF_CHECK(cbf_get_integerarrayparameters_wdims_fs(
                                                   ch, &cifcompression,
                                                   &binary_id, &elsize, &elsigned, &elunsigned,
                                                   &elements, &minelement, &maxelement,
                                                   &byteorder, &dim1, &dim2, &dim3, &padding));

      printMessage(tr("elsize %1, dim1 %2, dim2 %3, dim3 %4").arg(elsize).arg(dim1).arg(dim2).arg(dim3));

      array = new qint32[dim1*dim2];

      if (status == 0) {
        status = CBF_CHECK(cbf_get_integerarray(ch, &binary_id,
                                                array,
                                                sizeof(qint32), 1, elements, &elements_read));

        if (status == 0) {
          printMessage(tr("%1 elements read").arg(elements_read));

          for (int y=0; y<10; y++) {
            QString msg="[";
            for (int x=0; x<9; x++) {
              msg += tr("%1, ").arg(array[y*dim1+x]);
            }
            msg += tr("%1]").arg(array[y*dim1+9]);
            printMessage(msg);
          }
        }
      }

      delete [] array;
    }

//    fclose(f);

    defCBFPath=theFile;
  }
}

