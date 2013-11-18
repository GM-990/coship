a_file=`ls *.a main.out `
MEMXLS=libmemory.csv
rm $MEMXLS -f
for v in $a_file
do
  echo -n $v   >>$MEMXLS ;    objdump  -h  $v | awk '/.bss/ { count += strtonum("0x"$3)}'' END {print  ", .bss , " count}'  >>$MEMXLS
  echo -n $v   >>$MEMXLS ;    objdump  -h  $v | awk '/.text/ { count += strtonum("0x"$3)}'' END {print  ", .text , " count}'  >>$MEMXLS
  echo -n $v   >>$MEMXLS ;    objdump  -h  $v | awk '/.data/ { count += strtonum("0x"$3)}'' END {print  ", .data , " count}'  >>$MEMXLS
  echo -n $v   >>$MEMXLS ;    objdump  -h  $v | awk '/.reginfo/ { count += strtonum("0x"$3)}'' END {print  ", .reginfo , " count}'  >>$MEMXLS
  echo -n $v   >>$MEMXLS ;    objdump  -h  $v | awk '/.pdr/ { count += strtonum("0x"$3)}'' END {print  ", .pdr , " count}'  >>$MEMXLS
  echo -n $v   >>$MEMXLS ;    objdump  -h  $v | awk '/.mdebug.abi32/ { count += strtonum("0x"$3)}'' END {print  ", .mdebug.abi32 , " count}'  >>$MEMXLS
  echo -n $v   >>$MEMXLS ;    objdump  -h  $v | awk '/.rodata.str1.4/ { count += strtonum("0x"$3)}'' END {print  ", .rodata.str1.4 , " count}'  >>$MEMXLS
  echo -n $v   >>$MEMXLS ;    objdump  -h  $v | awk '/.comment/ { count += strtonum("0x"$3)}'' END {print  ", .comment , " count}'  >>$MEMXLS

done
