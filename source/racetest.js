function racetest(filename,exposure,iter) {
   for (i=0; i<iter; i++) {
      racetest_acquire(filename, exposure, i);
      racetest_acquiredark(filename, exposure, i);
   }
}

function racetest_acquire(filename, exposure, i) {
   acquire(filename, exposure, 1, 1);

status(0.0)

   if (status(0.0) == 1) {
      print("Iteration ", i, " failed")
   }

   while(status(1.0) == 0) {
   }
}

function racetest_acquiredark(filename, exposure, i) {
   acquireDark(filename, exposure, 1);

status(0.0)

   if (status(0.0) == 1) {
      print("Iteration ", i, " failed")
   }

   while(status(1.0) == 0) {
   }
}
