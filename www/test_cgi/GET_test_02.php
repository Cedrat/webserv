<html>
    <head>
        <title>get_browser1</title>
        <?php
            error_reporting(1);
            $x1=$_GET['f1'];
            $y1=$_GET['s1'];
            $z1=$x1+$y1;
            echo "Sum of the two numbers = ".$z1;
        ?>
    </head>

    <body bgcolor="skyblue">
        <form method="GET" >
            <table border="2" bgcolor="green">
                <tr>
                <td>Enter the first number for input :: </td>
                <td><input type="text" name="f1"/></td>
                </tr>
                <tr>
                <td>Enter the second number for input :: </td>
                <td><input type="text" name="s1"/></td>
                </tr>
                <tr align="center">
                <td colspan="3" >
                <input type="submit" value="+"/></td>
                </tr>
            </table>
        </form>
    </body>
</html>