#! /usr/bin/perl

read (STDIN, $buffer, $ENV{'CONTENT_LENGTH'});
@pairs = split(/&/, $buffer);
foreach $pair (@pairs) 
{
    ($name, $value) = split(/=/, $pair);
    $value =~ tr/+/ /;
    $value =~ s/%([a-fA-F0-9] [a-fA-F0-9])/pack("C", hex($1))/eg;
    $value =~ s/~!/ ~!/g;
    $FORM{$name} = $value;
}
   
if($FORM{python}) 
{
   $python_flag ="YES";
} 
else 
{
   $python_flag ="NO";
}
   
if($FORM{java}) 
{
   $java_flag ="YES";
}
else 
{
   $java_flag ="NO";
}
   
if($FORM{kotlin})
{
   $kotlin_flag ="YES";
} 
else
{
   $kotlin_flag ="NO";
}
   
if($FORM{perl}) 
{
   $perl_flag ="YES";
} 
else 
{
   $perl_flag ="NO";
}
   
if($FORM{swift}) 
{
   $swift_flag ="YES";
} 
else
{
   $swift_flag ="NO";
}
   
$first_name= $FORM{'first_name'};
$last_name= $FORM{'last_name'};
$payment_method= $FORM{'payment'};
$first_time= $FORM{'first_time'};
$feed_back= $FORM{'feedback'};
   
print "Content-type:text/html\r\n\r\n";
print "<html>";
print "<head>";
print "<title>GeeksForGeeks - Post Method</title>";
print "</head>";
print "<body>";
print "<h3>Hello $first_name $last_name</h3>";
print "<h3>Here is your Purchased Order!</h3>";
print "<h3>Python: $python_flag</h3>";
print "<h3>Java: $java_flag</h3>";
print "<h3>Kotlin: $kotlin_flag</h3>";
print "<h3>Perl: $perl_flag</h3>";
print "<h3>Swift: $swift_flag</h3>";
print "<h3>Payment Method: $payment_method</h3>";
print "<h3>First Time Customer: $first_time</h3>";
print "<h3>Feedback: $feed_back</h3>";
print "</body>";
print "</html>";
   
1;