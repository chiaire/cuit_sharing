/*（1）要求把欧几里得扩展算法、平方乘方法和模重复平方方法写成一个函数。（备注：函数名称命名方式 ****_学号后2位，变量名称***_学号后2位，例如学号为“2018011263”，平方乘算法，LRFun_63(int a_63,int m_63,int n_63）；
（2）在主程序中读入数据公钥e，两素数p，q，以及待加密消息m；
（3）计算n值和n的欧拉函数，并输出显示；
（4）计算私钥d值；
（5）调用平方乘方法或者模重复平方方法，使用公钥加密得到密文c，并输出显示；
（6）调用平方乘方法或者模重复平方方法，使用解密得到明文m，并输出显示；
（7）使用模重复平方或者平方乘输出计算的中间结果及最终运算结果。*/
#define _CRT_SECURE_NO_WARNINGS 
#include<stdio.h>

//模重复平方法 
int RLFun_24(int a_24, int n_24, int key_24[], int klen_24)
{
	int s = 1, i;
	for (i = 0; i < klen_24; i++)
	{

		if (key_24[i] == 1)
		{
			s = (s * a_24) % n_24;
		}
		a_24 = (a_24 * a_24) % n_24;
		printf("i=%d,  %d\n", i, s);
	}
	return s;
}
int Exgcd(int a_24, int b_24, int* x_24, int* y_24)//扩展欧几里得ax+by=gcd（a，b）
{
	int r, temp;
	if (b_24 == 0)
	{
		*x_24 = 1;
		*y_24 = 0;
		return a_24;
	}
	r = Exgcd(b_24, a_24 % b_24, x_24, y_24);  //ax1+by1=bx2+[a-(a/b)b]y2=ay2+bx2-b(a/b)y2
	temp = *x_24;					   //由于a和b相同，所以有x1=y2 y1=x2-(a/b)y2
	*x_24 = *y_24;
	*y_24 = temp - a_24 / b_24 * (*y_24);
	return *y_24;
}
int main() {
	int e_24, p_24, q_24, m_24, n_24, n2_24, d_24, c_24, x_24 = 0, y_24 = 0, key_24[10], i = 0, j = 0, temp_24;
	printf("依次输入公钥,两素数p和q,加密消息m\n");
	scanf("%d %d %d %d", &e_24, &p_24, &q_24, &m_24);
	temp_24 = e_24;
	n_24 = p_24 * q_24;
	n2_24 = (p_24 - 1) * (q_24 - 1);
	printf("计算n值和n的欧拉函数值为:%d,%d", n_24, n2_24);
	d_24 = Exgcd(n2_24, e_24, &x_24, &y_24);
	printf("\n计算私钥d得:d = %d", d_24);
	printf("\n开始加密:\n");
	for (i = 0; temp_24 > 0; i++) {
		key_24[i] = temp_24 % 2;
		temp_24 = temp_24 / 2;
	}
	temp_24 = i;
	c_24 = RLFun_24(m_24, n_24, key_24, temp_24);
	printf("加密后为:%d", c_24);
	printf("开始解密:\n");
	temp_24 = d_24;
	for (i = 0; temp_24 > 0; i++) {
		key_24[i] = temp_24 % 2;
		temp_24 = temp_24 / 2;
	}
	temp_24 = i;
	m_24 = RLFun_24(c_24, n_24, key_24, temp_24);
	printf("加密后为:%d", m_24);
}
