print("Current Directory = ", experiment.experimentDirectory)

baddark = "baddark-00001.dark.tif";
gooddark = "gooddark-00001.dark.tif";

files = ["data-00001.tif",
         "data-00002.tif",
         "data-00003.tif",
         "data-00004.tif",
         "data-00005.tif",
         "data-00006.tif",
         "data-00007.tif",
         "data-00008.tif",
         "data-00009.tif",
         "data-00010.tif"];

for (i in files) {
  processor.loadDark(baddark);
  processor.loadData(files[i]);
  processor.unsubtractDark();
  processor.saveData("raw-"+files[i]);
  processor.loadDark(gooddark);
  processor.subtractDark();
  processor.saveData("corr-"+files[i]);
}
