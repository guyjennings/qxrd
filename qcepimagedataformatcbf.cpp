#include "qcepimagedataformatcbf.h"
#include "qcepimagedata.h"
#include "cbf.h"

template <typename T>
QcepImageDataFormatCBF<T>::QcepImageDataFormatCBF(QString name) :
  QcepImageDataFormat<T>(name)
{
}

template <typename T>
QcepImageDataFormatBase::Priority QcepImageDataFormatCBF<T>::priority() const
{
  return QcepImageDataFormatBase::Versatile;
}

template <typename T>
QcepImageDataFormat<T>* QcepImageDataFormatCBF<T>::canLoadFile(QString path)
{
  FILE *file = fopen(qPrintable(path), "r");
  QcepImageDataFormat<T>* res = NULL;

  if (file) {
    cbf_handle ch;

    cbf_make_handle(&ch);

    if (cbf_read_widefile(ch, file, MSG_DIGEST) == 0) {
      res = this;
    }

    cbf_free_handle(ch);
  }

  return res;
}

template <typename T>
QcepImageDataFormat<T>* QcepImageDataFormatCBF<T>::canSaveFile(QString /*path*/)
{
  return this;
}

template <typename T>
QcepImageDataFormat<T>* QcepImageDataFormatCBF<T>::loadFile(QString path, QcepImageData<T> *img)
{
  if (img) {
    FILE *file = fopen(qPrintable(path), "rb");

    if (file) {
      cbf_handle ch;
      int status;

      cbf_make_handle(&ch);

      if (cbf_read_widefile(ch, file, MSG_DIGEST) == 0) {
        if (cbf_find_tag(ch, "_array_data.data") == 0) {
          qint32 *array;
          int binary_id, elsigned, elunsigned;
          size_t elements,elements_read, elsize;
          int minelement, maxelement;
          unsigned int cifcompression;
          const char *byteorder;
          size_t dim1, dim2, dim3, padding;

          status = cbf_get_integerarrayparameters_wdims_fs(
                                                       ch, &cifcompression,
                                                       &binary_id, &elsize, &elsigned, &elunsigned,
                                                       &elements, &minelement, &maxelement,
                                                       &byteorder, &dim1, &dim2, &dim3, &padding);

          array = new qint32[dim1*dim2];

          if (status == 0) {
            status = cbf_get_integerarray(ch, &binary_id,
                                                    array,
                                                    sizeof(qint32), 1, elements, &elements_read);

            if (status == 0) {
              img->resize(dim1, dim2);

              for (int y=0; y<dim2; y++) {
                for (int x=0; x<dim1; x++) {
                  img->setValue(x,y,array[y*dim1+x]);
                }
              }
            }
          }

          delete [] array;
        }
      }

      cbf_free_handle(ch);
    }
  }

  return this;
}

template <typename T>
QcepImageDataFormat<T>* QcepImageDataFormatCBF<T>::saveFile(QString path, QcepImageData<T> *img, int canOverwrite)
{
}

template class QcepImageDataFormatCBF<unsigned short>;
template class QcepImageDataFormatCBF<short>;
template class QcepImageDataFormatCBF<unsigned int>;
template class QcepImageDataFormatCBF<int>;
template class QcepImageDataFormatCBF<double>;
