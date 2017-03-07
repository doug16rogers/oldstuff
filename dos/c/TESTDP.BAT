@echo off

if %1$==$ goto no_file

echo Printing %1 ........................................................

capture /create=fun.prn
w:\rogers\util\4print -s -u %1 -hFile:_$fn____$fd_$ft//Printed:_$td_$tt______Page_$pn_/_$pp
copy fun.prn fun.prt
repl -v "\e%%-12345X" -n fun.prn
repl -v "@PJL ENTER LANGUAGE=PCL\n" -n fun.prn
repl -v "\e&l0O\e&l1S\eE\e&l1O" "\e&l1o1S" fun.prn
repl -v "\e&l0O\e&l2S\eE\e&l1O" "\e&l1o1S" fun.prn
repl -v "\eEX" -n fun.prn
REM nprint fun.prn /j=hpiiisi_pcl

goto end


:no_file
echo Please specify a file to print.
goto end

:end

