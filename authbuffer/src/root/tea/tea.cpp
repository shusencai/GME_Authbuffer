#include <stdlib.h>

#ifdef _WIN32
#include <Winsock2.h>
#else
#include <netinet/in.h>
#endif

#include <string.h>
#include <time.h>

#include "tea.h"

typedef unsigned int  WORD32;
const WORD32 DELTA = 0x9e3779b9;

#define ROUNDS 16
#define LOG_ROUNDS 4

/*
基于oi_tea.h oi_tea.c修改

修复OI_oi_symmetry_encrypt中的bug：(加密源串长度+10)%8余零时出现的死循环问题
其他不变

函数对应关系：
OI_TeaEncryptECB_1 对应 OI_TeaEncryptECB
tea_symmetry_encrypt 对应 oi_symmetry_encrypt
OI_TeaDecryptECB_1 对应 TeaDecryptECB
tea_symmetry_decrypt 对应 oi_symmetry_decrypt

头文件：/usr/local/commlib/baselib/oi_tea.h
静态库：/usr/local/commlib/baselib/libcomm_oi.a
c文件：/usr/local/commlib64_proj/trunk/baselib/oi_tea.c

*/

/*pOutBuffer、pInBuffer均为8byte, pKey为16byte*/
void
OI_TeaEncryptECB_1(const unsigned char * pInBuf, const unsigned char * pKey, unsigned char * pOutBuf)
{
    WORD32 y, z;
    WORD32 sum;
    WORD32 k[4];
    int i;

    /*plain-text is TCP/IP-endian; */

    /*GetBlockBigEndian(in, y, z); */
    y = ntohl(*((WORD32 *) pInBuf));
    z = ntohl(*((WORD32 *) (pInBuf + 4)));
    /*TCP/IP network byte order (which is big-endian). */

    for(i = 0; i < 4; i++)
    {
        /*now key is TCP/IP-endian; */
        k[i] = ntohl(*((WORD32 *) (pKey + i * 4)));
    }

    sum = 0;
    for(i = 0; i < ROUNDS; i++)
    {
        sum += DELTA;
        y += ((z << 4) + k[0]) ^ (z + sum) ^ ((z >> 5) + k[1]);
        z += ((y << 4) + k[2]) ^ (y + sum) ^ ((y >> 5) + k[3]);
    }



    *((WORD32 *) pOutBuf) = htonl(y);
    *((WORD32 *) (pOutBuf + 4)) = htonl(z);


    /*now encrypted buf is TCP/IP-endian; */
}


#define SALT_LEN 2
#define ZERO_LEN 7

/*pKey为16byte*/
/*
输入:pInBuf为需加密的明文部分(Body),nInBufLen为pInBuf长度;
输出:pOutBuf为密文格式,pOutBufLen为pOutBuf的长度是8byte的倍数,至少应预留nInBufLen+17;
*/
/*TEA加密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
void
symmetry_encrypt(const unsigned char * pInBuf, int nInBufLen, const unsigned char * pKey, unsigned char * pOutBuf, int *pOutBufLen)
{

    int nPadSaltBodyZeroLen /*PadLen(1byte)+Salt+Body+Zero的长度 */ ;
    int nPadlen;
    unsigned char src_buf[8], zero_iv[8], *iv_buf;
    int src_i, i, j;

    /*根据Body长度计算PadLen,最小必需长度必需为8byte的整数倍 */
    nPadSaltBodyZeroLen = nInBufLen /*Body长度 */  + 1 + SALT_LEN + ZERO_LEN /*PadLen(1byte)+Salt(2byte)+Zero(7byte) */ ;

    nPadlen = nPadSaltBodyZeroLen % 8;  /*len=nSaltBodyZeroLen%8 */
    if(nPadlen != 0)
    {
        /*模8余0需补0,余1补7,余2补6,...,余7补1 */
        nPadlen = 8 - nPadlen;
    }

    /* srand( (unsigned)time( NULL ) ); 初始化随机数 */
    /*加密第一块数据(8byte),取前面10byte */
    src_buf[0] = (((unsigned char) rand()) & 0x0f8) /*最低三位存PadLen,清零 */  | (unsigned char) nPadlen;
    src_i = 1;  /*src_i指向src_buf下一个位置 */

    while(nPadlen--)
        src_buf[src_i++] = (unsigned char) rand();   /*Padding */

    /*come here, i must <= 8 */

    memset(zero_iv, 0, 8);
    iv_buf = zero_iv;   /*make iv */

    *pOutBufLen = 0;    /*init OutBufLen */

    for(i = 1; i <= SALT_LEN;)  /*Salt(2byte) */
    {
        if(src_i < 8)
        {
            src_buf[src_i++] = (unsigned char) rand();
            i++;    /*i inc in here */
        }

        if(src_i == 8)
        {
            /*src_i==8 */

            for(j = 0; j < 8; j++)  /*CBC XOR */
                src_buf[j] ^= iv_buf[j];
            /*pOutBuffer、pInBuffer均为8byte, pKey为16byte */
            OI_TeaEncryptECB_1(src_buf, pKey, pOutBuf);
            src_i = 0;
            iv_buf = pOutBuf;
            *pOutBufLen += 8;
            pOutBuf += 8;
        }
    }

    /*src_i指向src_buf下一个位置 */

    while(nInBufLen)
    {
        if(src_i < 8)
        {
            src_buf[src_i++] = *(pInBuf++);
            nInBufLen--;
        }

        if(src_i == 8)
        {
            /*src_i==8 */

            for(i = 0; i < 8; i++)  /*CBC XOR */
                src_buf[i] ^= iv_buf[i];
            /*pOutBuffer、pInBuffer均为8byte, pKey为16byte */
            OI_TeaEncryptECB_1(src_buf, pKey, pOutBuf);
            src_i = 0;
            iv_buf = pOutBuf;
            *pOutBufLen += 8;
            pOutBuf += 8;
        }
    }

    /*src_i指向src_buf下一个位置 */

    for(i = 1; i <= ZERO_LEN;)
    {
        if(src_i < 8)
        {
            src_buf[src_i++] = 0;
            i++;    /*i inc in here */
        }

        if(src_i == 8)
        {
            /*src_i==8 */

            for(j = 0; j < 8; j++)  /*CBC XOR */
                src_buf[j] ^= iv_buf[j];
            /*pOutBuffer、pInBuffer均为8byte, pKey为16byte */
            OI_TeaEncryptECB_1(src_buf, pKey, pOutBuf);
            src_i = 0;
            iv_buf = pOutBuf;
            *pOutBufLen += 8;
            pOutBuf += 8;
        }
    }
}


