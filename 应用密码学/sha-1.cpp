/*（1）按照标准FIPS-180-2中SHA-1算法，从文件或者屏幕中读取消息，然后对消息分组，并对最后一个分组进行填充，并对每组通过数据扩充算法扩充到80个字，然后执行SHA-1算法，并显示输出。
（2）完成填充过程，消息的长度在1-200个字符。
三、实验要求
（1）输入待Hash消息字符串，编码方式为ASCII码。例如程序的默认输入为FIPS-180-2中示例的“abc”， 消息的长度在1-200个字符。
（2）按照SHA-1算法进行填充，然后512比特分组，分为多组，然后对每组消息进行处理，数据扩充到80个字。
（3）输出每一分组中的W0, W1，W14，W15，W16, W79 （十六进制）
（4）填充过程写成一个函数，数据扩充过程写成一个函数，数据扩充中循环移位也可以写成一个函数。函数名称命名方式 ****_学号后2位，变量名称***_学号后2位，例如学号为“2018011263”，平方乘算法，SHA1ProcessMessageBlock_63(unsigned char Message_Block）
例如：输入字符串“abc”
输出为：
W0 : 61626380
W1 :00000000
W14:00000000
W15 : 00000018 */

#include<stdio.h>
#include<cstring>
#include<cstdlib>
#define leftbit(bits,word) ((((word) << (bits)) & 0xFFFFFFFF) | ((word) >> (32-(bits))))

void SHA_1_024();
void PutBox24(char* s, int* box);//将消息转换成2进制数据依次填入数组
void Addlast24(int* box, int bit);//将消息的二进制长度输入末尾
void divide24(int* box, int w[][8]);//将2进制数据每4位转换成16进制数填入w数组
void extend24(int w[][8]);//扩展w数组
void bin(int w[][8], long long int p, int t);//异或循环左移的数据填充进w数组
void putdata(int w[][8]);//输出结果


int main(int argc, char* argv[]) {
	SHA_1_024();
	printf("\n");
}
//char string[100];
int lenth;//输入消息的长度
int bit;//输入消息的初始bit
char sput[100];
int box1[512];
int box2[512];
int w[80][8];//数据扩充存放数组
int w_[80][8];//输入bit大于448是要动用的扩充数组
long long int a, b, c, d, x;
void SHA_1_024() {
	printf("请输入消息:");
	gets_s(sput);
	lenth = strlen(sput);
	bit = lenth * 8;
	PutBox24(sput, box1);
	box1[bit] = 1;//原始消息后添加一个"1"比特位
	if (bit < 448) {
		Addlast24(box1, bit);
		divide24(box1, w);
		extend24(w);
		putdata(w);
	}
	else {
		Addlast24(box2, bit);
		divide24(box1, w);
		divide24(box2, w_);
		extend24(w);
		extend24(w_);
		putdata(w);
		putdata(w_);
	}
}
void PutBox24(char* s, int* box) {
	int i = 0;
	int temp;//暂时保存二进制的数
	while (s[i] != '\0') {
		for (; i < lenth; i++) {
			int count = 0;
			while (s[i] != 0) {
				temp = s[i] % 2;
				s[i] = s[i] / 2;
				box[(i + 1) * 7 + i - count] = temp;
				count++;
			}
		}
	}
}
void Addlast24(int* box, int bit) {
	int temp;
	int count = 0;
	while (bit != 0) {
		temp = bit % 2;
		bit = bit / 2;
		box[511 - count] = temp;
		count++;
	}
}
void divide24(int* box, int w[][8]) {
	int i = 0, j = 0, count = 0;
	int sum = 0, flag = 0;
	while (count < 513) {
		if (j == 8) {
			i++;
			j = 0;
		}
		if (flag == 4) {
			w[i][j] = sum;
			flag = 0;
			sum = 0;
			j++;
		}
		if (count % 4 == 0) {
			sum += box[count] * 8;
			flag++;
		}
		else if (count % 4 == 1) {
			sum += box[count] * 4;
			flag++;
		}
		else if (count % 4 == 2) {
			sum += box[count] * 2;
			flag++;
		}
		else if (count % 4 == 3) {
			sum += box[count] * 1;
			flag++;
		}
		count++;
	}
}
void extend24(int w[][8]) {
	int i = 0;
	long long int i_;
	int t = 16;
	while (t < 80) {
		for (i = 0; i < 8; i++) {
			a |= (static_cast<long long>((unsigned int)w[t - 3][7 - i] & 0xFu)) << (i * 4);
		}
		for (i = 0; i < 8; i++) {
			b |= (static_cast<long long>((unsigned int)w[t - 8][7 - i] & 0xFu)) << (i * 4);
		}
		for (i = 0; i < 8; i++) {
			c |= (static_cast<long long>((unsigned int)w[t - 14][7 - i] & 0xFu)) << (i * 4);
		}
		for (i = 0; i < 8; i++) {
			d |= ((unsigned int)w[t - 16][7 - i] & 0xFu) << (i * 4);
		}
		x = a ^ b ^ c ^ d;
		i_ = leftbit(1, x);
		bin(w, i_, t);
		a = b = c = d = 0;
		t++;
	}
}
void bin(int w[][8], long long int p, int t) {
	int temp;
	int i = 0;
	while (p != 0) {
		temp = p % 16;
		p = p / 16;
		w[t][7 - i] = temp;
		i++;
	}
}
void putdata(int w[][8]) {
	int i, j;
	printf("w[0]:");
	for (j = 0; j < 8; j++) {
		printf("%x", w[0][j]);
	}
	printf("\n");
	printf("w[1]:");
	for (j = 0; j < 8; j++) {
		printf("%x", w[1][j]);
	}
	printf("\n");
	printf("w[2]:");
	for (j = 0; j < 8; j++) {
		printf("%x", w[2][j]);
	}
	printf("\n");
	printf("w[14]:");
	for (j = 0; j < 8; j++) {
		printf("%x", w[14][j]);
	}
	printf("\n");
	printf("w[15]:");
	for (j = 0; j < 8; j++) {
		printf("%x", w[15][j]);
	}
	printf("\n");
	printf("w[16]:");
	for (j = 0; j < 8; j++) {
		printf("%x", w[16][j]);
	}
	printf("\n");
	printf("w[79]:");
	for (i = 0; i < 8; i++) {
		printf("%x", w[79][i]);
	}
	printf("\n");
}

