#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// CONVERTS UNSIGNED LONG VALUES TO BINARY STRINGS
const char *unsigned_to_binary(unsigned long msg, int size)
{
	char* b = (char *)malloc(size + 1);	//allocates memory to b
	b[0] = '\0';	//empties b

	int z;
	for (z = (1<<(size-1)); z > 0; z >>= 1)
	{
		// for each 0 or 1 in the msg, a corresponding
		// 0 or 1 is concatenated to string b
		strcat(b, ((msg & z) == z) ? "1" : "0");
	}

	return b;	// returns b
}

// CALCULATES THE REMAINDER
unsigned long crc_remainder(unsigned long msg, unsigned long check, int mlen, int clen) {
	unsigned long newmsg = msg << (clen-1);	// initialize newmsg
	unsigned long n;	// initialize n
	int i;	// initialize i
	for (i = mlen; i > 0; i--) {
		// perform division to calculate remainder
		if ((newmsg & (1 << (i+clen-2))) != 0) {
			n = check << (i - 1);
			newmsg = newmsg ^ n;
		}
	}
	return newmsg & ((1 << clen) - 1);	// return remainder & newmsg
}

int main() { 
	unsigned long msg = 0xC74A;	//initializes msg
	unsigned long divisor = 0xB;	//initializes divisor
	// divides the msg by the divisor and adds the remainder to the message
	unsigned long newmsg = (msg << 3) + crc_remainder(msg, divisor, 16, 4);
	unsigned long rem, newrem;
	printf("Message is: ");	
	printf("%s\n", unsigned_to_binary(msg, 16));	//prints msg
	printf("Divisor is: ");
	printf("%s\n", unsigned_to_binary(divisor, 4));	//prints divisor
	rem = crc_remainder(msg, divisor, 16, 4);	//calculates remainder
	printf("Remainder of message and divisor: ");
	printf("%s\n", unsigned_to_binary(rem, 3));	//prints remainder
	printf("Message with added bits is: ");
	printf("%s\n", unsigned_to_binary(newmsg, 19));	//prints msg w/ added bits
	newrem = crc_remainder(newmsg, divisor, 19, 4);	//calculates 2nd remainder
	printf("Remainder is: ");
	printf("%s\n", unsigned_to_binary(newrem, 3));	//prints 2nd remainder


	printf("\nEXAMPLE FROM BOOK:\n\n");
	char * ptr;	
	msg = strtoul("10011010", &ptr, 2);	//converts string to unsigned long w/ base 2
	divisor = strtoul("1101", &ptr, 2);

	printf("Message is: ");
	printf("%s\n", unsigned_to_binary(msg, 8));	//prints message
	printf("Divisor is: ");
	printf("%s\n", unsigned_to_binary(divisor, 4));	//prints divisor
	rem = crc_remainder(msg, divisor, 8, 4);
	printf("Remainder of message and divisor: ");	
	printf("%s\n", unsigned_to_binary(rem, 3));	//prints remainder
	newmsg = (msg << 3) + rem;	// add remainder to newmsg
	printf("Message with added bits is: ");
	printf("%s\n", unsigned_to_binary(newmsg, 11));	//print newmsg
	newrem = crc_remainder(newmsg, divisor, 11, 4);	//calculate newrem
	printf("Remainder is: ");
	printf("%s\n", unsigned_to_binary(newrem, 3));	//print newrem
	return 0;
}