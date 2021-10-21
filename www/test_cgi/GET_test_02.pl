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


### the data source #######################################
@quiz_question = (
  "How many hairs are there on Homer Simpson's head?",
  "What planet is Luke Skywalker from?",
  "What is the last digit if you multiply 3 by itself a million times?"
);
@quiz_choices = (
  "0;1;2;3;4",
  "Alderaan;Bespin;Corellia;Dantooine;Tatooine",
  "1;3;5;7;9"
);
@quiz_answer = (
  "2",
  "Tatooine",
  "1"
);
### end data source #######################################

### app logic #############################################
if($formHash{"request"} eq "begin_quiz") {
  &begin_quiz;
}
elsif($formHash{"request"} eq "grade_question") {
  &grade_question;
}
else {
  &welcome_page;
}
### end app logic #########################################


#################################################################
sub welcome_page {
 	print <<PAGE;
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
     <head>
      <meta http-equiv="content-type" content="text/html;charset=utf-8"/>
 	 <title>Trivia Quiz</title></head>
  <body>
    <h2>Welcome to the Trivia Quiz</h2>
	<p>You will be given a series of questions.</p>
    <form action="$ENV{'SCRIPT_NAME'}" method="get">
     <p><input type="hidden" name="request" value="begin_quiz"/>
     <input type="submit" value="Begin Quiz"/></p>
    </form>
  </body>
</html>
PAGE
}

#################################################################
sub begin_quiz {
 	print <<TOP;

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
     <head>
      <meta http-equiv="content-type" content="text/html;charset=utf-8"/>
 	<title>Trivia Quiz</title></head>
 	<body>
      <p>Here is your first question.</p>
TOP

	&print_question(1, 0);

 	print <<BOTTOM;

    </body>
</html>
BOTTOM
}

#################################################################
sub print_question {
  my ($qnumber, $correct) = @_;
  my $index = $qnumber-1;
  my @choices = split(/;/, $quiz_choices[$index]);
  
  print<<QUESTION;

  <form action = "$ENV{'SCRIPT_NAME'}" method="get">
  <p>$qnumber. $quiz_question[$index]</p>
  <p>
QUESTION
  foreach $answer (@choices) {
     print '<input type="radio" name="answer" value="', "$answer\" />$answer";
  }
  print<<FORM;

  </p>
   <p>
       <input type="hidden" name="qnumber" value="$qnumber"/>
       <input type="hidden" name="correct" value="$correct"/>
       <input type="hidden" name="request" value="grade_question"/>
       <input type="submit" value="Submit answer"/>
   </p>
   </form>
FORM
}


#################################################################
sub grade_question {
	print<<TOP;

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
     <head>
      <meta http-equiv="content-type" content="text/html;charset=utf-8"/>
	<title>Trivia Quiz</title></head><body>
TOP

  my $qnumber = $formHash{"qnumber"};
  my $correct = $formHash{"correct"};
  my $index = $qnumber - 1;
 
  if($formHash{"answer"} eq $quiz_answer[$index]) {
     print "Your answer of $formHash{'answer'} is CORRECT.<br />\n";
     $correct++;
  }
  else {
     print "Sorry, your answer of $formHash{'answer'} is INCORRECT.<br />\n";
  }
 
  $qnumber++; 	# Either way, the question has been answered.
  
  if($qnumber > scalar @quiz_question) {
    print "Your final score is $correct correct out of ", $qnumber-1, ".\n",
          "Thank you for playing.<br />\n",
          "<a href=\"$ENV{'SCRIPT_NAME'}?request=begin_quiz\">To play again</a>";
  }
  else {
    print "Your score so far is $correct correct out of ", $qnumber-1, ".\n",
          "Here is your next question.\n"; 
    &print_question($qnumber, $correct);
  }
  print<<BOTTOM;

  </body>
</html>
BOTTOM
}


