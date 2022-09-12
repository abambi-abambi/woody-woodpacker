/* Sample from:				https://www.programmersought.com/article/45853930943/
** Theory about cypher rc4:	https://habr.com/ru/post/111510/
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX 65534

int			arr1[256];
char		arr2[256];
int			key[256]; // randomly generated key
int			keyStream[MAX]; // key
char		plain_text[MAX];
char		crypto_text[MAX];
const char	*word_list = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

void	init_arr1()
{
	for (int i = 0; i < 256; i++)
	{
		arr1[i] = i;
	}
}

void	init_key()
{
	int	index;
	int	keylen;
	srand(time(NULL)); // current time as a seed
	
	keylen = (int)((double)random()/(double)(RAND_MAX) * 256); // random length of the key
	for (int i = 0; i < keylen; i++)
	{
		index = (int)((double)(random())/(double)(RAND_MAX) * 63);
		key[i] = word_list[index];
	}
	for (int i = 0; i < 256; i++) // init_arr2
	{
		arr2[i] = key[i%keylen];
	}	
}

void	permute_arr1()// replaces in arr1;
{
	int	temp;
	int	j = 0;
	for (int i = 0; i < 256; i++)
	{
		j = (j + arr1[i] + arr2[i]) % 256;
		temp = arr1[i];
		arr1[i] = arr1[j];
		arr1[j] = temp;
	}
}

void	create_key_stream(char *text, int text_len)
{
	int	i,j;
	int	temp, t, k;
	int	index = 0;

	i = j = 0;
	while (text_len --)
	{
		i = (i + 1) % 256;
		j = (j + arr1[i]) % 256;
		temp = arr1[i];
		arr1[i] = arr1[j];
		arr1[j] = temp;
		t = (arr1[i] + arr1[j]) % 256;
		keyStream[index] = arr1[t];
		index++;
	} 
}

void	Rc4EncryptText(char *text)// encrypt and decode
{
	int	text_len = strlen(text);
	int	plain_word;

	init_arr1();
	init_key();
	permute_arr1();
	create_key_stream(text, text_len);

	printf("Original text:\n\t%s\n\n", text);
	printf("Ciphere(d) text:\n\t");
	for (int i = 0; i < text_len; i++)
	{
		crypto_text[i] = (char)(keyStream[i] ^ text[i]); // encrypt
		printf("%c", crypto_text[i]);
	}
	printf("\n\nStart decryption:\n\t");
	for (int i = 0; i < text_len; i++){
		plain_text[i] = (char)(keyStream[i] ^ crypto_text[i]); // decrypt
		printf("%c", plain_text[i]);
	}
	printf("\n");
}

int		main()
{   
	char	text[] = "Hello world!!!";
	Rc4EncryptText(text);
	return (0);
}

// Compile:
// g++ rc4_ciphering.c

// Launch:
// ./a.out
