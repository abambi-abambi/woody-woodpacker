# ELF

https://man7.org/linux/man-pages/man5/elf.5.html
https://ru.manpages.org/elf/5

https://habr.com/ru/post/480642/
"Введение в ELF-файлы в Linux: понимание и анализ"

"ELF — это сокращение от Executable and Linkable Format (формат исполняемых и связываемых файлов) и определяет структуру бинарных файлов, библиотек, и файлов ядра (core files). Спецификация формата позволяет операционной системе корректно интерпретировать содержащиеся в файле машинные команды. Файл ELF, как правило, является выходным файлом компилятора или линкера и имеет двоичный формат. С помощью подходящих инструментов он может быть проанализирован и изучен."

"возможность изучения ELF-файлов может быть ценна для поиска дыр в безопасности и обнаружения подозрительных файлов."

"зачем изучать ELF?

    Для общего понимания работы операционной системы
    Для разработки ПО
    Цифровая криминалистика и реагирование на инциденты (DFIR)
    Исследование вредоносных программ (анализ бинарных файлов)"



ELF-файл состоит из:

-    заголовка ELF
-    данных



В терминале: readelf path_to/your_file
(покажет список флагов, повторить с соответствующим флагом): readelf -your_flag path_to/your_file


Например:
readelf -h /usr/bin/ls
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00 
и далее.

readelf -t /usr/bin/ls
readelf -S /usr/bin/ls
readelf -l /usr/bin/ls
readelf --help

readelf -l ./woody


diff <(xxd /usr/bin/ls) <(xxd ./woody)


1) Первые 4 байта определяют, что это ELF-файл:
Сначала значение 7f, потом 45 = E, 4c = L, 46 = F

2) 5-й байт - это поле класса.
Это значение означает архитектуру, для которой предназначен файл. Оно может равняться 01 (32-битная архитектура) или 02 (64-битная).

3) 6-й байт - это поле данных о little or big-endian (?):
Два варианта: 01 — little-endian, либо 02 — big-endian. Эффект (?) становится видимым при использовании утилиты hexdump на бинарном файле: hexdump -n 16 /usr/bin/ls (также можно посмотреть на заголовок elf)

4) 7-й байт - номер версии.
В настоящее время имеется только версия 01.

5) 8-й байт - поле OS/ABI.
Порядок вызова функции определяется «двоичным интерфейсом приложения» Application Binary Interface (ABI). Поля OS/ABI описывают, какой ABI используется, и его версию. В нашем случае, значение равно 00, это означает, что специфические расширения не используются.

6) 9-й байт - версия ABI

7) 10-й байт - ожидаемый тип машины (AMD64, например).

8) 17-й байт - поле типа. Указывает, для чего предназначен файл. Вот несколько часто встречающихся типов файлов.

CORE (значение 4)
DYN (Shared object file), библиотека (значение 3)
EXEC (Executable file), исполняемый файл (значение 2)
REL (Relocatable file), файл до линковки (значение 1)

https://www.stackfinder.ru/questions/17029206/searching-init-section-in-elf-file
https://question-it.com/questions/9972862/poisk-razdela-init-v-fajle-elf
Поиск раздела .init в файле ELF

https://docs.oracle.com/cd/E53394_01/html/E54833/elf-23207.html#scrolltoc

https://habr.com/ru/post/199490/
Практическая информация по созданию файла elf
"Секции содержат всю информацию в файле (программа, данные, отладочная информация и т.д)
У каждой секции есть тип, имя и другие параметры. В секции ".text" обычно хранится код, в ".symtab" — таблица символов программы (имена файлов, процедур и переменных), в ".strtab" — таблица строк, в секциях с префиксом ".debug_" — отладочная информация и т.д. Кроме того, в файле должна обязательно быть пустая секция с индексом 0."

https://tech-geek.ru/elf-files-linux/
"e_shstrndx — это поле содер­жит индекс (то есть поряд­ковый номер в раз­деле заголов­ков сек­ций) заголов­ка одной из сек­ций, которая называ­ется .shstrtab. Эта сек­ция содер­жит име­на всех осталь­ных сек­ций ELF-фай­ла в кодиров­ке ASCII с завер­шающим нулем."

# Википедия, хорошие таблицы:
https://ru.wikipedia.org/wiki/Executable_and_Linkable_Format
"Заголовок имеет размер 52 байта для 32-битных файлов или 64 для 64-битных. Данное различие обуславливается тем, что в заголовке файла содержится три поля, имеющих размер указателя, который составляет 4 и 8 байт для 32- и 64-битных процессоров соответственно. Такими полями являются e_entry, e_phoff и e_shoff."

# PE
http://codenet.ru/progr/formt/elf_pe.php
Формат PE

# Утилиты

https://ru.wikipedia.org/wiki/Executable_and_Linkable_Format#%D0%A1%D0%BE%D0%B4%D0%B5%D1%80%D0%B6%D0%B8%D0%BC%D0%BE%D0%B5_%D1%81%D0%B5%D0%BA%D1%86%D0%B8%D0%B9_%D0%B8_%D1%81%D0%B5%D0%B3%D0%BC%D0%B5%D0%BD%D1%82%D0%BE%D0%B2
"Существует множество утилит для работы с файлами формата ELF, основные из них содержатся в наборе программных инструментов GNU Binutils:

