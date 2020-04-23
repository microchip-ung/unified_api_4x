# Copyright (c) 2004-2018 Microsemi Corporation "Microsemi".
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

# Test File to read in config info

use Time::localtime;
my $year=localtime->year + 1900;

# pass in working directory and block name from excel
my $workdir = @ARGV[0];
my $outdir = @ARGV[1];

# build up file names from this
my $importfile = $workdir . "block_states" . ".txt";
my $cfilename = $outdir . "vtss_daytona_reg_init_" . "block_states" . ".inc";
my $hfilename = $outdir . "vtss_daytona_reg_init_" . "block_states" . ".h";

my $copyright = <<"EOT"
/*
 Copyright (c) 2004-$year Microsemi Corporation "Microsemi".

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
*/
EOT
;

# open output files to blow away old data
open (CFH, ">", $cfilename) or die "Cannot open $cfilename for writing";
print CFH "$copyright\n";
close CFH;

open (HFH, ">", $hfilename) or die "Cannot open $cfilename for writing";
print HFH "$copyright\n";
print HFH "#ifndef _VTSS_DAYTONA_REG_INIT_BLOCK_STATES_H" . "\n";
print HFH "#define _VTSS_DAYTONA_REG_INIT_BLOCK_STATES_H" . "\n\n";
close HFH;

# change based on the format of the config spreadhseet
my $nondatacols = 2;

# var for debug printing
my $debug = 0;

# initialize local variables
my @blocks = ();
my @block_modes = ();
my $line = 0;
#my $block = "";
(-e $importfile) or die "File $importfile does not exist!";

# open file to read from
open (FH, "<", $importfile);

print "\nCreating config mode table \n";
stdouthot(); # make stdout hot

open (CFH, ">>", $cfilename) or die "Cannot open $cfilename for writing";
print CFH "static const u8 block_mode[MODE_LAST][BLOCK_LAST] = {\n";

open (HFH, ">>", $hfilename) or die "Cannot open $cfilename for writing";
print HFH "typedef enum {\n";

while (<FH>) {
	my @data  = split("\t", $_);
	# remove any extra tabs at the end of the line
	while (($data[$#data] eq "\n" | $data[$#data] eq "") && $#data >= 1) { pop @data; }
	$dl = @data;
	#debprint "(header test) line " . $line . "data length is" . $dl . "\n";
	if ($line == 0) { ######### Line 0 - Block Name
		# pop first few columns off of data
		for (my $j=0; $j<$nondatacols; $j++) { shift @data; };
		# add block names to list
		#debprint "(header test) in case 1 (line" . $line . "), data is " . @data . "\n";
		foreach $block (@data) {
            if ($block eq "cxfi") {$block = "xfi_1"}
            if ($block eq "lxfi") {$block = "xfi_2"}
            if ($block eq "cxfi_hss11g") {$block = "xfi_hss11g_1"}
            if ($block eq "lxfi_hss11g") {$block = "xfi_hss11g_2"}
            if ( ($block =~ /si/) || ($block =~ /sy/) ) {} else {
                push(@block_modes, "typedef enum {\n");
                print HFH "    BLOCK_" . uc($block) . ",\n";
            }
			$block =~ s/_1|_2//g; # remove block number, don't care as they should be the same
			push(@blocks, $block);
		}
        print HFH "    BLOCK_LAST\n} block_name_t;\n\n";
	} else {
		my $mode = shift @data;
        if ($line == 1) {
            print HFH "typedef enum {\n";
        }
        print HFH "    MODE_" . uc($mode) . ",\n";
        print CFH "{";

		# remove unused mode names (which are in the first few columns)
		for (my $j=1; $j<$nondatacols; $j++) { shift @data; };
		# check that we still have some data, this filters out empty lines
		$dl = @data;
		if ($dl >= 1 && $data[0] ne "\n") {
            my $i = 0;
            my $seen_blocks = "";
			foreach $block_mode (@data) {
                if ( ($blocks[$i] =~ /si/) || ($blocks[$i] =~ /sy/) ) {} else {
                    $bm = " (u8)BM_" . uc($blocks[$i]) . "_" . uc($block_mode);
                    print CFH $bm . ",";    #Add to block_mode table
                    if ($seen_blocks =~ $blocks[$i]) {      # Check if this block is already seen
                        $block_modes[$i] = "";              #Only have one block mode enum for each block
                        for ($j=0; ($blocks[$j] ne $blocks[$i]); $j++) {}; #The block mode is added to the first find of this block
                        if ($block_modes[$j] =~ $bm) {} else {
                            $block_modes[$j] = $block_modes[$j] .  "   " . $bm . ",\n";
                        }
                    } else {
                        $seen_blocks = $seen_blocks . " " . $blocks[$i];    #Now this block is seen
                        if ($block_modes[$i] =~ $bm) {} else {
#if ($line == 2)  {print "seen_blocks " . $seen_blocks . "   i " . $i . "\n";}
#if ($line == 2)  {print "clear \n";}
#if (($i >= 17) && ($i <= 22))  {print "add  i " . $i . " line " . $line . "\n";}
                            $block_modes[$i] = $block_modes[$i] .  "   " . $bm . ",\n";
                        }
                    }
                }
                $i++;
			}
            print CFH "},\n";
		}
	}
	print ".";
	$line++;
}

print HFH "    MODE_LAST\n} mode_name_t;\n\n";
print HFH "#endif /* _VTSS_DAYTONA_REG_INIT_BLOCK_STATES_H */" . "\n";

#$i = 0;
#foreach $block_mode (@block_modes) {
#    if ($block_mode ne "")   {print HFH $block_mode . "    BM_" . uc($blocks[$i]) . "_LAST\n} block_" . $blocks[$i] . "_mode_t;\n\n";}
#    $i++;
#}

close HFH;
print CFH "};\n\n";
close CFH;
close FH;

print "\nComplete!\n";


# calls print only if the global variable debugp is set
sub debprint {
  my $string = shift;
  if ( $debug == 1 ) { print $string; }
}

# makes stdout "hot", or unbuffered, so we can show progress
sub stdouthot {
	my $ofh = select STDOUT;
	  $| = 1;
	  select $ofh;
}
