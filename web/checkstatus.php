<html>
<head>
<link rel="stylesheet" type="text/css" href="/css.css" />
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
</head>
<body>
<?php
   $id = $_GET["id"];
?>
<p>
<form id="right" method="get" action="checkstatus.php">
<?php
   echo "Id:<input type=text name=id value=\"$id\" size=44><input type=submit>";
?>
</form></p>

<?php

$downloaddir = '/mnt/ontofunc/dataout/';

if ($id==null) {
  //echo "Job not found.";
} else {
  $mysqli = new mysqli("p:localhost", $_USERNAME, $_PASSWORD, "ontofunc");
  
  $query = "SELECT * FROM Job WHERE id=$id";
  $res = $mysqli->query($query);
  if ($res->num_rows == 0) {
    echo "Job not found.";
  } else {
    $row = $res->fetch_assoc();
    $status = $row["status"];
    ?>
    <table>
     <colgroup>
     <col width="50">
     <col width="250">
     <col width="500">
     <col width="100">
     </colgroup>
       <tr>
       <th align="left">ID</th><th align="left">Name</th><th align="left">Status</th><th>Stat file</th><th>Groups file</th>
       </tr>
       <tr>
       <?php
       echo "<td>".$row["id"]."</td><td>".$row["name"]."</td><td>" ;
    if ($status == 0) {
      echo "Not started";
    } else if ($status == 1) {
      echo "Running";
    } else if ($status == 2) {
      echo "Finished successfully";
    } else {
      echo "Error: ";

      if ($row["errorcode"]==1) {
	echo "Input file not formatted correctly at line " . $row["encountered"] . " (too many columns): <tt>" . $row["errorline"] ."</tt>";
      } else if ($row["errorcode"]==2) {
	echo "Input file not formatted correctly at line " . $row["encountered"] . " (incorrect attribute value): <tt>" . $row["errorline"] ."</tt>";	
      } else {
	echo "unknown error";
      }
    }
       ?>
    </td><td>
	<?php
	if ($status == 2) {
	  echo "<a href=\"download.php?id=$id&type=stat\">statistics.txt</a>";
	}
	?>
	</td>
	    <td>
	<?php
	if ($status == 2) {
	  echo "<a href=\"download.php?id=$id&type=group\">groups.txt</a>";
	}
	?>

	    </td>
       </tr>
       </table>
<?php

	}
}

?>

</p>
</body>
</html>