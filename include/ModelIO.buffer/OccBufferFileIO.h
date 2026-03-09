#ifndef OCC_BUFFER_FILE_IO_H
#define OCC_BUFFER_FILE_IO_H

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>

struct occ_perform_buffer_t {
        std::vector<double> points;
        std::vector<double> normals;
        std::vector<int> elements;
        std::set<std::vector<int>> edges;
};

// 写结构体数组和MD5字符串到文件，成功返回true，失败返回false
inline bool writeOccPerformBuffer(const std::vector<occ_perform_buffer_t>& buffers, const std::string& md5, const char* fileName) {
        try {
                std::ofstream outFile(fileName, std::ios::binary);
                if (outFile) {
                        // 写入结构体数组大小
                        size_t sizeBuffers = buffers.size();
                        outFile.write(reinterpret_cast<const char*>(&sizeBuffers), sizeof(sizeBuffers));

                        for (const auto& buffer : buffers) {
                                // 写入points向量大小
                                size_t sizePoints = buffer.points.size();
                                outFile.write(reinterpret_cast<const char*>(&sizePoints), sizeof(sizePoints));
                                // 写入points数据
                                outFile.write(reinterpret_cast<const char*>(buffer.points.data()), buffer.points.size() * sizeof(double));

                                // 写入normals向量大小
                                size_t sizeNormals = buffer.normals.size();
                                outFile.write(reinterpret_cast<const char*>(&sizeNormals), sizeof(sizeNormals));
                                // 写入normals数据
                                outFile.write(reinterpret_cast<const char*>(buffer.normals.data()), buffer.normals.size() * sizeof(double));

                                // 写入elements向量大小
                                size_t sizeElements = buffer.elements.size();
                                outFile.write(reinterpret_cast<const char*>(&sizeElements), sizeof(sizeElements));
                                // 写入elements数据
                                outFile.write(reinterpret_cast<const char*>(buffer.elements.data()), buffer.elements.size() * sizeof(int));

                                // 写入edges集合大小
                                size_t sizeEdges = buffer.edges.size();
                                outFile.write(reinterpret_cast<const char*>(&sizeEdges), sizeof(sizeEdges));
                                // 写入edges数据
                                for (const auto& edge : buffer.edges) {
                                        // 写入每个边向量的大小
                                        size_t edgeSize = edge.size();
                                        outFile.write(reinterpret_cast<const char*>(&edgeSize), sizeof(edgeSize));
                                        // 写入边向量的数据
                                        outFile.write(reinterpret_cast<const char*>(edge.data()), edge.size() * sizeof(int));
                                }
                        }

                        // 写入MD5字符串长度
                        size_t md5Size = md5.length();
                        outFile.write(reinterpret_cast<const char*>(&md5Size), sizeof(md5Size));
                        // 写入MD5字符串数据
                        outFile.write(md5.c_str(), md5.length());

                        outFile.close();
                        return true;
                }
                else {
                        std::cerr << "Failed to open the file for writing: " << fileName << std::endl;
                        return false;
                }
        }
        catch (const std::exception& e) {
                std::cerr << "An exception occurred while writing the file: " << e.what() << std::endl;
                return false;
        }
}

// 从文件读取结构体数组和MD5字符串，成功返回true，失败返回false
inline bool readOccPerformBuffer(std::vector<occ_perform_buffer_t>& buffers, std::string& md5, const char* fileName) {
        try {
                std::ifstream inFile(fileName, std::ios::binary);
                if (inFile) {
                        // 读取结构体数组大小
                        size_t sizeBuffers;
                        inFile.read(reinterpret_cast<char*>(&sizeBuffers), sizeof(sizeBuffers));
                        buffers.resize(sizeBuffers);

                        for (auto& buffer : buffers) {
                                // 读取points向量大小
                                size_t sizePoints;
                                inFile.read(reinterpret_cast<char*>(&sizePoints), sizeof(sizePoints));
                                buffer.points.resize(sizePoints);
                                // 读取points数据
                                inFile.read(reinterpret_cast<char*>(buffer.points.data()), buffer.points.size() * sizeof(double));

                                // 读取normals向量大小
                                size_t sizeNormals;
                                inFile.read(reinterpret_cast<char*>(&sizeNormals), sizeof(sizeNormals));
                                buffer.normals.resize(sizeNormals);
                                // 读取normals数据
                                inFile.read(reinterpret_cast<char*>(buffer.normals.data()), buffer.normals.size() * sizeof(double));

                                // 读取elements向量大小
                                size_t sizeElements;
                                inFile.read(reinterpret_cast<char*>(&sizeElements), sizeof(sizeElements));
                                buffer.elements.resize(sizeElements);
                                // 读取elements数据
                                inFile.read(reinterpret_cast<char*>(buffer.elements.data()), buffer.elements.size() * sizeof(int));

                                // 读取edges集合大小
                                size_t sizeEdges;
                                inFile.read(reinterpret_cast<char*>(&sizeEdges), sizeof(sizeEdges));
                                for (size_t i = 0; i < sizeEdges; ++i) {
                                        // 读取每个边向量的大小
                                        size_t edgeSize;
                                        inFile.read(reinterpret_cast<char*>(&edgeSize), sizeof(edgeSize));
                                        std::vector<int> edge(edgeSize);
                                        // 读取边向量的数据
                                        inFile.read(reinterpret_cast<char*>(edge.data()), edge.size() * sizeof(int));
                                        buffer.edges.insert(edge);
                                }
                        }

                        // 读取MD5字符串长度
                        size_t md5Size;
                        inFile.read(reinterpret_cast<char*>(&md5Size), sizeof(md5Size));
                        // 读取MD5字符串数据
                        char* md5Str = new char[md5Size + 1];
                        inFile.read(md5Str, md5Size);
                        md5Str[md5Size] = '\0';
                        md5 = md5Str;
                        delete[] md5Str;

                        inFile.close();
                        return true;
                }
                else {
                        std::cerr << "Failed to open the file for reading: " << fileName << std::endl;
                        return false;
                }
        }
        catch (const std::exception& e) {
                std::cerr << "An exception occurred while reading the file: " << e.what() << std::endl;
                return false;
        }
}

#endif#pragma once
