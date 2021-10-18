#!/bin/perl

# upload a file
#$file = {'file'};

read (STDIN, $buffer, $ENV{'CONTENT_LENGTH'});

($mime_sep) = split(/\s+/,$buffer);

@fields = split(/$mime_sep/,$buffer);
foreach $input(@fields){
        ($headline,$data) = split(/\r\n\r\n/,$input,2);
        $filedata = $data if ($headline =~ $filedata)
        }

$fnsave = "/mnt/nfs/homes/dchampda/Documents/webserv/upload/".$$;
chop $filedata;
chop $filedata;
open (HERE,">$fnsave");
print HERE $filedata;


# Generate the output page
$fsz = length ($filedata);
$now = localtime();

print << "REPLYPAGE"
Content-type: text/html

<BODY BGCOLOR=white text=black>
<H2>Your page has been uploaded and saved
as $fnsave</H2>
File size is $fsz bytes  - $file<BR>
$now
</BODY>
REPLYPAGE
;