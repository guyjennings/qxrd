/*
  A js script to generate an array of plot markers from a calibration grid image
  */

var grid;
//var x0 = 59.4838917636;
//var y0 = 46.0376164063;
var nx = 39;
var ny = 39;
//var dxx = 50.6669752504;
//var dxy = 0.2394249314;
//var dyx = -0.2309734292;
//var dyy = 50.6682539949;

function initGrid()
{
  if (centering.countPowderPoints() >= 3) {
    x0 = centering.getPowderPointX(0);
    y0 = centering.getPowderPointY(0);

    dxx = centering.getPowderPointX(1)-x0;
    dxy = centering.getPowderPointY(1)-y0;

    dyx = centering.getPowderPointX(2)-x0;
    dyy = centering.getPowderPointY(2)-y0;

    print("x0 = ",  x0,  " y0 = ", y0);
    print("dxx = ", dxx, " dxy = ", dxy);
    print("dyx = ", dyx, " dyy = ", dyy);
  }
}

function refineAxes()
{
  j = 0;
  for (i=2; i<=nx; i++) {
    x = x0 + i*dxx + j*dyx;
    y = y0 + i*dxy + j*dyy;

    if (centering.fitPeakNear(x,y)) {
      if (centering.peakHeight > 200) {
        xx = centering.peakCenterX;
        yy = centering.peakCenterY;
        if (Math.sqrt(Math.pow(xx-x,2)+Math.pow(yy-y,2)) < 5) {
          dxx = (xx-x0)/i;
          dxy = (yy-y0)/i;
          centering.appendPowderPoint(xx,yy);
        }
      }
    }
  }

  i = 0;
  for (j=2; j<=ny; j++) {
    x = x0 + i*dxx + j*dyx;
    y = y0 + i*dxy + j*dyy;

    if (centering.fitPeakNear(x,y)) {
      if (centering.peakHeight > 200) {
        xx = centering.peakCenterX;
        yy = centering.peakCenterY;
        if (Math.sqrt(Math.pow(xx-x,2)+Math.pow(yy-y,2)) < 5) {
          dyx = (xx-x0)/j;
          dyy = (yy-y0)/j;
          centering.appendPowderPoint(xx,yy);
        }
      }
    }
  }
}

function calGrid()
{
  grid = [];

  centering.deletePowderPoints();

  for (j=0; j<=ny; j++) {
    for (i=0; i<=nx; i++) {
      x = x0 + i*dxx + j*dyx;
      y = y0 + i*dxy + j*dyy;

      centering.peakRadius = 2;
      centering.peakBackground = 1500;
      centering.peakBackgroundX = 0;
      centering.peakBackgroundY = 0;

      if (centering.fitPeakNear(x,y)) {
        if (centering.peakHeight > 200) {
          xx = centering.peakCenterX;
          yy = centering.peakCenterY;
          if (Math.sqrt(Math.pow(xx-x,2)+Math.pow(yy-y,2)) < 5) {
            centering.appendPowderPoint(xx,yy);
            grid.push([i,j,xx,yy]);
          }
        }
      }
    }
  }
}

function dumpGrid()
{
  for(p in grid) {
    pt = grid[p]
    i = pt[0];
    j = pt[1];
    x = pt[2];
    y = pt[3];
    xx = x0 + i*dxx + j*dyx;
    yy = y0 + i*dxy + j*dyy;
    printMessage(i,j,xx,yy,x,y,x-xx,y-yy);
  }
}
