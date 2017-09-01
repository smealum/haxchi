#include "../../payload/titleprot_bin.h"
#include "../../payload/titleprot2_bin.h"
#include "../../payload/wupserver_bin.h"

//not really a bin but still a const val
const int from_cbhc = 1;

const unsigned char *get_titleprot_bin()
{
	return titleprot_bin;
}

unsigned int get_titleprot_bin_len()
{
	return titleprot_bin_len;
}

const unsigned char *get_titleprot2_bin()
{
	return titleprot2_bin;
}

unsigned int get_titleprot2_bin_len()
{
	return titleprot2_bin_len;
}

const unsigned char *get_wupserver_bin()
{
	return wupserver_bin;
}

unsigned int get_wupserver_bin_len()
{
	return wupserver_bin_len;
}