/*pOutBuffer、pInBuffer均为8byte, pKey为16byte*/
void
OI_TeaDecryptECB_1(const unsigned char * pInBuf, const unsigned char * pKey, unsigned char * pOutBuf)
{
    WORD32 y, z, sum;
    WORD32 k[4];
    int i;

    /*now encrypted buf is TCP/IP-endian; */
    /*TCP/IP network byte order (which is big-endian). */
    y = ntohl(*((WORD32 *) pInBuf));
    z = ntohl(*((WORD32 *) (pInBuf + 4)));

    for(i = 0; i < 4; i++)
    {
        /*key is TCP/IP-endian; */
        k[i] = ntohl(*((WORD32 *) (pKey + i * 4)));
    }

    sum = DELTA << LOG_ROUNDS;
    for(i = 0; i < ROUNDS; i++)
    {
        z -= ((y << 4) + k[2]) ^ (y + sum) ^ ((y >> 5) + k[3]);
        y -= ((z << 4) + k[0]) ^ (z + sum) ^ ((z >> 5) + k[1]);
        sum -= DELTA;
    }

    *((WORD32 *) pOutBuf) = htonl(y);
    *((WORD32 *) (pOutBuf + 4)) = htonl(z);

    /*now plain-text is TCP/IP-endian; */
}


/*pKey为16byte*/
/*
输入:pInBuf为密文格式,nInBufLen为pInBuf的长度是8byte的倍数; *pOutBufLen为接收缓冲区的长度
    特别注意*pOutBufLen应预置接收缓冲区的长度!
输出:pOutBuf为明文(Body),pOutBufLen为pOutBuf的长度,至少应预留nInBufLen-10;
返回值:如果格式正确返回TRUE;
*/
/*TEA解密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
bool
symmetry_decrypt(const unsigned char * pInBuf, int nInBufLen, const unsigned char * pKey, unsigned char * pOutBuf, int *pOutBufLen)
{

    int nPadLen, nPlainLen;
    unsigned char dest_buf[8];
    const unsigned char *iv_buf;
    int dest_i, i, j;


    if((nInBufLen % 8) || (nInBufLen < 16))
        return false;


    OI_TeaDecryptECB_1(pInBuf, pKey, dest_buf);

    nPadLen = dest_buf[0] & 0x7 /*只要最低三位 */ ;

    /*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte) */
    i = nInBufLen - 1 /*PadLen(1byte) */  - nPadLen - SALT_LEN - ZERO_LEN;  /*明文长度 */
    if(*pOutBufLen < i)
        return false;
    *pOutBufLen = i;
    if(*pOutBufLen < 0)
        return false;


    iv_buf = pInBuf;    /*init iv */
    nInBufLen -= 8;
    pInBuf += 8;

    dest_i = 1; /*dest_i指向dest_buf下一个位置 */


    /*把Padding滤掉 */
    dest_i += nPadLen;

    /*dest_i must <=8 */

    /*把Salt滤掉 */
    for(i = 1; i <= SALT_LEN;)
    {
        if(dest_i < 8)
        {
            dest_i++;
            i++;
        }

        if(dest_i == 8)
        {
            /*dest_i==8 */
            OI_TeaDecryptECB_1(pInBuf, pKey, dest_buf);
            for(j = 0; j < 8; j++)
                dest_buf[j] ^= iv_buf[j];

            iv_buf = pInBuf;
            nInBufLen -= 8;
            pInBuf += 8;

            dest_i = 0; /*dest_i指向dest_buf下一个位置 */
        }
    }

    /*还原明文 */

    nPlainLen = *pOutBufLen;
    while(nPlainLen)
    {
        if(dest_i < 8)
        {
            *(pOutBuf++) = dest_buf[dest_i++];
            nPlainLen--;
        }
        else if(dest_i == 8)
        {
            /*dest_i==8 */
            OI_TeaDecryptECB_1(pInBuf, pKey, dest_buf);
            for(i = 0; i < 8; i++)
                dest_buf[i] ^= iv_buf[i];

            iv_buf = pInBuf;
            nInBufLen -= 8;
            pInBuf += 8;

            dest_i = 0; /*dest_i指向dest_buf下一个位置 */
        }
    }

    /*校验Zero */
    for(i = 1; i <= ZERO_LEN;)
    {
        if(dest_i < 8)
        {
            if(dest_buf[dest_i++])
                return false;
            i++;
        }
        else if(dest_i == 8)
        {
            /*dest_i==8 */
            OI_TeaDecryptECB_1(pInBuf, pKey, dest_buf);
            for(j = 0; j < 8; j++)
                dest_buf[j] ^= iv_buf[j];

            iv_buf = pInBuf;
            nInBufLen -= 8;
            pInBuf += 8;

            dest_i = 0; /*dest_i指向dest_buf下一个位置 */
        }

    }

    return true;
}

