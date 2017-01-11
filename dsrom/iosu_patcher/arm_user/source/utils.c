
void* m_memcpy(void *dst, const void *src, unsigned int len)
{
    const unsigned char *src_ptr = (const unsigned char *)src;
    unsigned char *dst_ptr = (unsigned char *)dst;

    while(len)
    {
        *dst_ptr++ = *src_ptr++;
        --len;
    }
    return dst;
}

void* m_memset(void *dst, int val, unsigned int bytes)
{
    unsigned char *dst_ptr = (unsigned char *)dst;
    unsigned int i = 0;
    while(i < bytes)
    {
        dst_ptr[i] = val;
        ++i;
    }
    return dst;
}
