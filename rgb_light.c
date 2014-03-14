#include "rgb_light.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static void pabort(const char *s)
{
  perror(s);
  abort();
}

//static const char *device = "/dev/spidev0.0";
//static uint8_t mode = 0;
//static uint8_t bits = 8;
//static uint32_t speed = 500000;
//static uint16_t delay = 0;

//int fd;

static void transfer(RGB_LIGHT_T *state, uint8_t r_color, uint8_t g_color, uint8_t b_color)
{
  int ret;
  uint8_t tx[] = {
    r_color, g_color, b_color,
  };
  uint8_t rx[ARRAY_SIZE(tx)] = {0, };
  struct spi_ioc_transfer tr = {
    .tx_buf = (unsigned long)tx,
    .rx_buf = (unsigned long)rx,
    .len = ARRAY_SIZE(tx),
    .delay_usecs = state->delay,
    .speed_hz = state->speed,
    .bits_per_word = state->bits,
  };

  ret = ioctl(state->fd, SPI_IOC_MESSAGE(1), &tr);
  if (ret < 1)
    pabort("can't send spi message");

}

void initialize_rgb(RGB_LIGHT_T *state) {
  int ret = 0;

  state->fd = open(state->device, O_RDWR);
  if (state->fd < 0)
    pabort("can't open device");

  /*
   * spi mode
   */
  ret = ioctl(state->fd, SPI_IOC_WR_MODE, &state->mode);
  if (ret == -1)
    pabort("can't set spi mode");

  ret = ioctl(state->fd, SPI_IOC_RD_MODE, &state->mode);
  if (ret == -1)
    pabort("can't get spi mode");

  /*
   * bits per word
   */
  ret = ioctl(state->fd, SPI_IOC_WR_BITS_PER_WORD, &state->bits);
  if (ret == -1)
    pabort("can't set bits per word");

  ret = ioctl(state->fd, SPI_IOC_RD_BITS_PER_WORD, &state->bits);
  if (ret == -1)
    pabort("can't get bits per word");

  /*
   * max speed hz
   */
  ret = ioctl(state->fd, SPI_IOC_WR_MAX_SPEED_HZ, &state->speed);
  if (ret == -1)
    pabort("can't set max speed hz");

  ret = ioctl(state->fd, SPI_IOC_RD_MAX_SPEED_HZ, &state->speed);
  if (ret == -1)
    pabort("can't get max speed hz");

  /* printf("spi mode: %d\n", mode); */
  /* printf("bits per word: %d\n", bits); */
  /* printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000); */

  // transfer(fd, (uint8_t) r, (uint8_t) g, (uint8_t) b);
}

void shutdown_rgb(RGB_LIGHT_T *state) {
  close(state->fd);
}

