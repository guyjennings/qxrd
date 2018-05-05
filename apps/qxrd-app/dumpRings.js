function dumpRings()
{
   var dataFile = data().fileBase;

   print("Saving settings")

   fopen(dataFile+".fitParms")
   fprint("# fitting settings for ", dataFile, "\n")
   fprint("centering.centerX = ", centering.centerX, "\n")
   fprint("centering.centerY = ", centering.centerY, "\n")
   fprint("centering.detectorDistance = ", centering.detectorDistance, "\n")
   fprint("centering.detectorTilt = ", centering.detectorTilt, "\n")
   fprint("centering.tiltPlaneRotation = ", centering.tiltPlaneRotation, "\n")
   fprint("centering.energy = ", centering.energy, "\n")
   fclose()

   print("Starting saving marked points")
   fopen(dataFile+".fitData")
   fprint("# fitted points for ", dataFile, "\n")

   var pts = centering.markedPoints
   for (i in pts) {
      fprint(i, pts[i], "\n")
   }
   fclose();

   print("Starting saving fitted rings")
   fopen(dataFile+".fitRings")
   fprint("# fitted rings for ", dataFile, "\n")

   var rings = centering.fittedRings
   for (i in rings) {
      fprint(i, rings[i], "\n")
   }
   fclose();

   print("Finished")
}
