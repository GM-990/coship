#!/usr/bin/perl

use Getopt::Std;

##################
#  Sub Functions #
##################
sub GenTempFile
{
	my $tmp = "./tmp1";

	if ( -f $tmp ) { unlink $tmp; }
	if ( -f $tempfile ) { unlink $tempfile; }
	
	open(MAPFILE, "<$mapfile") or die "In GenTempFile --- open $mapfile : $!\n";
	open(TMPFILE, ">>$tmp") or die "In GenTempFile --- open $tmp : $!\n";

	my $count = 0;
	while(<MAPFILE>)
	{
		if ( $_ =~ /Module Contribution Map.*?/ ) { $count++; }
		if ( $count > 0 ) { print TMPFILE $_; $count++; }
	}
	
	close(MAPFILE);
	close(TMPFILE);
	
	if ( $count == 0 ) { print "File Format Error -- 1 !!\n\n"; exit 1; }
	elsif ( $opt_d )
	{
		print "Module Contribution Map Size : $count lines\n";
		print "************* First Filte Over !! ********************\n\n";
	}
	
	open(MAPFILE, "<$tmp") or die "In GenTempFile --- open $tmp : $!\n";
	open(TMPFILE, ">>$tempfile") or die "In GenTempFile --- open $tempfile : $!\n";

	$count=0;
	while(<MAPFILE>)
	{
		my $r = grep /\//, $_;
		if ( $r == 1 ) { print TMPFILE $_; $count++; }
	}

	close(MAPFILE);
	close(TMPFILE);

	if ( $count == 0 ) { print "File Format Error -- 2 !!\n\n"; exit 2; }
	elsif ( $opt_d )
	{
		print "MMCP Module Contribution Map Size : $count lines\n";
		print "************* Second Filte Over !! ********************\n\n";
	}

	if ( -f $tmp ) { unlink $tmp; }
}

sub FormatData
{
	if ( -f $formatedfile ) { unlink $formatedfile; }
	if ( -f $modulenamefile ) { unlink $modulenamefile; }

	open(TMPFILE, "<$tempfile") or die "open $tempfile : $!\n";
	open(FORMATEDFILE, ">>$formatedfile") or die "open $formatedfile : $!\n";
	open(MODULENAMEFILE, ">>$modulenamefile") or die "open $modulenamefile : $!\n";

	my @modules;
	my $i = 0;
	while(<TMPFILE>)
	{
		my @column = split / /, $_, 2;
		my $modulename = qx/basename $column[0]/;
		my $currentmodule = $modulename;
		my $newmodule = 1; # 0: false; 1: true

		for ( my $c = 0; $c < $i; $c++ ) 
		{
			chomp($currentmodule); chomp($modules[$c]); 
			if ( $currentmodule eq $modules[$c] ) { $newmodule = 0; last; } 
		}
		if ( $newmodule == 1 ) { push @modules, $modulename; print MODULENAMEFILE $modulename; $i++; }

		$column[1] =~ s#.*(def_\w+).* (\d+)#$1$2#;
		my $section = $1;
		my $size = $2;
		chomp($modulename);
		print FORMATEDFILE "$modulename $section $size\n";
	}

	close(FORMATEDFILE);
	close(MODULENAMEFILE);
	close(TMPFILE);
}

sub FormatNumber
{
	my $arg = shift;

	if ( ! $arg ) { return 0; }

	my $pf;
	my $mod;
	my $pos = 0;

	while( $arg > 1 )
	{
		$mod = $arg % 10;

		$pf = $mod.$pf;

		$arg /= 10; $pos++;
		
		if ( (!($pos % 3)) && ($arg > 1) ) { $pf = ','.$pf; }
	}

	return $pf;
}

