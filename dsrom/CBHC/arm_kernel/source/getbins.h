
#ifndef _GETBINS_H_
#define _GETBINS_H_

#define titleprot_addr 0x0510D984
#define wupserver_addr 0x0510E570

#define titleprot_phys (titleprot_addr - 0x05100000 + 0x13D80000)
#define wupserver_phys (wupserver_addr - 0x05100000 + 0x13D80000)

const unsigned char *get_titleprot_bin();
unsigned int get_titleprot_bin_len();

const unsigned char *get_wupserver_bin();
unsigned int get_wupserver_bin_len();

#endif
