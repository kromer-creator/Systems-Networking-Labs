#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define SERVER_PORT 5432
#define MAX_PENDING 1
#define MAX_LINE 256

// CRC FUNCTION TO CONVERT UNSIGNED LONG TO BINARY
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

// CRC FUNCTION TO CALCULATE REMAINDER OF MSG
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

int main()
{
	struct sockaddr_in sin;
	char buf[MAX_LINE];
	int len;
  unsigned int l;
	int s, new_s;
	char * ptr;

	/* build address data structure */
	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(SERVER_PORT);

	/* setup passive open */
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("simplex-talk: socket");
		exit(1);
	}
	if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0) {
		perror("simplex-talk: bind");
		exit(1);
	}
	listen(s, MAX_PENDING);
	
	while(1){
		l = sizeof(sin);
		if ((new_s = accept(s, (struct sockaddr *)&sin, &l)) < 0) {
			perror("simplex-talk: accept");
			exit(1);
		}
		
		/* wait for connection, then receive and print text */
		while((len = recv(new_s, buf, sizeof(buf), 0))) {
			buf[MAX_LINE-1] = '\0';
			
			// FIND LENGTH OF BUF
			len = strlen(buf);
			// FIND THE LAST 3 CHARS OF BUF AKA CLIENT'S REMAINDER
			const char *last_three = &buf[len-3];
			
			// CONVERT MSG & DIVISOR TO UNSIGNED LONG
			unsigned long msg = strtoul(buf, &ptr, 2);
			unsigned long divisor = strtoul("1011", &ptr, 2);
			// CALCULATE REMAINDER & PRINT
			unsigned long rem = crc_remainder(msg, divisor, len+1, 4);
			printf("Remainder: %s\n", unsigned_to_binary(rem, 3));
			
			// IF REMAINDER CLIENT REMAINDER != SERVER REMAINDER, PRINT ERROR MESSAGE
			if ((strcmp(last_three, unsigned_to_binary(rem, 3))) != 0){	
				printf("Error detected!\n");
			}
			else{
				// ELSE, PRINT NO ERRORS DETECTED
				printf("No errors detected!\n");
			}
		}
			
		close(new_s);
	}
}
