#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

/*** constants ***/
#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define POLL_TIMEOUT (3 * 1000) /* 3 seconds */
#define MAX_BUF 64

/*** gpio functions ***/
/* gpio export */
int gpio_export(unsigned int gpio);
/* gpio unexport */
int gpio_unexport(unsigned int gpio);
/* gpio set dir */
int gpio_set_dir(unsigned int gpio, const char *dir);
/* gpio set value */
int gpio_set_value(unsigned int gpio, unsigned int value);
/* gpio get value */
int gpio_get_value(unsigned int gpio, unsigned int *value);
/* gpio set edge */
int gpio_set_edge(unsigned int gpio, const char *edge);
/* gpio fd open */
int gpio_fd_open(unsigned int gpio, unsigned int dir);
/* gpio fd close */
int gpio_fd_close(int fd);
