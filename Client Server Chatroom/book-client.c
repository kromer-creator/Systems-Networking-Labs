#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#define SERVER_PORT 5432
#define MAX_LINE 256

// CRC REMAINDER FUNCTION
unsigned long crc_remainder(unsigned long msg, unsigned long check, int mlen, int clen) {
	unsigned long newmsg = msg << (clen-1);
	unsigned long n;
	int i;
	for (i = mlen; i > 0; i--) {
		if ((newmsg & (1 << (i+clen-2))) != 0) {
			n = check << (i - 1);
			newmsg = newmsg ^ n;
		}
	}
	return newmsg & ((1 << clen) - 1);
}

// CRC UNSIGNED TO BINARY FUNCTION
const char *unsigned_to_binary(unsigned long msg, int size)
{
	char* b = (char *)malloc(size + 1);
	b[0] = '\0';

	int z;
	for (z = (1<<(size-1)); z > 0; z >>= 1)
	{
		strcat(b, ((msg & z) == z) ? "1" : "0");
	}

	return b;
}

int main(int argc, char * argv[])
{
	FILE *fp;
	struct hostent *hp;
	struct sockaddr_in sin;
	char *host;
	char buf[MAX_LINE];
	int s;
	int len;

	if (argc==2) {
		host = argv[1];
	}
	else {
		fprintf(stderr, "usage: simplex-talk host\n");
		exit(1);
	}

	/* translate host name into peer's IP address */
	hp = gethostbyname(host);
	if (!hp) {
		fprintf(stderr, "simplex-talk: unknown host: %s\n", host);
		exit(1);
	}

	/* build address data structure */
	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	memcpy((char *)&sin.sin_addr, hp->h_addr, hp->h_length);
	sin.sin_port = htons(SERVER_PORT);

	/* active open */
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("simplex-talk: socket");
		exit(1);
	}
	if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		perror("simplex-talk: connect");
		close(s);
		exit(1);
	}
	/* main loop: get and send lines of text */
  printf("Send a message to the server: ");
	while (fgets(buf, sizeof(buf), stdin)) {
		buf[MAX_LINE-1] = '\0';
		len = strlen(buf) + 1;

		char * ptr;
		// CONVERT INPUT STRING TO UNSIGNED LONG
		unsigned long msg = strtoul(buf, &ptr, 2);
		unsigned long divisor = strtoul("1011", &ptr, 2);
		unsigned long rem, newrem;
		// COMPUTE REMAINDER + PRINT
		rem = crc_remainder(msg, divisor, len, 4);
		printf("Remainder: %s\n", unsigned_to_binary(rem, 3));
		// ADD REMAINDER TO INPUT MESSAGE + PRINT
		unsigned long newmsg = (msg << 3) + rem;
		printf("Message with added bits is: %s\n", unsigned_to_binary(newmsg, len+1));
		// CALCULATE SECOND REMAINDER AND PRINT
		newrem = crc_remainder(newmsg, divisor, len+3, 4);
		printf("2nd remainder is: %s\n", unsigned_to_binary(newrem, 3));
		char crc_val[3] = "";	// INITIALIZE EMPTY 'CRC VALUE' STRING
		
		// .3 CHANCE OF A BIT FLIP
		srand(time(0));
		int rand_num = (rand() % 10) + 1;
		if (rand_num <= 3){
			srand(time(0));
			// RANDOM NUM IN LENGTH OF BUFFER
			int rndm = (rand() % len - 1) + 1;

			// RANDOMLY FLIP A BIT IN BUF
			if (buf[rndm] == '1'){
				buf[rndm] = '0';
			}
			else{
				buf[rndm] = '1';
			}

			// PRINT THE BIT-FLIPPED STRING
			printf("Bits have been flipped!!\n");
			printf("New string: %s\n", buf);

			// REMOVE NEWLINE FROM BUF
			if (buf[len-2] == '\n'){
				buf[len-2] = 0;
			}
			// CONCATENATE REMAINDER TO END OF BUF
			strcat(buf, unsigned_to_binary(rem, 3));
			// PRINT BIT-FLIPPED MESSAGE WITH ADDED BITS
			printf("Bit-Flipped message with added bits: %s\n", buf);
			
		}
		else{
			// REMOVE NEWLINE FROM BUF
			if (buf[len-2] == '\n'){
				buf[len-2] = 0;
			}
			// CONCATENATE REMAINDER TO END OF BUF
			strcat(buf, unsigned_to_binary(rem, 3));
			// PRINT THE MESSAGE WITH ADDED BITS
			printf("Message with added bits: %s\n", buf);
		}
		// SEND MESSAGE TO THE SERVER
		len = strlen(buf);
		send(s, buf, len, 0);
    printf("Send a message to the server: ");
	}
}
