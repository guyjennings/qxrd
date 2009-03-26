#include <stdio.h>
#include <stdint.h>

typedef struct {
  uint16_t FileType;
  uint16_t unk;
  uint16_t HeaderSize;		        // Size of this file header in Bytes
  uint32_t FileSize;			// Size of the whole file in Bytes
  uint16_t ImageHeaderSize;	        // Size of the image header in Bytes
  uint16_t ULX, ULY, BRX, BRY;          // bounding rectangle of the image
  uint16_t NrOfFrames;		        // self explanatory
  uint16_t Correction;	  	        // 0 = none, 1 = offset, 2 = gain, 4 = bad pixel, (ored)
  double IntegrationTime;	        // frame time in microseconds
  uint16_t TypeOfNumbers;		// short, long integer, float, signed/u};
} HISHeader;

void his_dump(char *filename)
{
  HISHeader h;
  FILE *f;

  f=fopen(filename,"r");

  if (f) {
    fread(&h.FileType,     2, 1, f);
    fread(&h.unk,          2, 1, f);
    fread(&h.HeaderSize,   2, 1, f);
    fread(&h.FileSize,     4, 1, f);
    fread(&h.ImageHeaderSize, 2, 1, f);
    fread(&h.ULX, 2, 1, f);
    fread(&h.ULY, 2, 1, f);
    fread(&h.BRX, 2, 1, f);
    fread(&h.BRY, 2, 1, f);
    fread(&h.NrOfFrames, 2, 1, f);
    fread(&h.Correction, 2, 1, f);
    fread(&h.IntegrationTime, 8, 1, f);
    fread(&h.TypeOfNumbers, 2, 1, f);

    int width = h.BRX-h.ULX+1;
    int height = h.BRY-h.ULY+1;
    int pxlsize = (h.FileSize-100)/(width*height);

    printf("File %60s: Unk %5d, HS %5d, FS %8d, IHS %5d, (%5d,%5d)-(%5d,%5d), NF %2d, CORR %2d, Int %g, NumType %d, PxlSz %d\n",
	   filename, h.unk, h.HeaderSize, h.FileSize, h.ImageHeaderSize,
	   h.ULX, h.ULY, h.BRX, h.BRY, h.NrOfFrames, h.Correction, h.IntegrationTime/1e6, h.TypeOfNumbers, pxlsize);

    fclose(f);
  }
}

int main(int argc, char *argv[]) 
{
  printf("Size of HISHeader = %d\n", sizeof(HISHeader));

  for (int i=1; i < argc; i++) {
    his_dump(argv[i]);
  }

  return 0;
}
