<?php
   // this code will check if form is submitted using 
   // get method or not
   if($_SERVER['REQUEST_METHOD'] == "GET")
   {
      // this code will check that both name and city 
      // is entered or not
      if(isset($_GET['name']) && isset($_GET['city']))
      {
         echo "You are ";
         echo $_GET['name'];
         echo " from ";
         echo $_GET['city'];
         exit();
      }
   }
?>
<html>
<head>
   <title>PHP GET Method Example</title>
</head>
<body>
<form method="GET">
   Name: <input type="text" name="name"><br/>
   City: <input type="text" name="city"><br/>
   <input type="submit">
</form>
</body>
</html>