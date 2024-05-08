/*实验要求
（1）把实验三完成的SHA-1杂凑函数设计成可以调用的函数；
（2）原始消息输入字符，调用SHA-1杂凑函数，摘要值输出采用十六进制；
（3）在RSA密钥生成过程中，采用自选的小素数p和q，生成RSA私钥d和公钥e，p和q的乘积大于256（十进制）；
（4）用RSA私钥d签名，公钥e验证，每8比特签名一次，总共做20次签名和验证；
（5）输出界面美观。

测试结果要求：
（1）第1组测试要求：1组固定的数值设置，RSA算法测试， d=493, p=23, q=29，求出公钥e=5;
（2）输入为ASCII码，程序的默认输入为FIPS-180-2中示例的“abc”，最终的消息摘要值为：
a9993e36 4706816a ba3e2571 7850c26c 9cd0d89d
（3）签名过程：
第一个字节：0xa9=169，169493mod(23*29)= 169493mod(667)=78。
第一个字节：0x99=153，153493mod(23*29)= 153493mod(667)=474。
后面的分组以此类推。
（4）第2组输入的ASCII码为各自学号，例如“2015011263”，计算其消息摘要值并进行签名。
（5）截图最好有个人信息，如输出执行的文件的路径有学号或者姓名等。
（6）截图最好前景是运行界面，背景是运行界面未遮住的一部分代码。
（7）代码附在实验报告的后面，排版整齐美观。*/

import java.math.BigInteger;
import java.util.Scanner;

public class rsa_sign {

    public static void main(String[] args) throws Exception {
        String s = "2022121024";
        System.out.println(s);
        System.out.println("SHA1: " + SHA1_24.generate_19(s.getBytes()));
        Scanner scanner = new Scanner(System.in);

        String s1=SHA1_24.generate_19(s.getBytes());
        // 提示用户输入素数p和q
        System.out.println("请输入素数p: ");
        BigInteger p = scanner.nextBigInteger();

        System.out.println("请输入素数q: ");
        BigInteger q = scanner.nextBigInteger();

        // 确保p和q的乘积大于256
        while (p.multiply(q).compareTo(BigInteger.valueOf(256)) <= 0) {
            System.out.println("p和q的乘积需要大于256，请重新输入。");
            System.out.println("请输入素数p: ");
            p = scanner.nextBigInteger();

            System.out.println("请输入素数q: ");
            q = scanner.nextBigInteger();
        }

        // 提示用户输入私钥d
        System.out.println("请输入私钥d: ");
        BigInteger d = scanner.nextBigInteger();

        // 计算n = p * q
        BigInteger n = p.multiply(q);

        // 计算φ(n) = (p-1) * (q-1)
        BigInteger phi = p.subtract(BigInteger.ONE).multiply(q.subtract(BigInteger.ONE));

        // 计算公钥e
        BigInteger e = d.modInverse(phi);

        // 输出公钥e
        System.out.println("公钥e: " + e);

        scanner.close();



        // 将消息摘要分成每两个字符一组，每组代表一个字节
        for (int i = 0; i < s1.length(); i += 2) {
            String hexPair = s1.substring(i, i + 2);
            int decimalValue = Integer.parseInt(hexPair, 16);
            BigInteger messageBlock = BigInteger.valueOf(decimalValue);

            // 使用私钥d进行签名
            BigInteger signatureBlock = messageBlock.modPow(d, n);

            // 输出签名结果
            System.out.println("第" + ((i / 2) + 1) + "个字节：" + "0x" + hexPair + "=" + decimalValue + "，" +
                    decimalValue + "^" + d + "mod(" + n + ")=" + signatureBlock + "。");
        }

        // 公钥e的值，用于验证签名


        // 验证签名
        for (int i = 0; i < s1.length(); i += 2) {
            String hexPair = s1.substring(i, i + 2);
            int decimalValue = Integer.parseInt(hexPair, 16);
            BigInteger messageBlock = BigInteger.valueOf(decimalValue);

            // 使用私钥d进行签名
            BigInteger signatureBlock = messageBlock.modPow(d, n);

            // 使用公钥e进行验证
            BigInteger verifiedBlock = signatureBlock.modPow(e, n);

            // 输出验证结果
            System.out.println("验证第" + ((i / 2) + 1) + "个字节：" + "0x" + hexPair + "=" + decimalValue + "，" +
                    "验证结果：" + verifiedBlock + "。");
        }
    }
}

class SHA1_24 {
    static String generate_19(byte[] dataBytes) throws Exception {
        int[] blks = new int[(((dataBytes.length + 8) >> 6) + 1) * 16];
        int i;

        for (i = 0; i < dataBytes.length; i++) {
            blks[i >> 2] |= dataBytes[i] << (24 - (i % 4) * 8);
        }

        blks[i >> 2] |= 0x80 << (24 - (i % 4) * 8);
        blks[blks.length - 1] = dataBytes.length * 8;

        int[] w = new int[80];

        int a = 0x67452301;
        int b = 0xEFCDAB89;
        int c = 0x98BADCFE;
        int d = 0x10325476;
        int e = 0xC3D2E1F0;

        for (i = 0; i < blks.length; i += 16) {
            for (int j = 0; j < 16; j++) {
                w[j] = blks[i + j];
            }

            for (int j = 16; j < 80; j++) {
                w[j] = (w[j - 3] ^ w[j - 8] ^ w[j - 14] ^ w[j - 16]);
                w[j] = (w[j] << 1) | (w[j] >>> 31);
            }

            int temp;
            int a2 = a;
            int b2 = b;
            int c2 = c;
            int d2 = d;
            int e2 = e;

            for (int j = 0; j < 80; j++) {
                temp = ((a2 << 5) | (a2 >>> 27)) + e2 + w[j] + ((j < 20) ? 0x5A827999 + ((b2 & c2) | ((~b2) & d2))
                        : (j < 40) ? 0x6ED9EBA1 + (b2 ^ c2 ^ d2)
                        : (j < 60) ? 0x8F1BBCDC + ((b2 & c2) | (b2 & d2) | (c2 & d2))
                        : 0xCA62C1D6 + (b2 ^ c2 ^ d2));
                e2 = d2;
                d2 = c2;
                c2 = (b2 << 30) | (b2 >>> 2);
                b2 = a2;
                a2 = temp;
            }

            a += a2;
            b += b2;
            c += c2;
            d += d2;
            e += e2;
        }


        return String.format("%08x%08x%08x%08x%08x", a, b, c, d, e);
    }

}
