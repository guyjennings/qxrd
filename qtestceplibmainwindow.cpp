#include "qtestceplibmainwindow.h"
#include "ui_qtestceplibmainwindow.h"
#include <QFileDialog>
#include "qcepmutexlocker.h"
#include "qcepimagedataformattiff.h"
#include "qcepimagedataformatcbf.h"
#include "qcepimagedata.h"
#include "hdf5.h"
#include "napi.h"
#include "cbf.h"

static QcepImageDataFormatTiff<quint16> rawfmt("raw");
static QcepImageDataFormatTiff<quint32> raw2fmt("raw2");
static QcepImageDataFormatTiff<short> maskfmt("mask");
static QcepImageDataFormatTiff<double> dblfmt("double");
static QcepImageDataFormatCBF<double> dblcbf("dblcbf");

QtestceplibMainWindow::QtestceplibMainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::QtestceplibMainWindow),
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
  connect(ui->m_ActionLoadTIFFImage, SIGNAL(triggered()), this, SLOT(doLoadTIFFImage()));
  connect(ui->m_ActionLoadCBFImage, SIGNAL(triggered()), this, SLOT(doLoadCBFImage()));
  connect(ui->m_ActionTestHDF, SIGNAL(triggered()), this, SLOT(doTestHDF5Library()));
  connect(ui->m_ActionTestHDFSlab, SIGNAL(triggered()), this, SLOT(doTestHDF5SlabOutput()));
  connect(ui->m_ActionTestNexus, SIGNAL(triggered()), this, SLOT(doTestNexusLibrary()));
  connect(ui->m_ActionTestCBF, SIGNAL(triggered()), this, SLOT(doTestCBFLibrary()));
}

QtestceplibMainWindow::~QtestceplibMainWindow()
{
  delete ui;
}

void QtestceplibMainWindow::printMessage(QString msg)
{
  ui->m_Messages->appendPlainText(msg);
}

QString defPath, defHDFPath, defNexusPath, defCBFPath;

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
}

void QtestceplibMainWindow::writeSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, "qtestceplib", settings);
}

void QtestceplibMainWindow::doLoadImage()
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

void QtestceplibMainWindow::doLoadTIFFImage()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read TIFF Image from...", defPath);

  if (theFile.length()) {
    QcepImageDataFormatTiff<double> fmt("tiff");

    QcepImageData<double> *img = new QcepImageData<double>(QcepSettingsSaverWPtr(), 1024,1024);

    fmt.loadFile(theFile, img);
  }
}

void QtestceplibMainWindow::doLoadCBFImage()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read CBF Image from...", defPath);

  if (theFile.length()) {
    QcepImageDataFormatCBF<double> fmt("cbf");

    QcepImageData<double> *img = new QcepImageData<double>(QcepSettingsSaverWPtr(), 1024,1024);

    fmt.loadFile(theFile, img);
  }
}

void QtestceplibMainWindow::doTestHDF5Library()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read HDF5 File...", defHDFPath);

  if (theFile.length()) {
    hid_t file, data;

    file = H5Fopen(qPrintable(theFile), H5F_ACC_RDONLY, H5P_DEFAULT);
    H5Fclose(file);

    defHDFPath=theFile;
  }
}

void QtestceplibMainWindow::doTestHDF5SlabOutput()
{
  QString theFile = QFileDialog::getSaveFileName(
        this, "Save HDF Slab file...", defHDFPath);

  if (theFile.length()) {
    const int BIGDIM = 128;
    const int CHKDIM = 32;

    hid_t file_id, dataset_id, dataspace_id, memspace_id;

    file_id = H5Fcreate(qPrintable(theFile), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    hsize_t dims[3], count[3], offset[3], stride[3], block[3];

    dims[0] = BIGDIM;
    dims[1] = BIGDIM;
    dims[2] = BIGDIM;

    dataspace_id = H5Screate_simple(3, dims, NULL);

    dataset_id = H5Dcreate(file_id, "data", H5T_NATIVE_FLOAT, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    count[0] = CHKDIM;
    count[1] = CHKDIM;
    count[2] = CHKDIM;

    stride[0] = 1;
    stride[1] = 1;
    stride[2] = 1;

    block[0] = 1;
    block[1] = 1;
    block[2] = 1;

    float chk[CHKDIM][CHKDIM][CHKDIM];

    float value = 0;

    memspace_id = H5Screate_simple(3, count, NULL);

    for (int k=0; k<(BIGDIM/CHKDIM); k++) {
      for (int j=0; j<(BIGDIM/CHKDIM); j++) {
        for (int i=0; i<(BIGDIM/CHKDIM); i++) {

          for (int ck=0; ck<CHKDIM; ck++) {
            for (int cj=0; cj<CHKDIM; cj++) {
              for (int ci=0; ci<CHKDIM; ci++) {
                chk[ck][cj][ci] = value;
              }
            }
          }

          offset[0] = i*32;
          offset[1] = j*32;
          offset[2] = k*32;

          fprintf(stderr, "Offset %d,%d,%d\n", offset[0], offset[1], offset[2]);

          H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, offset, stride, count, block);

          H5Dwrite(dataset_id, H5T_NATIVE_FLOAT, memspace_id, dataspace_id, H5P_DEFAULT, chk);

          value += 1;
        }
      }
    }

    H5Fclose(file_id);
  }
}

void QtestceplibMainWindow::doTestNexusLibrary()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read Nexus File...", defNexusPath);

  if (theFile.length()) {
    NXhandle nxHandle;

    NXopen(qPrintable(theFile), NXACC_READ, &nxHandle);

    NXclose(&nxHandle);

    defNexusPath=theFile;
  }
}

int QtestceplibMainWindow::cbfCheck(int status,const char *file, int line)
{
  if (status != 0) {
    printMessage(tr("CBF Library error %1 in %2:line %3").arg(status).arg(file).arg(line));
  }

  return status;
}

#define CBF_CHECK(a) cbfCheck(a, __FILE__, __LINE__)

void QtestceplibMainWindow::doTestCBFLibrary()
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

