for (j=0; j<10; j++) {
  i0 = acquisition.test
  for (i=0; i<1000; i++) {
    acquisition.test++
  }
  print("Loop",j, " delta = ", acquisition.test - i0)
}

for (j=0; j<10; j++) {
  i0 = acquisition.fileIndex
  for (i=0; i<1000; i++) {
    acquisition.fileIndex++
  }
  print("Loop",j, " delta = ", acquisition.fileIndex - i0)
}