- elfedit — изменение заголовка файла ELF, часть набора GNU Binutils.
- objdump — вывод информации об объектных файлах (в том числе и ELF), часть набора GNU Binutils.
- readelf — вывод подробной информации об объектном файле формата ELF, часть набора GNU Binutils.
- elfdump — вывод информации о ELF файле, часть набора GNU Binutils.
- elfutils — альтернатива для GNU Binutils, официально доступна только для GNU/Linux, но существуют порты на другие операционные системы.
- file — вывод небольшого количества информации о файлах известных программе форматов (доступна для большинства UNIX-подобных операционных систем)."


Посмотреть полный заголовок (заголовок имеет размер 52 байта для 32-битных файлов или 64 для 64-битных):
hexdump -C -n 64 /usr/bin/ls

Команды для изучения (содержимого) файлов:
dumpelf
If command 'dumpelf' not found, it can be installed with:
sudo apt install pax-utils

dumpelf /usr/bin/ls
- тоже получаем заголовок elf

readelf -l /usr/bin/ls
readelf -S /usr/bin/ls

objdump -f /usr/bin/ls

file /usr/bin/ls

ldd /usr/bin/ls

vimdiff binary_1 binary_2

http://www.muppetlabs.com/~breadbox/software/ELF.txt

objdump -D /usr/bin/ls | head -n 20
objdump -D /usr/bin/ls | tail -f -n 20

objdump -S woody > woody.obj ; objdump -S /usr/bin/ls > hello.obj

# What are stripped and not-stripped executables in Unix?
https://unix.stackexchange.com/questions/2969/what-are-stripped-and-not-stripped-executables-in-unix
"If you compile an executable with gcc's -g flag, it contains debugging information. That means for each instruction there is information which line of the source code generated it, the name of the variables in the source code is retained and can be associated to the matching memory at runtime etc. Strip can remove this debugging information and other data included in the executable which is not necessary for execution in order to reduce the size of the executable."
"See also the strip(1) manpage. Generally strip removes all symbols since they're not strictly necessary; it removes debugging info too, but symbols are the big thing."

# Ассемблер:
sudo apt-get install nasm

https://www.opennet.ru/man.shtml?topic=xxd&category=1&russian=0
Флаги при компиляции

https://www.cyberforum.ru/asm-beginners/thread1768203.html
put zero in register

https://ru.stackoverflow.com/questions/4446/%D0%A7%D1%82%D0%BE-%D1%8D%D1%84%D1%84%D0%B5%D0%BA%D1%82%D0%B8%D0%B2%D0%BD%D0%B5%D0%B9-inc-eax-%D0%B8%D0%BB%D0%B8-add-eax-1?ysclid=l7qi8ejhwo8978828
Что эффективней: inc eax или add eax,1?

http://natalia.appmat.ru/c&c++/assembler.html

https://sysprog.ru/post/komandy-cbw-cwd-cwde-cdq
Команды CBW, CWD, CWDE, CDQ

https://stackoverflow.com/questions/31234395/why-use-rip-relative-addressing-in-nasm
lea rsi, [rel msg]
"lea reg,[expression] uses all this complex way of calculating addresses to load the address itself into reg (unlike mov, which dereferences the address calculated). Thus the information, unavailable at compilation time, namely absolute address which would be in RIP, can be encoded in the instruction without knowing its value. The nasm expression lea rsi,[rel msg] gets translated into something like"

https://wasm.in/blogs/vvedenie-v-reversing-s-nulja-ispolzuja-ida-pro-chast-3.5/

http://pascal.net.ru/DB
db

https://stackoverflow.com/questions/34231839/assembly-print-variables-and-values
"db directives let you put assemble-time-constant bytes into the object file (usually in the data section). You can use an expression as an argument, to have the assembler do some math for you at assemble time. Anything that needs to happen at run time needs to be done by instructions that you write, and that get run."

# Пример из чек-листа требует добавления библиотеки:

Before running the program, run the following command:
`echo "int main(void){ printf(\"Hello\\n\"); return 0;}" > hello.c; gcc -m64 -o hello hello.c`
(type == DYN)

gcc -m64 -no-pie -o hello hello.c
(type == EXEC)

This will not work in LINUX, 64-bit:
gcc -m32 hello.c
"In file included from hello.c:1:
/usr/include/stdio.h:27:10: fatal error: bits/libc-header-start.h: No such file or directory
   27 | #include <bits/libc-header-start.h>"

Следующее все равно не поможет (для gcc -m32 hello.c):
sudo apt-get install gcc-multilib 

# Видео из Интры про проект:
https://elearning.intra.42.fr/notions/woody_woodpacker/subnotions/woody_woodpacker/videos/woody_woodpacker

****

# Шифрование
https://habr.com/ru/post/111510/
rc4 схема шифрования

https://habr.com/ru/post/499490/
рандомайзеры

# Битовые операции
https://learnc.info/c/bitwise_operators.html
биты

# Определить размер файла: lseek(): 
http://www.c-cpp.ru/content/lseek

# Примеры elf-файлов
uname
date
echo &USER
ls
pwd
cat
sort < file_name