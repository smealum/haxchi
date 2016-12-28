
#ifndef _GETBINS_H_
#define _GETBINS_H_

#define titleprot_addr 0x0510D988
#define titleprot2_addr 0x0510D9AC
#define wupserver_addr 0x0510E570

const unsigned char *get_titleprot_bin();
unsigned int get_titleprot_bin_len();

const unsigned char *get_titleprot2_bin();
unsigned int get_titleprot2_bin_len();

const unsigned char *get_wupserver_bin();
unsigned int get_wupserver_bin_len();

#endif
