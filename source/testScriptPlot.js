processor.newOutputScan("testing")
for (x=0; x<10; x+= 0.1) {
  processor.appendToOutputScan(x, Math.sin(x))
  processor.plotOutputScan()
}
