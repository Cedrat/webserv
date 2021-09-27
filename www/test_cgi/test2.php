<?php
        //Enter your code here, enjoy!

$array = array("1" => "PHP code tester Sandbox Online",  
              "foo" => "bar", 5 , 5 => 89009, 
              "case" => "Random Stuff: " . rand(100,999),
              "PHP Version" => phpversion()
              );
              
foreach( $array as $key => $value ){
    echo $key."\t=>\t".$value."\n";
}

