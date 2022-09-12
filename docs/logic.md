# Program logic:

   - find the `.text` section for future encryption;

   - find the `LOAD` segment with equal p_memsz and p_filesz;

   - check if there is enough space filled with null bytes to insert the decryptor at the end of this `LOAD` segment;

   - if yes:
		- encrypt `.text` section;
   
   		- change the entry point in elf header to point to the decryptor part;

   		- save the old entry point, new entry point, encrypment key, start point and size of the encrypted `.text` section in the decryptor part;

   		- in the `LOAD` segment: add the PF_W flag for ability to write, insert the decryptor at the end of the `LOAD` segment, increase segment parameters: p_memsz and p_filesz;

