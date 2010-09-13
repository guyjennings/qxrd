/******************************************************************
*
*  $Id: qxrddataprocessorcudakernel.cu,v 1.2 2010/09/13 19:59:35 jennings Exp $
*
*******************************************************************/

extern "C" void qxrdCuda();

void qxrdCuda()
{
}

#define BLOCK_SIZE  16
#define INPUT_OVERSAMPLE 1
#define OUTPUT_OVERSAMPLE 1
#define OUTPUT_BLOCK_SIZE (BLOCK_SIZE*2*OUTPUT_OVERSAMPLE)

__global__ void
    circularIntegrate(unsigned short *g_image, bool *g_mask, int *g_output, float xc, float yc)
{
  __shared__ int sums[OUTPUT_BLOCK_SIZE], npix[OUTPUT_BLOCK_SIZE];
  __shared__ int minrad;

  const int index = BLOCK_SIZE*threadIdx.y + threadIdx.x;

  if (index < OUTPUT_BLOCK_SIZE) {
    sums[index] = 0;
    npix[index] = 0;
  }

  const unsigned int blkminx = blockIdx.x * blockDim.x;
  const unsigned int blkminy = blockIdx.y * blockDim.y;
  const unsigned int blkmaxx = blkminx + BLOCK_SIZE;
  const unsigned int blkmaxy = blkminy + BLOCK_SIZE;

  if ((threadIdx.x == 0) && (threadIdx.y == 0)) {
    if (xc < blkminx) {
      if (yc < blkminy) {
        minrad = (sqrt((blkminx - xc)*(blkminx - xc) + (blkminy - yc)*(blkminy - yc))*OUTPUT_OVERSAMPLE);
      } else if (yc <= blkmaxy) {
        minrad = (blkminx-xc)*OUTPUT_OVERSAMPLE;
      } else {
        minrad = (sqrt((blkminx - xc)*(blkminx - xc) + (blkmaxy - yc)*(blkmaxy - yc))*OUTPUT_OVERSAMPLE);
      }
    } else if (xc <= blkmaxx) {
      if (yc < blkminy) {
        minrad = (blkminy-yc)*OUTPUT_OVERSAMPLE;
      } else if (yc <= blkmaxy) {
        minrad = 0;
      } else {
        minrad = (yc - blkmaxy)*OUTPUT_OVERSAMPLE;
      }
    } else {
      if (yc < blkminy) {
        minrad = (sqrt((blkmaxx - xc)*(blkmaxx - xc) + (blkminy - yc)*(blkminy - yc))*OUTPUT_OVERSAMPLE);
      } else if (yc <= blkmaxy) {
        minrad = (xc - blkmaxx)*OUTPUT_OVERSAMPLE;
      } else {
        minrad = (sqrt((blkmaxx - xc)*(blkmaxx - xc) + (blkmaxy - yc)*(blkmaxy - yc))*OUTPUT_OVERSAMPLE);
      }
    }
  }

  __syncthreads();

  const unsigned int ix = blockIdx.x * blockDim.x + threadIdx.x;
  const unsigned int iy = blockIdx.y * blockDim.y + threadIdx.y;
  const unsigned int xdim = 2048;
  const unsigned int ydim = 2048;
  float ios = 0.5/((float) INPUT_OVERSAMPLE);
  if ((ix < xdim) && (iy < ydim)) {
    if (g_mask[iy*xdim + ix]) {
      for (int iysub = 0; iysub < INPUT_OVERSAMPLE; iysub++) {
        for (int ixsub = 0; ixsub < INPUT_OVERSAMPLE; ixsub++) {
          float dx = ios + (ix - xc) + ((float)ixsub)/((float)INPUT_OVERSAMPLE);
          float dy = ios + (iy - yc) + ((float)iysub)/((float)INPUT_OVERSAMPLE);
          float radius = sqrt(dx*dx + dy*dy);

          unsigned short value = g_image[iy*xdim + ix];

          int bin = radius*OUTPUT_OVERSAMPLE - minrad;

          if ((bin >= 0) && (bin < OUTPUT_BLOCK_SIZE)) {
            atomicAdd(&sums[bin], value);
            atomicAdd(&npix[bin], 1);
          }
        }
      }
    }
  }

  __syncthreads();

  const int outbin = threadIdx.y * BLOCK_SIZE + threadIdx.x;

  if (outbin < OUTPUT_BLOCK_SIZE) {
    if (npix[outbin] > 0) {
      atomicAdd(&g_output[2*(minrad+outbin)+0], sums[outbin]);
      atomicAdd(&g_output[2*(minrad+outbin)+1], npix[outbin]);
    }
  }
}

/******************************************************************
*
*  $Log: qxrddataprocessorcudakernel.cu,v $
*  Revision 1.2  2010/09/13 19:59:35  jennings
*  Merged
*
*  Revision 1.1.2.2  2010/06/16 19:22:39  jennings
*  First steps to adding CUDA support
*
*
*******************************************************************/

