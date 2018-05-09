#ifndef _TEA_H_
#define _TEA_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*pKey为16byte*/
/*
输入:pInBuf为需加密的明文部分(Body),nInBufLen为pInBuf长度;
输出:pOutBuf为密文格式,pOutBufLen为pOutBuf的长度是8byte的倍数,至少应预留nInBufLen+17;
*/
/*TEA加密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
    void symmetry_encrypt(const unsigned char * pInBuf, int nInBufLen, 
		const unsigned char * pKey, unsigned char * pOutBuf, int *pOutBufLen);


/*pKey为16byte*/
/*
输入:pInBuf为密文格式,nInBufLen为pInBuf的长度是8byte的倍数; *pOutBufLen为接收缓冲区的长度
    特别注意*pOutBufLen应预置接收缓冲区的长度!
输出:pOutBuf为明文(Body),pOutBufLen为pOutBuf的长度,至少应预留nInBufLen-10;
返回值:如果格式正确返回TRUE;
*/
/*TEA解密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/ 
   bool symmetry_decrypt(const unsigned char * pInBuf, int nInBufLen, 
   		const unsigned char * pKey, unsigned char * pOutBuf, int *pOutBufLen);

#ifdef __cplusplus
}
#endif

#endif // #ifndef _TEA_H_
