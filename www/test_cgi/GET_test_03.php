<html>
    <head>
    </head>

    <body>
        <form action="" method="GET">
        User Name:
        <input type="text" name="user" required="">
        Password:
        <input type="password" name="pass" required="">

        <input type="submit" name="submit">
        </br>
    </body>
</html>

<?php 
    if(isset($_GET['submit']))
    { 
        $user=$_GET['user'];
        $pass=$_GET['pass'];
        echo $user; 
        echo "<br>"; 
        echo $pass;
    }
?>
