

#! /usr/bin/perl
print "Content-type: text/html\n\n";

if ($ENV{"REQUEST_METHOD"} eq "POST") {
  read(STDIN, $datastring, $ENV{"CONTENT_LENGTH"});		
}
elsif (exists $ENV{"REQUEST_METHOD"}) {		# data from GET transaction (or HEAD or other)
  $datastring = $ENV{"QUERY_STRING"};
}
else {
  print "Offline execution detected\n";
  print "Please enter some data.\n";
  $datastring = <>;

  chomp $datastring;
  print "== data accepted == HTML output follows ==\n\n";
}

###decode######################################################
$datastring =~s/%0D%0A/\n/g;                    			#step to deal with line
																											#breaks in text areas
@nameValuePairs = split(/&/, $datastring);						#step 1
foreach $pair (@nameValuePairs) {
  ($name, $value) = split(/=/, $pair);								#step 2
  
  $name =~tr/+/ /;                                 		#step 3
  $name =~s/%([\da-fA-F]{2})/pack("C",hex($1))/eg; 		#step 3
  $value =~tr/+/ /;                                		#step 3
  $value =~s/%([\da-fA-F]{2})/pack("C",hex($1))/eg;		#step 3
  
  if(exists $formHash{$name}) {												#improved step 4,
    $formHash{$name} = $formHash{$name}.";".$value;		#now handles multiple
  }																										#select menus
  else {
    $formHash{$name} = $value;
  }   	
}
###done decoding###############################################

$dataDir = "";  # data in same directory as this program

### the data source #######################################
@sizes = ("large", "medium", "small");
$default_size="medium";

%size_prices = (
  "large" => "8.00",
  "medium" => "6.00",
  "small" => "4.00"
);
%topping_prices = (
  "large" => "1.00",
  "medium" => "0.75",
  "small" => "0.50"
);
%toppings = (
  "m_pepperoni" => "Pepperoni",
  "m_sausage" => "Italian Sausage",
  "v_peppers" => "Green Bell Peppers",
  "v_mushrooms" => "Mushrooms",
  "v_onions" => "Vidallia Onions",
  "v_olives" => "Black Olives"
);
### end of data source ####################################


### app logic #############################################
if($formHash{"request"} eq "confirmation_page") {
	&confirmation_page;
}
elsif($formHash{"request"} eq "confirm_order") {
	&confirm_order;
}
else {
	&print_form;
}
### end app logic #########################################



#################################################################
sub print_form {
 print<<TOP;

 <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
     "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
 <html xmlns="http://www.w3.org/1999/xhtml">
  <head>
   <meta http-equiv="content-type" content="text/html;charset=utf-8"/>
   <title>Personalize a pizza</title></head>
   <body>
   <form action="$ENV{'SCRIPT_NAME'}" method="get" >
   <table border="1">
    <tr><th>Select size</th><th>Select toppings</th></tr>
    <tr><td>
TOP

 foreach $size (@sizes) {
   print '<input type="radio" name="chosen_size" value="', $size, '"';

   if($size eq $default_size) {
     print ' checked="checked"';
   }

   print "/><b>$size</b><br/>Base Price: \$$size_prices{$size} <br/>",
     "Each topping: \$$topping_prices{$size}<br/>\n";
 }
 print " </td><td>\n";

 foreach $topping (sort keys %toppings) {
  print '<input type="checkbox" name="', $topping, '" value="yes"/>',
    "$toppings{$topping}<br/>\n";
 }
 
 print<<BOTTOM;

    </td></tr>
   </table>
   <p><input type="hidden" name="request" value="confirmation_page"/>
   <input type="submit" value="Calculate total"/></p>
   </form>
  </body>
 </html>
BOTTOM
}

#################################################################
sub confirmation_page {
 print <<TOP;

 <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
     "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
 <html xmlns="http://www.w3.org/1999/xhtml">
  <head>
   <meta http-equiv="content-type" content="text/html;charset=utf-8"/>
   <title>Calculating cost of your pizza</title></head>
   <body>
TOP

 $size = $formHash{"chosen_size"};
 $total=$size_prices{$size};
 print "You have selected a <b>$size</b> pizza with the following extra toppings:\n<ul>";

 foreach $topping (sort keys %toppings) {
  if(exists $formHash{$topping}) {
    $total = $total + $topping_prices{$size};
    print "<li><b>$toppings{$topping}</b></li>\n";
  }
 }
 print "</ul>\n";

 $total = sprintf "%.2f", $total;
 print "for a total of <b>\$$total</b>.\n";

 print<<CONFIRM;

   Please give the following information to confirm order.<br/>
   <form action="$ENV{'SCRIPT_NAME'}" method="get">
    Name: <input type="text" name="name" value="" size="20"/><br/>
    Phone: <input type="text" name="phone" value="" size="20"/><br/>
    <input type="hidden" name="chosen_size" value="$size"/>
    <input type="hidden" name="total" value="$total"/>
    <input type="hidden" name="request" value="confirm_order"/>
CONFIRM

 foreach $topping (sort keys %toppings) {
  if($formHash{$topping} eq "yes") {
    print '   <input type="hidden" name="', $topping, '" value="yes"/>', "\n";
  }
 }

 print<<BOTTOM;

    <input type="submit" value="Confirm Order"/>
   </form>
   </body>
  </html>
BOTTOM
}

#################################################################
sub confirm_order {

 $size = $formHash{"chosen_size"};
 $total= $formHash{"total"};

 open(ORDERFILE, ">>$dataDir"."order.txt") or &errorPage;
 print ORDERFILE "name=", $formHash{"name"}, "\n";
 print ORDERFILE "phone=", $formHash{"phone"}, "\n";
 print ORDERFILE "total=", $formHash{"total"}, "\n";
 print ORDERFILE "size=", $formHash{"chosen_size"}, "\n";
 foreach $topping (sort keys %toppings) {
  if(exists $formHash{$topping}) {
    print ORDERFILE $toppings{$topping}, "\n";
  }
 }
 print ORDERFILE "---------------------------------------\n";
 close(ORDERFILE);
 
 print <<TOP;

 <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
     "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
 <html xmlns="http://www.w3.org/1999/xhtml">
  <head>
   <meta http-equiv="content-type" content="text/html;charset=utf-8"/>
   <title>Thank you for your order</title></head>
   <body>
    Thank you $formHash{"name"} (phone $formHash{"phone"}).
       We have processed your order of a <b>$size</b> pizza
       with the following extra toppings:<ul>
TOP

 foreach $topping (sort keys %toppings) {
  if(exists $formHash{$topping}) {
    print "<li><b>$toppings{$topping}</b></li>\n";
  }
 }


 print<<BOTTOM;

      </ul>
 for a total of <b>\$$total</b>.
   </body>
  </html>
BOTTOM
}

#################################################################
sub errorPage {
 my $message = $_[0];  # optional message parameter
 
 print<<ALL;

 <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
     "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
 <html xmlns="http://www.w3.org/1999/xhtml">
  <head>
   <meta http-equiv="content-type" content="text/html;charset=utf-8"/>
 <title>Server Error</title></head><body>
  <h2>Server Error Encountered</h2>
  $message 
  
  If the problem persists, please notify the <a href="mailto:admin\@uweb.edu">webmaster</a>.
 </body></html>
ALL
 
exit;   # terminate program since failure to open data file
}

