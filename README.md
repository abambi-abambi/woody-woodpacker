# woody-woodpacker 
is a simple elf64 binary encryptor.


# build the project:
make


# choose the elf-64-file for encryption:
./woody_woodpacker /path_to/binary_file
or
./woody_woodpacker [-i flag] /path_to/binary_file
('-i' flag prints information about building new 'woody' file)


# launch
./woody
the new 'woody' file, which is a copy of original elf-64-file,
but with the encrypted '.text' section, which will be decrypted


The new 'woody' program will print '....woody....',
the rest behaviour is identical to the original elf-64-file.


# For more details see:

subject.pdf & checklist.pdf
