/*
  A js script to generate an array of plot markers from a calibration grid image
  */

var grid;
var x0 = 58.6275;
var y0 = 45.2048;
var nx = 39;
var ny = 39;
var dxx = (1984.96 - x0)/38.0;
var dxy = (54.9697 - y0)/38.0;
var dyx = (50.1158 - x0)/38.0;
var dyy = (1971.85 - y0)/38.0;

function calgrid()
{
  grid = [];

  centering.deletePowderPoints();

  for (j=0; j<=ny; j++) {
    for (i=0; i<=ny; i++) {
      x = x0 + i*dxx + j*dyx;
      y = y0 + i*dxy + j*dyy;

      centering.peakRadius = 2;
      centering.peakBackground = 1500;
      centering.peakBackgroundX = 0;
      centering.peakBackgroundY = 0;

      if (centering.fitPeakNear(x,y)) {
        centering.appendPowderPoint(centering.peakCenterX,centering.peakCenterY);
        grid.push([i,j,centering.peakCenterX,centering.peakCenterY]);
      }
    }
  }
}

function dumpgrid()
{
  for(p in grid) {
    pt = grid[p]
    i = pt[0];
    j = pt[1];
    x = pt[2];
    y = pt[3];
    xx = x0 + i*dxx + j*dyx;
    yy = y0 + i*dxy + j*dyy;
    printMessage(i,j,xx,yy,x-xx,y-yy);
  }
}
