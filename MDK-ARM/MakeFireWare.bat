
:: addr_start Ӧ�ó�����ʼ��ַ

::����������ʱ���ݲ���
set NAME_PRJ=%2

::����һЩ�ļ�
set "FILE_HEX_SRC=%NAME_PRJ%.hex"
set "FILE_OUT_HEX_LEN=%NAME_PRJ%_len.hex"
set "FILE_OUT_BIN_TEMP=%NAME_PRJ%_temp.bin"
set "FILE_OUT_BIN=%NAME_PRJ%.bin"
set "FILE_OUT_HEX_BOOT=%NAME_PRJ%_With_BootLoader.hex"

::��ȡԴ�ļ�
copy /b ..\..\bootloader\MDK-ARM\Objects\FM33XX_bootload.hex BootLoader.hex
copy /b .\Release\Objects\%FILE_HEX_SRC% %FILE_HEX_SRC%

:: 1�����Դ�ļ�
srec_cat %FILE_HEX_SRC% -intel -fill 0xFF -over %FILE_HEX_SRC% -intel -o %FILE_OUT_HEX_LEN%  -intel -obs=16

:: 2��Hex to bin
srec_cat %FILE_OUT_HEX_LEN% -intel -offset -%1 -o %FILE_OUT_BIN_TEMP% -binary

:: 3�����У��:iaptool infile outfile  Add 2Byte CRC,4Byte outfile length,250Byte 0xFF at the head of infile�����������ļ�outfile
iaptool %FILE_OUT_BIN_TEMP% %FILE_OUT_BIN%

:: 4���ϲ��ļ�
srec_cat BootLoader.hex -intel %FILE_HEX_SRC% -intel -o %FILE_OUT_HEX_BOOT% -intel -obs=16

::���������ļ��������Ĺ���
copy /b %FILE_OUT_HEX_BOOT% D:\automatic\code

::���������ļ�
if not exist .\UserFile\ md .\UserFile\

::�����ļ�
copy /b %FILE_HEX_SRC% .\UserFile\%FILE_HEX_SRC%
copy /b %FILE_OUT_HEX_BOOT% .\UserFile\%FILE_OUT_HEX_BOOT% 
copy /b %FILE_OUT_BIN% .\UserFile\%FILE_OUT_BIN%

::ɾ����ʱ�ļ�
del BootLoader.hex 
del %FILE_HEX_SRC%
del %FILE_OUT_HEX_LEN%
del %FILE_OUT_BIN_TEMP%
del %FILE_OUT_BIN%
del %FILE_OUT_HEX_BOOT%




::pause
