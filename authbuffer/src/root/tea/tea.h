#ifndef _TEA_H_
#define _TEA_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*pKeyΪ16byte*/
/*
����:pInBufΪ����ܵ����Ĳ���(Body),nInBufLenΪpInBuf����;
���:pOutBufΪ���ĸ�ʽ,pOutBufLenΪpOutBuf�ĳ�����8byte�ı���,����ӦԤ��nInBufLen+17;
*/
/*TEA�����㷨,CBCģʽ*/
/*���ĸ�ʽ:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
    void symmetry_encrypt(const unsigned char * pInBuf, int nInBufLen, 
		const unsigned char * pKey, unsigned char * pOutBuf, int *pOutBufLen);


/*pKeyΪ16byte*/
/*
����:pInBufΪ���ĸ�ʽ,nInBufLenΪpInBuf�ĳ�����8byte�ı���; *pOutBufLenΪ���ջ������ĳ���
    �ر�ע��*pOutBufLenӦԤ�ý��ջ������ĳ���!
���:pOutBufΪ����(Body),pOutBufLenΪpOutBuf�ĳ���,����ӦԤ��nInBufLen-10;
����ֵ:�����ʽ��ȷ����TRUE;
*/
/*TEA�����㷨,CBCģʽ*/
/*���ĸ�ʽ:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/ 
   bool symmetry_decrypt(const unsigned char * pInBuf, int nInBufLen, 
   		const unsigned char * pKey, unsigned char * pOutBuf, int *pOutBufLen);

#ifdef __cplusplus
}
#endif

#endif // #ifndef _TEA_H_
