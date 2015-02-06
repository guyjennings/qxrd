function findring(x, y) {
  centering.peakCenterX     = x;
  centering.peakCenterY     = y;
  centering.peakBackground  = 3500;
  centering.peakBackgroundX = 0;
  centering.peakBackgroundY = 0;
  centering.peakWidth       = 2;
  centering.peakHeight      = 15800;

  if (centering.fitRingNear(x,y, 25)) {
    centering.appendPowderPoint(centering.peakCenterX, centering.peakCenterY);
  }
}

function nextring() {
  findring(centering.peakCenterX, centering.peakCenterY)
}
