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
	int nfds = 0;
	int gpio_fd, timeout;
	struct epoll_event evd;
	struct epoll_event * events;
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

		int epollfd = epoll_create(10);
		events =  calloc (10, sizeof(struct epoll_event));
		evd.data.fd = gpio_fd;
		evd.events = EPOLLPRI;
		epoll_ctl(epollfd,EPOLL_CTL_ADD,gpio_fd,&evd);

        while (keepgoing) {
		 		nfds = epoll_wait(epollfd,events,10,-1);
		        if (nfds < 0) {
				 			printf("\npoll() failed!\n");
				 			return -1;
				 	    }
        	    for (int i = 0;i<nfds;i++ )
        	    {
        	    	if (events->events&EPOLLPRI)
        	    	{
        	    		memset(buf,0x00,sizeof(buf));
        	    		len = read(events->data.fd,buf,sizeof(buf));
        	    		lseek(events->data.fd,0,SEEK_SET);
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
        	    }
		 	}
		 	gpio_fd_close(gpio_fd);
	        return (0);
}
