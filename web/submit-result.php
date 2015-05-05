<html>
<head>
<link rel="stylesheet" type="text/css" href="/css.css" />
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
</head>
<body>
<p>
<?php

$uploaddir = '/mnt/ontofunc/data/';


$mysqli = new mysqli("p:localhost", $_USERNAME, $_PASSWORD, "ontofunc");

$name = htmlspecialchars($_POST["project"], ENT_QUOTES);
$email = htmlspecialchars($_POST["email"], ENT_QUOTES);
$test = htmlspecialchars($_POST["type"], ENT_QUOTES);
$ontology = htmlspecialchars($_POST["ontology"], ENT_QUOTES);
$cutoff = htmlspecialchars($_POST["cutoff"], ENT_QUOTES);
$randomsets = htmlspecialchars($_POST["randsets"], ENT_QUOTES);
$rootnode = htmlspecialchars($_POST["rootnode"], ENT_QUOTES);
$version = htmlspecialchars($_POST["version"], ENT_QUOTES);

$query = "INSERT INTO Job VALUES (NULL, '$name', '$email', '$test', '$ontology', '$version', '$cutoff', '$randomsets', '$rootnode', 0, '', '', -1)";
$mysqli->query($query);

$jobid = $mysqli->insert_id ;

mkdir($uploaddir . $jobid) ;
$uploadfile = $uploaddir . $jobid . "/input.txt" ;
if (move_uploaded_file($_FILES['filename']['tmp_name'], $uploadfile)) {
  echo "JobID is $jobid. To check your status, use this link: <a href=\"http://phenomebrowser.net/ontofunc/checkstatus.php?id=$jobid\">http://phenomebrowser.net/ontofunc/checkstatus.php?id=$jobid</a>";
} else {
  echo "Could not move file to tmp directory!\n";
  $query = "DELETE FROM Job WHERE id=$jobid";
  //  $mysqli->query($query);

}

?>
</p>
</body>
</html>
