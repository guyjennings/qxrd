<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
  <html>
  <head>
    <title>QXRD Downloads</title>
    <style type="text/css">
      <!--
      body {
      font-family: Arial, Helvetica, sans-serif;
      font-size: 75%;
      font-weight: bold;
      margin: 3;
      background-repeat: no-repeat;
      background-attachment: scroll;
      }
      a {
      font-family: Arial, Helvetica, sans-serif;
      font-size: 100%;
      font-weight: bold;
      text_decoration: none;
      }
      tr {
      font-family: Arial, Helvetica, sans-serif;
      font-size: 75%;
      font-weight: bold;
      text_decoration: none;
      }
      -->
    </style>
  </head>
  <body link="#000000" alink="#000000" vlink="#000000" bgcolor="#ffffff">
   <h1>QXRD Downloads</h1>
    <h2>Zipped Windows Executables</h2>
      <table>
       <th>File</th><th>Date</th><th>Size</th>
      <?php
       foreach(glob("qxrd*.zip") as $filename) {
          echo "<tr><td><a href=\"$filename\">$filename</a></td><td>".date("M d Y H:i", filemtime($filename))."</td><td>".filesize($filename)."</td></tr>\n";
       }
     ?>
      </table>
    <h2>Source Tarballs</h2>
      <table>
       <th>File</th><th>Date</th><th>Size</th>
     <?php
       foreach(glob("qxrd*.tar.gz") as $filename) {
          echo "<tr><td><a href=\"$filename\">$filename</a></td><td>".date("M d Y H:i", filemtime($filename))."</td><td>".filesize($filename)."</td></tr>\n";
       }
     ?>
      </table>
    <h2>RPM Packages</h2>
      <table>
       <th>File</th><th>Date</th><th>Size</th>
     <?php
       foreach(glob("qxrd*.rpm") as $filename) {
          echo "<tr><td><a href=\"$filename\">$filename</a></td><td>".date("M d Y H:i", filemtime($filename))."</td><td>".filesize($filename)."</td></tr>\n";
       }
     ?>
      </table>
  </body>
</html>
