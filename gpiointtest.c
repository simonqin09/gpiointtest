#include "gpioconfig.h"

int keepgoing = 1;
// Callback called when SIGINT is sent to the process (Ctrl-C)
void signal_handler(int sig)
{
	printf( "Ctrl-C pressed, cleaning up and exiting..\n" );
	keepgoing = 0;
}

int main(int argc, char **argv)
{
	/* initial GPIO*/
	int gpio1 = 52;
	int gpio2 = 53;
	int value = 0;
	int nfds = 2;
	int gpio_fd, timeout, rc;
	struct pollfd fdset[2];
	char buf[MAX_BUF];
	int len;
	int cnt = 0;
	unsigned int valueback = 0;

	signal(SIGINT, signal_handler);

    /* GPIO - 35, SODIMM-133, output, connected to LED*/
	gpio_export(gpio1);
	gpio_set_dir(gpio1, "out");
	gpio_set_value(gpio1, value);
	printf ("\nSucessfully Configure GPIO-%d value as %d\n", gpio1, value);


	 /*GPIO-34, SODIMM-135, Input, Interrupt*/
	     gpio_export(gpio2);
		 gpio_set_dir(gpio2, "in");
		 gpio_set_edge(gpio2, "rising");
		 printf ("\nSucessfully Configure GPIO-%d as input\n", gpio2);

		 gpio_fd = gpio_fd_open(gpio2, O_RDONLY);
         timeout = POLL_TIMEOUT;

		 while (keepgoing) {
		 		memset((void*)fdset, 0, sizeof(fdset));

		 		fdset[0].fd = STDIN_FILENO;
		 		fdset[0].events = POLLIN;

		 		fdset[1].fd = gpio_fd;
		 		fdset[1].events = POLLPRI;

		 		rc = poll(fdset, nfds, timeout);

		 		if (rc < 0) {
		 			printf("\npoll() failed!\n");
		 			return -1;
		 		}

		 		if (rc == 0) {
		 			printf(".");
		 		}
		 		if (fdset[1].revents & POLLPRI) {
		 			lseek(fdset[1].fd, 0, SEEK_SET);  // Read from the start of the file
		 		    len = read(fdset[1].fd, buf, MAX_BUF);
		 			if (cnt > 0)
		 			{
		 				usleep(20000);
		 			    gpio_get_value(gpio2, &valueback);
                        if (valueback == 1)
		 			    {
                        	printf("\nButton pressed times X%d\n", cnt);
                        	value = !value;
		 			        gpio_set_value(gpio1, value);
		 			        if (value == 0)
		 			        {
		 			        	printf ("\nLED turns off\n");
		 			        }
		 			        else
		 			        {
		 			        	printf ("\nLED turns on\n");
		 			        }
		 			     }
		 			 }
		 			cnt++;
		 		}

		 		if (fdset[0].revents & POLLIN) {
		 			(void)read(fdset[0].fd, buf, 1);
		 			printf("\npoll() stdin read 0x%2.2X\n", (unsigned int) buf[0]);
		 		}

		 		fflush(stdout);
		 	}

		 gpio_fd_close(gpio_fd);
	     return (0);
}
