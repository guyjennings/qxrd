dark1 = ""
dark2 = ""
files = []

for (i in files) {
  processor.loadDark(dark1);
  processor.loadData(files[i]);
  processor.unsubtractDark();
  processor.loadDark(dark2);
  processor.subtractDark();
  processor.saveData("corr-"+files[i]);
}
