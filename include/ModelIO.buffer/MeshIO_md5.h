#ifndef MD5_CALCULATOR_H
#define MD5_CALCULATOR_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

// 定义一些常量
const unsigned int S[4][4] = {
    {7, 12, 17, 22},
    {5,  9, 14, 20},
    {4, 11, 16, 23},
    {6, 10, 15, 21}
};
const unsigned int K[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x2678e6fd, 0xa2bfe8a1,
    0xc76c51a3, 0x71c337dd, 0x1f83d9ab, 0x29adceea,
    0xb00327c8, 0x98df8b00, 0xef1f191e, 0xe39492a6,
    0x9d618830, 0x498ea3e5, 0x3b10d28a, 0xd15d2c09,
    0xf4d50d87, 0x391c0cb3, 0x43186745, 0xa53a76d8,
    0x500c6d83, 0x7d850099, 0x2441453b, 0x0d95748f,
    0x6d9d6122, 0x92722c85, 0x223a8025, 0x53b0a992,
    0x7479632e, 0x0b3e2b8a, 0x40652319, 0x7766a0ab,
    0x62950c60, 0x5a9fe429, 0x106aa070, 0x32d2efba,
    0x03601104, 0xd6990624, 0x983e5152, 0x597f299c,
    0x2b040329, 0x86cb3077, 0x60902e59, 0x74c4756a,
    0xa4904230, 0xf75095dd, 0x1983d9ab, 0x076295c7
};
const unsigned int A0 = 0x67452301;
const unsigned int B0 = 0xefcdab89;
const unsigned int C0 = 0x98badcfe;
const unsigned int D0 = 0x10325476;

inline unsigned int leftRotate(unsigned int x, unsigned int n) {
        return (x << n) | (x >> (32 - n));
}

inline unsigned int F(unsigned int b, unsigned int c, unsigned int d) {
        return (b & c) | ((~b) & d);
}

inline unsigned int G(unsigned int b, unsigned int c, unsigned int d) {
        return (b & d) | (c & (~d));
}

inline unsigned int H(unsigned int b, unsigned int c, unsigned int d) {
        return b ^ c ^ d;
}

inline unsigned int I(unsigned int b, unsigned int c, unsigned int d) {
        return c ^ (b | (~d));
}

inline void md5ProcessBlock(unsigned int* M, unsigned int& A, unsigned int& B, unsigned int& C, unsigned int& D) {
        unsigned int a = A;
        unsigned int b = B;
        unsigned int c = C;
        unsigned int d = D;

        for (int i = 0; i < 64; ++i) {
                unsigned int f, g;
                if (i < 16) {
                        f = F(b, c, d);
                        g = i;
                }
                else if (i < 32) {
                        f = G(b, c, d);
                        g = (5 * i + 1) % 16;
                }
                else if (i < 48) {
                        f = H(b, c, d);
                        g = (3 * i + 5) % 16;
                }
                else {
                        f = I(b, c, d);
                        g = (7 * i) % 16;
                }

                unsigned int temp = d;
                d = c;
                c = b;
                b = leftRotate(a + f + K[i] + M[g], S[i / 16][i % 4]) + b;
                a = temp;
        }

        A += a;
        B += b;
        C += c;
        D += d;
}

inline void padMessage(const unsigned char* originalMessage, unsigned int originalLength, unsigned char*& paddedMessage, unsigned int& paddedLength) {
        // 计算填充后的长度
        paddedLength = originalLength;
        while (paddedLength % 64 != 56) {
                paddedLength++;
        }
        paddedLength += 8;

        // 分配填充后的数据内存
        paddedMessage = new unsigned char[paddedLength];
        // 复制原始数据
        for (unsigned int i = 0; i < originalLength; ++i) {
                paddedMessage[i] = originalMessage[i];
        }
        // 填充1
        paddedMessage[originalLength] = 0x80;
        // 填充0
        for (unsigned int i = originalLength + 1; i < paddedLength - 8; ++i) {
                paddedMessage[i] = 0x00;
        }
        // 添加原始数据长度（以位为单位）
        unsigned long long bitLength = originalLength * 8;
        for (int i = 0; i < 8; ++i) {
                paddedMessage[paddedLength - 8 + i] = (bitLength >> (8 * (7 - i))) & 0xFF;
        }
}

inline std::string calculateMD5(const std::string& fileName) {
        std::ifstream file(fileName, std::ios::binary);
        if (!file) {
                std::cerr << "无法打开文件: " << fileName << std::endl;
                return "";
        }

        // 读取文件内容到内存
        file.seekg(0, std::ios::end);
        unsigned int originalLength = file.tellg();
        file.seekg(0, std::ios::beg);
        unsigned char* originalMessage = new unsigned char[originalLength];
        file.read(reinterpret_cast<char*>(originalMessage), originalLength);

        // 填充数据
        unsigned char* paddedMessage;
        unsigned int paddedLength;
        padMessage(originalMessage, originalLength, paddedMessage, paddedLength);

        // 初始化MD5寄存器
        unsigned int A = A0;
        unsigned int B = B0;
        unsigned int C = C0;
        unsigned int D = D0;

        // 处理分组
        for (unsigned int i = 0; i < paddedLength; i += 64) {
                unsigned int* M = reinterpret_cast<unsigned int*>(paddedMessage + i);
                md5ProcessBlock(M, A, B, C, D);
        }

        // 释放内存
        delete[] originalMessage;
        delete[] paddedMessage;

        // 构建MD5结果字符串
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        ss << std::setw(8) << A << std::setw(8) << B << std::setw(8) << C << std::setw(8) << D;
        return ss.str();
}

#endif