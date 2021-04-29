
:: addr_start 应用程序起始地址

::编译器调用时传递参数
set NAME_PRJ=%2

::命名一些文件
set "FILE_HEX_SRC=%NAME_PRJ%.hex"
set "FILE_OUT_HEX_LEN=%NAME_PRJ%_len.hex"
set "FILE_OUT_BIN_TEMP=%NAME_PRJ%_temp.bin"
set "FILE_OUT_BIN=%NAME_PRJ%.bin"
set "FILE_OUT_HEX_BOOT=%NAME_PRJ%_With_BootLoader.hex"

::提取源文件
copy /b ..\..\bootloader\MDK-ARM\Objects\FM33XX_bootload.hex BootLoader.hex
copy /b .\Release\Objects\%FILE_HEX_SRC% %FILE_HEX_SRC%

:: 1、填充源文件
srec_cat %FILE_HEX_SRC% -intel -fill 0xFF -over %FILE_HEX_SRC% -intel -o %FILE_OUT_HEX_LEN%  -intel -obs=16

:: 2、Hex to bin
srec_cat %FILE_OUT_HEX_LEN% -intel -offset -%1 -o %FILE_OUT_BIN_TEMP% -binary

:: 3、添加校验:iaptool infile outfile  Add 2Byte CRC,4Byte outfile length,250Byte 0xFF at the head of infile，并生成新文件outfile
iaptool %FILE_OUT_BIN_TEMP% %FILE_OUT_BIN%

:: 4、合并文件
srec_cat BootLoader.hex -intel %FILE_HEX_SRC% -intel -o %FILE_OUT_HEX_BOOT% -intel -obs=16

::复制升级文件到升级的工程
copy /b %FILE_OUT_HEX_BOOT% D:\automatic\code

::整理、清理文件
if not exist .\UserFile\ md .\UserFile\

::复制文件
copy /b %FILE_HEX_SRC% .\UserFile\%FILE_HEX_SRC%
copy /b %FILE_OUT_HEX_BOOT% .\UserFile\%FILE_OUT_HEX_BOOT% 
copy /b %FILE_OUT_BIN% .\UserFile\%FILE_OUT_BIN%

::删除临时文件
del BootLoader.hex 
del %FILE_HEX_SRC%
del %FILE_OUT_HEX_LEN%
del %FILE_OUT_BIN_TEMP%
del %FILE_OUT_BIN%
del %FILE_OUT_HEX_BOOT%




::pause
