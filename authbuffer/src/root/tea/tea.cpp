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
����oi_tea.h oi_tea.c�޸�

�޸�OI_oi_symmetry_encrypt�е�bug��(����Դ������+10)%8����ʱ���ֵ���ѭ������
��������

������Ӧ��ϵ��
OI_TeaEncryptECB_1 ��Ӧ OI_TeaEncryptECB
tea_symmetry_encrypt ��Ӧ oi_symmetry_encrypt
OI_TeaDecryptECB_1 ��Ӧ TeaDecryptECB
tea_symmetry_decrypt ��Ӧ oi_symmetry_decrypt

ͷ�ļ���/usr/local/commlib/baselib/oi_tea.h
��̬�⣺/usr/local/commlib/baselib/libcomm_oi.a
c�ļ���/usr/local/commlib64_proj/trunk/baselib/oi_tea.c

*/

/*pOutBuffer��pInBuffer��Ϊ8byte, pKeyΪ16byte*/
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

/*pKeyΪ16byte*/
/*
����:pInBufΪ����ܵ����Ĳ���(Body),nInBufLenΪpInBuf����;
���:pOutBufΪ���ĸ�ʽ,pOutBufLenΪpOutBuf�ĳ�����8byte�ı���,����ӦԤ��nInBufLen+17;
*/
/*TEA�����㷨,CBCģʽ*/
/*���ĸ�ʽ:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
void
symmetry_encrypt(const unsigned char * pInBuf, int nInBufLen, const unsigned char * pKey, unsigned char * pOutBuf, int *pOutBufLen)
{

    int nPadSaltBodyZeroLen /*PadLen(1byte)+Salt+Body+Zero�ĳ��� */ ;
    int nPadlen;
    unsigned char src_buf[8], zero_iv[8], *iv_buf;
    int src_i, i, j;

    /*����Body���ȼ���PadLen,��С���賤�ȱ���Ϊ8byte�������� */
    nPadSaltBodyZeroLen = nInBufLen /*Body���� */  + 1 + SALT_LEN + ZERO_LEN /*PadLen(1byte)+Salt(2byte)+Zero(7byte) */ ;

    nPadlen = nPadSaltBodyZeroLen % 8;  /*len=nSaltBodyZeroLen%8 */
    if(nPadlen != 0)
    {
        /*ģ8��0�貹0,��1��7,��2��6,...,��7��1 */
        nPadlen = 8 - nPadlen;
    }

    /* srand( (unsigned)time( NULL ) ); ��ʼ������� */
    /*���ܵ�һ������(8byte),ȡǰ��10byte */
    src_buf[0] = (((unsigned char) rand()) & 0x0f8) /*�����λ��PadLen,���� */  | (unsigned char) nPadlen;
    src_i = 1;  /*src_iָ��src_buf��һ��λ�� */

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
            /*pOutBuffer��pInBuffer��Ϊ8byte, pKeyΪ16byte */
            OI_TeaEncryptECB_1(src_buf, pKey, pOutBuf);
            src_i = 0;
            iv_buf = pOutBuf;
            *pOutBufLen += 8;
            pOutBuf += 8;
        }
    }

    /*src_iָ��src_buf��һ��λ�� */

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
            /*pOutBuffer��pInBuffer��Ϊ8byte, pKeyΪ16byte */
            OI_TeaEncryptECB_1(src_buf, pKey, pOutBuf);
            src_i = 0;
            iv_buf = pOutBuf;
            *pOutBufLen += 8;
            pOutBuf += 8;
        }
    }

    /*src_iָ��src_buf��һ��λ�� */

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
            /*pOutBuffer��pInBuffer��Ϊ8byte, pKeyΪ16byte */
            OI_TeaEncryptECB_1(src_buf, pKey, pOutBuf);
            src_i = 0;
            iv_buf = pOutBuf;
            *pOutBufLen += 8;
            pOutBuf += 8;
        }
    }
}


/*pOutBuffer��pInBuffer��Ϊ8byte, pKeyΪ16byte*/
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


/*pKeyΪ16byte*/
/*
����:pInBufΪ���ĸ�ʽ,nInBufLenΪpInBuf�ĳ�����8byte�ı���; *pOutBufLenΪ���ջ������ĳ���
    �ر�ע��*pOutBufLenӦԤ�ý��ջ������ĳ���!
���:pOutBufΪ����(Body),pOutBufLenΪpOutBuf�ĳ���,����ӦԤ��nInBufLen-10;
����ֵ:�����ʽ��ȷ����TRUE;
*/
/*TEA�����㷨,CBCģʽ*/
/*���ĸ�ʽ:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
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

    nPadLen = dest_buf[0] & 0x7 /*ֻҪ�����λ */ ;

    /*���ĸ�ʽ:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte) */
    i = nInBufLen - 1 /*PadLen(1byte) */  - nPadLen - SALT_LEN - ZERO_LEN;  /*���ĳ��� */
    if(*pOutBufLen < i)
        return false;
    *pOutBufLen = i;
    if(*pOutBufLen < 0)
        return false;


    iv_buf = pInBuf;    /*init iv */
    nInBufLen -= 8;
    pInBuf += 8;

    dest_i = 1; /*dest_iָ��dest_buf��һ��λ�� */


    /*��Padding�˵� */
    dest_i += nPadLen;

    /*dest_i must <=8 */

    /*��Salt�˵� */
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

            dest_i = 0; /*dest_iָ��dest_buf��һ��λ�� */
        }
    }

    /*��ԭ���� */

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

            dest_i = 0; /*dest_iָ��dest_buf��һ��λ�� */
        }
    }

    /*У��Zero */
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

            dest_i = 0; /*dest_iָ��dest_buf��һ��λ�� */
        }

    }

    return true;
}

