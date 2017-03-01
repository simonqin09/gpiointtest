#include "gpioconfig.h"

/* gpio export */
int gpio_export(unsigned int gpio)
{
	int fd, len;
	char buf[MAX_BUF];

	fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
	if (fd < 0) {
		 printf ("\nFailed export GPIO-%d\n", gpio);
		return fd;
	}

	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);
	//printf ("\nSucessfully export GPIO-%d\n", gpio);
	return 0;
}
/* gpio unexport */
int gpio_unexport(unsigned int gpio)
{
	int fd, len;
	char buf[MAX_BUF];

	fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
	if (fd < 0) {
		 printf ("\nFailed unexport GPIO-%d\n", gpio);
		return fd;
	}

	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);
	//printf ("\nSucessfully unexport GPIO-%d\n", gpio);
	return 0;
}
/* gpio set dir */
int gpio_set_dir(unsigned int gpio, const char *dir)
{
	int fd, len;
	char buf[MAX_BUF];

	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/direction", gpio);
	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		 printf ("\nFailed set GPIO-%d direction\n", gpio);
		return fd;
	}

	write(fd, dir, strlen(dir)+1);
	close(fd);
	//printf ("\nSucessfully set GPIO-%d direction\n", gpio);
	return 0;
}
/* gpio set value */
int gpio_set_value(unsigned int gpio, unsigned int value)
{
	int fd, len;
	char buf[MAX_BUF];

	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		printf ("\nFailed set GPIO-%d value\n", gpio);
		return fd;
	}

	if (value!=0)
		{
		int i = write(fd, "1", 2);
	   // printf ("\nGPIO-%d value set high\n", gpio);
		}
	else
		{
		write(fd, "0", 2);
		//printf ("\nGPIO-%d value set low\n", gpio);
		}

	close(fd);
	//printf ("\nSucessfully set GPIO-%d value\n", gpio);
	return 0;
}
/* gpio get value */
int gpio_get_value(unsigned int gpio, unsigned int *value)
{
	int fd, len;
	char buf[MAX_BUF];
	char ch;

	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

	fd = open(buf, O_RDONLY);
	if (fd < 0) {
		printf ("\nFailed get GPIO-%d value\n", gpio);
		return fd;
	}

	read(fd, &ch, 1);

	if (ch != '0') {
		*value = 1;
	} else {
		*value = 0;
	}

	close(fd);
	//printf ("\nSucessfully get GPIO-%d value\n", gpio);
	return 0;
}
/* gpio set edge */
int gpio_set_edge(unsigned int gpio, const char *edge)
{
	int fd, len;
	char buf[MAX_BUF];

	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		printf ("\nFailed set GPIO-%d edge\n", gpio);
		return fd;
	}

	write(fd, edge, strlen(edge) + 1);
	close(fd);
	return 0;
}
/* gpio fd open */
int gpio_fd_open(unsigned int gpio, unsigned int dir)
{
	int fd, len;
	char buf[MAX_BUF];

	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

	fd = open(buf, dir | O_NONBLOCK );
	if (fd < 0) {
		perror("gpio/fd_open");
	}
	return fd;
}
/* gpio fd close */
int gpio_fd_close(int fd)
{
	return close(fd);
}
