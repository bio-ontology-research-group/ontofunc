<!DOCTYPE html
PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="en-US" xml:lang="en-US">
   <head>
   <link rel="stylesheet" type="text/css" href="/css.css" />
   <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
   </head>
   <body>
   <h2 id="right">Submit a new Job</h2>
   <script type="text/javascript">
   function xxxx () {
   Fenster = window.open("/ontofunc/instructions-submit.html", "NewWindow", "width=700,height=700,scrollbars=yes");
 }
   document.write("<p> Instructions are available <a href=\"javascript:xxxx()\">here</a>.</p>") ;
</script><noscript> <p>Instructions are available <a href="/instructions-submit.html">here</a>.</p></noscript><table> 
     <colgroup>
     <col width="350">
     <col width="500">
     </colgroup>
     <form method="post" action="submit-result.php" enctype="multipart/form-data">
     <tr><th align=left>Project Name (optional):</th><td><input type="text" name="project"  size="30" /></td></tr>
     <tr><th align=left>e-mail address (optional):</th><td> <input type="text" name="email"  size="30" /></td></tr>
     <tr><th align=left>Input File:<br><small><a href="https://func.eva.mpg.de/doc/inputfile.html">Format Description (link to FUNC website)</a></small></th><td> <input type="file" name="filename" maxlength="100000000" size="30" /></td></tr>
     <tr><th align=left>Test:</th><td> <select name="type" >
     <option value="binomial">binomial</option>
     <option value="wilcoxon">wilcoxon</option>
     <option value="hypergeometric">hypergeometric</option>
     <option value="mcdonaldkreitman">mcdonaldkreitman</option>
     </select></td></tr>
     <tr><th align="left">Ontology version:</th>
     <td>
     <select name="version">
     <?php
     
     foreach (new DirectoryIterator('/mnt/ontofunc/obodata/') as $fileInfo) {
     if($fileInfo->isDot()) continue;
     $dirname = $fileInfo->getFilename() ;
     echo "<option value=\"$dirname\">$dirname</option>";
   }
     ?>
     </select>
     </td>
     </tr>
     <tr><th align=left>Ontology:<br><small><a href="ontohelp.html">Explanation</a></small></th><td> <select name="ontology" >
     <?php
     $fp = fopen("/mnt/ontofunc/obodata/02-Apr-2013/obo-index.txt", "r") ;
while (($ids = fgetcsv($fp, 1000, "\t")) !== FALSE) {
  echo "<option value=\"".$ids[0]."\">".$ids[1]." (". $ids[2] .")</option>";
}

?>
</select></td></tr>
<tr><th align=left>Cutoff for number of genes/group:</th><td> <input type="text" name="cutoff" value="20" size="30" /></td></tr>
  <tr><th align=left>Number of random sets:</th><td> <select name="randsets" >
  <option value="1000">1000</option>
  <option value="5000">5000</option>
  <option value="10000">10000</option>
  <option value="20000">20000</option>
  </select></td></tr>
  <tr><th align=left>Root node:</th><td> <input name="rootnode" type="text" value="owl:Thing" /></td></tr>
				       <tr><td bgcolor="lightgreen" align=right><input type="reset"  name=".reset" /></td><td align=left bgcolor="lightgreen"><input type="submit" name="submit" value="Process File" /></td></tr></table></form>
				       </body>
				       </html>