sub CountSize
{
	if ( ! -f $modulenamefile ) { die "Module file is not exist !! \nPlease execute FormatData() to generate it !!\n"; }

	open(MODULENAMEFILE, "<$modulenamefile") or die "open $modulenamefile : $!\n";
	
	my @allmodules;
	while(<MODULENAMEFILE>) { push @allmodules, $_; }

	close(MODULENAMEFILE);

	my %size;
	$size{def_data} = 0; 
	$size{def_const} = 0; 
	$size{def_code} = 0; 
	$size{def_bss} = 0; 
	$size{total_def} = 0;
	$size{total_mod} = 0;

	foreach(@allmodules) 
	{ 
		chomp($_); 

		$size{$_} = 0;
		$size{$_.def_data} = 0; 
		$size{$_.def_const} = 0; 
		$size{$_.def_code} = 0; 
		$size{$_.def_bss} = 0; 
	}
	
	open(FORMATEDFILE, "<$formatedfile") or die "open $formatedfile : $!\n";

	if ( -f $resultfile ) { unlink $resultfile; }
	open(RESULTFILE, ">>$resultfile") or die "open $resultfile : $!\n";

	while (<FORMATEDFILE>)
	{
		my @column = split / /, $_;
		
		$size{$column[0].$column[1]} += $column[2];
		$size{$column[0]} += $column[2];
		$size{$column[1]} += $column[2];
	} 
	$size{total_def} += $size{def_data};  $size{total_def} += $size{def_code};
	$size{total_def} += $size{def_const}; $size{total_def} += $size{def_bss};

	print RESULTFILE "\nModule Nmae\t\tdef_data\tdef_code\tdef_const\tdef_bss\t\tSingal Module Size\n";
	print RESULTFILE "----------------------------------------------------------------------------------------------------------\n";
	foreach(@allmodules) 
	{ 
		chomp($_); 
	 	$size{total_mod} += $size{$_};	

		if ($opt_c)
		{
			if ( index($_, '.') < 7 ) { print RESULTFILE "$_\t\t\t"; }
			elsif ( index($_, '.') > 12 ) { print RESULTFILE "$_\t"; }
			else { print RESULTFILE "$_\t\t"; }

			print RESULTFILE FormatNumber($size{$_.def_data});
			if ( $size{$_.def_data} > 999999 ) { print RESULTFILE "\t"; } else { print RESULTFILE "\t\t"; }
			print RESULTFILE FormatNumber($size{$_.def_code});
			if ( $size{$_.def_code} > 999999 ) { print RESULTFILE "\t"; } else { print RESULTFILE "\t\t"; }
			print RESULTFILE FormatNumber($size{$_.def_const});
			if ( $size{$_.def_const} > 999999 ) { print RESULTFILE "\t"; } else { print RESULTFILE "\t\t"; }
			print RESULTFILE FormatNumber($size{$_.def_bss});
			if ( $size{$_.bss} > 999999 ) { print RESULTFILE "\t"; } else { print RESULTFILE "\t\t"; }
			print RESULTFILE FormatNumber($size{$_});
			print RESULTFILE "\n";
		}
		else
		{
			if ( index($_, '.') < 7 ) 
			{
				print RESULTFILE "$_\t\t\t$size{$_.def_data}\t\t$size{$_.def_code}\t\t$size{$_.def_const}\t\t$size{$_.def_bss}\t\t$size{$_}\n";
			}
			elsif ( index($_, '.') > 12 )
			{
				print RESULTFILE "$_\t$size{$_.def_data}\t\t$size{$_.def_code}\t\t$size{$_.def_const}\t\t$size{$_.def_bss}\t\t$size{$_}\n";
			}
			else
			{
				print RESULTFILE "$_\t\t$size{$_.def_data}\t\t$size{$_.def_code}\t\t$size{$_.def_const}\t\t$size{$_.def_bss}\t\t$size{$_}\n";
			}
		}			
	}
	print RESULTFILE "----------------------------------------------------------------------------------------------------------\n";

	if ($opt_c)
	{
		print RESULTFILE "Total\t\t\t"; 
		print RESULTFILE FormatNumber($size{$_.def_data});
		if ( $size{$_.def_data} > 999999 ) { print RESULTFILE "\t"; } else { print RESULTFILE "\t\t"; }
		print RESULTFILE FormatNumber($size{$_.def_code});
		if ( $size{$_.def_code} > 999999 ) { print RESULTFILE "\t"; } else { print RESULTFILE "\t\t"; }
		print RESULTFILE FormatNumber($size{$_.def_const});
		if ( $size{$_.def_const} > 999999 ) { print RESULTFILE "\t"; } else { print RESULTFILE "\t\t"; }
		print RESULTFILE FormatNumber($size{$_.def_bss});
		print RESULTFILE "\n\n\t\t\t\t\t\t\t\t\tMMCP TOTAL SIZE :  ";
		print RESULTFILE FormatNumber($size{total_def});
		print RESULTFILE "\n\n";
	}
	else
	{
		print RESULTFILE "Total\t\t\t$size{def_data}\t\t$size{def_code}\t\t$size{def_const}\t\t$size{def_bss}\n";
		print RESULTFILE "\n\n\t\t\t\t\t\t\tMMCP TOTAL SIZE :  $size{total_def}\n";
	}
	if ( $opt_d ) { if ( $size{total_def} == $size{total_mod} ) { print "\nOK !!\n"; } }

	close(FORMATEDFILE);
	close(RESULTFILE);
}

sub ShowUsage
{
	print "\nAnalyseSymbolMap.pl [ -f MapFileName | -o OutPutFileName | -c | -h ]\n";
	print "		-f MapFileName		; Specify map file\n";
	print "					; Use mmcp.lku.map in local directory if not give the argument\n";
	print "		-o OutPutFileName	; Specify output file\n";
	print "					; no output if not give the argument\n";
	print "		-c			; Show numbers with a format of separated by comma every three digits\n";
	print "		-h			; Show help\n\n";
}

###################
#  Main Functions #
###################
$mapfile = "./mmcp.lku.map";
$formatedfile = "./mmcp.st20.formated";
$modulenamefile = "./mmcp.st20.modulename";
$tempfile = "./mmcp.st20.temp";
$resultfile = "./analyse.result";

getopts('dcho:f:');
if ( $opt_h ) { ShowUsage(); exit 1; }; 
if ( $opt_f ) { $mapfile = $opt_f; }
if ( $opt_o ne '' ) { $resultfile = $opt_o; }

if ( ! -f $mapfile ) { die "Map file not exist !!\n"; ShowUsage(); exit 1; }

GenTempFile();
FormatData();
CountSize();

if ( ! $opt_d ) { if ( -f $tempfile ) { unlink $tempfile; } }
if ( ! $opt_d ) { if ( -f $formatedfile ) { unlink $formatedfile; } }
if ( ! $opt_d ) { if ( -f $modulenamefile ) { unlink $modulenamefile; } }

system(cat, $resultfile);

if ( ! $opt_d ) { if ( $opt_o eq '' ) { unlink $resultfile; } }

