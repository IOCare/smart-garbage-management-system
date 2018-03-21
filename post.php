<?php
	error_reporting(0);
    $data = $_GET["data"];
	
	$pieces = explode(",", $data);
	if($pieces[1]!=""){
		wh_log($data);
		$timestamp = date("Y-m-d H:i:s");
		$servername = "us-west05";
		$username = "makingv1_xtrude";
		$password = "xtrudeproject1";
		$dbname = "makingv1_xtrudeDB";

		// Create connection
		$conn = new mysqli($servername, $username, $password, $dbname);
		// Check connection
		if ($conn->connect_error) {
			die("Connection failed: " . $conn->connect_error);
		} 
		
		
		$sql = "SELECT level,weight,lat,lng,time  FROM smart_tb WHERE NodeID = '".$dbID."'";
		$result =  mysqli_query($conn, $sql);
		if(mysqli_num_rows($result)< 0){
			$sql = "INSERT INTO smart_tb (nodeid) VALUES ($pieces[0])";
		}
		
		
        if($pieces[1] == "wgt"){
			$sql = "Update smart_tb SET nodeid = '$pieces[0]',weight = '$pieces[2]',time = '$timestamp' WHERE id = 1";
		}
		if($pieces[1] == "lvl"){
			$sql = "Update smart_tb SET nodeid = '$pieces[0]', level = '$pieces[2]',time = '$timestamp' WHERE id = 1";
		}
		if($pieces[1] == "pos"){
			$sql = "Update smart_tb SET nodeid = '$pieces[0]',lat = '$pieces[2]',lng = '$pieces[3]',time = '$timestamp' WHERE id = 1";
		}
		
		

		if ($conn->query($sql) === TRUE) {
			echo "New record created successfully";
		} else {
			echo "Error: " . $sql . "<br>" . $conn->error;
		}

		$conn->close();

	}

	
	//$pieces = explode(";", $data);

	//wh_log("{data:".$myJSON."}");
	
	echo "Posted";


function wh_log($log_msg)
{
    $log_filename = "log";
    if (!file_exists($log_filename)) 
    {
        // create directory/folder uploads.
        mkdir($log_filename, 0777, true);
    }
    $log_file_data = $log_filename.'/log_' . date('d-M-Y') . '.log';
    file_put_contents($log_file_data, $log_msg . "\r\n", FILE_APPEND);
}
?>