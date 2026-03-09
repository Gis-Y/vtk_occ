/**
 * @file TriangleMeshIsoLine.h
 * @brief 三角形网格等值线分割库 - 用于根据标量场等值线分割三角形网格
 *
 * ========================================================================
 * 主要功能概述
 * ========================================================================
 * 本库提供了一套完整的工具，用于对三角形网格进行等值线分割。主要功能包括：
 * 1. 三角形网格的等值线分割 - 根据指定的等值线值将三角形网格分割成多个子区域
 * 2. Nastran格式支持 - 读取和写入Nastran有限元网格文件格式
 * 3. 三维几何处理 - 在三维空间中处理三角形网格，包括坐标变换和法线计算
 * 4. 面积守恒分割 - 确保分割前后总面积保持不变
 * 5. 节点法线计算 - 自动计算和插值节点法线
 *
 * ========================================================================
 * 核心数据结构
 * ========================================================================
 *
 * 1. Point3D - 三维点结构
 *    - 包含坐标(x,y,z)、标量值(value)和法线向量(nx,ny,nz)
 *    - 支持向量运算：加减、点积、叉积、归一化等
 *    - 重载了比较运算符，支持容差判断
 *
 * 2. Edge - 边结构
 *    - 包含两个端点p1和p2
 *    - 重载了相等运算符（无向边）
 *
 * 3. Polygon - 多边形结构
 *    - 存储凸多边形的顶点和边
 *    - 包含点包含检测、面积计算等方法
 *
 * 4. TriangleMesh - 三角形网格类
 *    - 主要数据结构，用于存储和管理三角形网格
 *    - 支持节点合并、法线计算、网格拆分等操作
 *
 * ========================================================================
 * 核心算法类
 * ========================================================================
 *
 * 1. CoordinateTransformer - 坐标变换类
 *    - 功能：在全局3D坐标和局部2D坐标之间转换
 *    - 原理：以三角形平面建立局部坐标系
 *      * 原点：三角形第一个顶点
 *      * X轴：p1→p2方向
 *      * Z轴：三角形法线
 *      * Y轴：Z×X（右手坐标系）
 *    - 作用：将3D三角形投影到2D平面进行处理
 *
 * 2. TriangleSplitter - 三角形拆分类
 *    - 输入：三角形三个顶点 + 等值线值列表
 *    - 处理流程：
 *      1. 建立局部坐标系
 *      2. 在边上插入等值点
 *      3. 连接等值点形成分割边
 *      4. 用分割边切割多边形
 *      5. 将结果转换回全局坐标
 *    - 特点：
 *      * 避免插值点太靠近端点
 *      * 支持多个等值线同时分割
 *
 * ========================================================================
 * 基本使用流程
 * ========================================================================
 *
 * 【示例1：读取Nastran文件并进行等值线分割】
 *
 *   // 1. 创建网格对象
 *   TriangleMesh mesh;
 *
 *   // 2. 从Nastran文件读取网格
 *   NastranReadResult result = mesh.readFromNastran("input.nas", true);
 *   if (!result.success) {
 *       cerr << "读取失败: " << result.error_message << endl;
 *       return;
 *   }
 *
 *   // 3. 设置节点物理量（例如x坐标）
 *   for (size_t i = 0; i < mesh.getNodeCount(); i++) {
 *       vector<double> coords = mesh.getNodeCoords(i);
 *       mesh.setNodeValue(i, coords[0]); // 使用x坐标作为物理量
 *   }
 *
 *   // 4. 定义等值线值
 *   vector<double> isoValues = {0.1, 0.2, 0.3, 0.4, 0.5};
 *
 *   // 5. 分割网格
 *   TriangleMesh splitMesh = mesh.splitByIsolines(isoValues);
 *
 *   // 6. 写入结果
 *   NastranWriteOptions options;
 *   options.header_comment = "等值线分割结果";
 *   splitMesh.writeToNastran("output.nas", options);
 *
 *
 * 【示例2：创建简单网格并分割】
 *
 *   // 1. 创建网格并添加节点
 *   TriangleMesh mesh;
 *   int n1 = mesh.addNode(0, 0, 0, 0); // x,y,z,值
 *   int n2 = mesh.addNode(1, 0, 0, 10);
 *   int n3 = mesh.addNode(0.5, 1, 0, 5);
 *
 *   // 2. 添加三角形
 *   mesh.addTriangle(n1, n2, n3);
 *
 *   // 3. 自动计算法线
 *   mesh.calculateNodeNormals();
 *
 *   // 4. 定义等值线并分割
 *   vector<double> isoValues = {2, 4, 6, 8};
 *   TriangleMesh result = mesh.splitByIsolines(isoValues);
 *
 * ========================================================================
 * 文件格式支持
 * ========================================================================
 *
 * 1. Nastran格式：
 *    - 读取：GRID（节点）、CTRIA3（三角形）、CQUAD4（四边形，自动转为三角形）
 *    - 写入：生成包含节点、单元和温度卡片的结果文件
 *
 * 2. CSV格式（节点值）：
 *    - 可通过readNodeValuesFromCSV()读取节点物理量值
 *
 * ========================================================================
 * 测试函数
 * ========================================================================
 *
 * 库提供了多个测试函数，可直接调用以验证功能：
 *
 * 1. test3DTriangle() - 测试3D三角形拆分算法
 * 2. testTriangleMeshAdvanced() - 测试改进的三角形网格类
 * 3. testIsolineSplit() - 实测用例：基于x坐标等值线拆分
 * 4. testIsolineSplitByDistance() - 基于距离原点的距离等值线拆分
 *
 * 使用示例：
 *   TriangleMeshIsoLine::testIsolineSplit(); // 运行实测用例
 *
 * ========================================================================
 * 注意事项
 * ========================================================================
 *
 * 1. 数值稳定性：
 *    - 使用容差(1e-9)处理浮点数比较
 *    - 防止除零错误
 *    - 处理退化情况（三点共线）
 *
 * 2. 内存管理：
 *    - 注意大网格可能导致的内存消耗
 *    - 分割后的网格节点和三角形数量可能显著增加
 *
 * 3. 性能考虑：
 *    - 对于大型网格，分割操作可能较耗时
 *    - 建议先在小规模数据上测试
 *
 * 4. 坐标系：
 *    - 所有操作在三维空间中进行
 *    - 内部使用局部坐标系进行2D处理
 *    - 保持面积守恒
 *
 * ========================================================================
 * 主要函数说明
 * ========================================================================
 *
 * 【TriangleMesh类关键方法】
 *
 * 1. 网格构建：
 *    - addNode(x, y, z, value, nx, ny, nz) - 添加节点（自动合并重复节点）
 *    - addTriangle(n1, n2, n3) - 添加三角形
 *    - clear() - 清空网格
 *
 * 2. 网格操作：
 *    - splitByIsolines(isoValues) - 根据等值线分割网格
 *    - extractIsolineEdges(isoValue) - 提取单条等值线的边
 *    - calculateNodeNormals() - 自动计算节点法线
 *
 * 3. 文件I/O：
 *    - readFromNastran(filename, mergeDuplicateNodes) - 读取Nastran文件
 *    - writeToNastran(filename, options) - 写入Nastran文件
 *    - readNodeValuesFromCSV(filename) - 从CSV读取节点值
 *
 * 4. 查询功能：
 *    - getNodeCount() - 获取节点数量
 *    - getTriangleCount() - 获取三角形数量
 *    - calculateTotalArea() - 计算网格总面积
 *    - getBoundingBox() - 获取包围盒
 *    - getValueRange() - 获取节点值范围
 *
 * 【TriangleSplitter类关键方法】
 *
 * - TriangleSplitter(p1, p2, p3, isoList) - 构造函数
 * - split() - 执行分割，返回多边形列表
 * - printTransformInfo() - 打印坐标变换信息
 *
 * ========================================================================
 * 错误处理
 * ========================================================================
 *
 * 1. 函数返回错误码：
 *    - NastranReadResult::success - 读取是否成功
 *    - NastranWriteResult::success - 写入是否成功
 *
 * 2. 验证网格有效性：
 *    - isValid() - 检查网格数据是否有效
 *
 * 3. 异常处理：
 *    - 大部分函数使用返回值或错误消息，不抛出异常
 *    - 建议在使用前检查返回值
 *
 * ========================================================================
 * 开发说明
 * ========================================================================
 *
 * 命名空间：TriangleMeshIsoLine
 * 作者：zhangxk@dlut.edu.cn by Deepseek Support
 * 版本：1.0
 * 依赖：标准C++库（vector, map, algorithm, cmath, fstream等）
 * 编译要求：支持C++11或更高版本
 *
 * ========================================================================
 * 许可证
 * ========================================================================
 *
 * 本代码库属于大连软魂科技有限公司商业许可证，使用者应遵循相关法律法规。
  *
 * @copyright 大连软魂科技有限公司
 * @date 2026
 */


#ifndef TRIANGLEMESHISOLINE_H
#define TRIANGLEMESHISOLINE_H

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <memory>
#include <cassert>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iterator>
#include <queue>
#include <limits>

namespace TriangleMeshIsoLine {

    using namespace std;

    // 三维点结构
    struct Point3D {
        double x, y, z;
        double value; // 标量物理量
        double nx, ny, nz; // 法线向量

        Point3D(double x = 0, double y = 0, double z = 0, double val = 0,
            double nx = 0, double ny = 0, double nz = 1.0)
            : x(x), y(y), z(z), value(val), nx(nx), ny(ny), nz(nz) {}

        inline bool operator==(const Point3D& other) const {
            return fabs(x - other.x) < 1e-9 &&
                fabs(y - other.y) < 1e-9 &&
                fabs(z - other.z) < 1e-9;
        }

        inline bool operator<(const Point3D& other) const {
            if (fabs(x - other.x) > 1e-9) return x < other.x;
            if (fabs(y - other.y) > 1e-9) return y < other.y;
            if (fabs(z - other.z) > 1e-9) return z < other.z;
            return false;
        }

        // 向量减法
        inline Point3D operator-(const Point3D& other) const {
            return Point3D(x - other.x, y - other.y, z - other.z, 0,
                nx - other.nx, ny - other.ny, nz - other.nz);
        }

        // 向量加法
        inline Point3D operator+(const Point3D& other) const {
            return Point3D(x + other.x, y + other.y, z + other.z, 0,
                nx + other.nx, ny + other.ny, nz + other.nz);
        }

        // 标量乘法
        inline Point3D operator*(double scalar) const {
            return Point3D(x * scalar, y * scalar, z * scalar, 0,
                nx * scalar, ny * scalar, nz * scalar);
        }

        // 向量点积
        inline double dot(const Point3D& other) const {
            return x * other.x + y * other.y + z * other.z;
        }

        // 向量叉积
        inline Point3D cross(const Point3D& other) const {
            return Point3D(
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x,
                0,
                0, 0, 0  // 叉积结果的法线信息不继承
            );
        }

        // 向量长度
        inline double length() const {
            return sqrt(x * x + y * y + z * z);
        }

        // 法线长度
        inline double normalLength() const {
            return sqrt(nx * nx + ny * ny + nz * nz);
        }

        // 单位化向量
        inline Point3D normalized() const {
            double len = length();
            if (len < 1e-12) return *this;
            return Point3D(x / len, y / len, z / len, 0,
                nx / len, ny / len, nz / len);
        }

        // 单位化法线
        inline Point3D normalNormalized() const {
            double len = normalLength();
            if (len < 1e-12) return Point3D(x, y, z, value, 0, 0, 1.0);
            return Point3D(x, y, z, value, nx / len, ny / len, nz / len);
        }

        // 设置法线
        inline void setNormal(double nx_val, double ny_val, double nz_val) {
            nx = nx_val;
            ny = ny_val;
            nz = nz_val;
        }

        // 获取法线
        inline Point3D getNormal() const {
            return Point3D(0, 0, 0, 0, nx, ny, nz);
        }
    };

    // 边结构
    struct Edge {
        Point3D p1, p2;

        Edge(const Point3D& pt1, const Point3D& pt2) : p1(pt1), p2(pt2) {}

        inline bool operator==(const Edge& other) const {
            return (p1 == other.p1 && p2 == other.p2) || (p1 == other.p2 && p2 == other.p1);
        }
    };

    // 多边形结构（凸多边形）
    struct Polygon {
        vector<Point3D> vertices;
        vector<Edge> edges;

        Polygon() {}

        Polygon(const vector<Point3D>& verts) : vertices(verts) {
            buildEdges();
        }

        inline void addVertex(const Point3D& p) {
            vertices.push_back(p);
            buildEdges();
        }

        inline void buildEdges() {
            edges.clear();
            if (vertices.size() < 2) return;

            for (size_t i = 0; i < vertices.size(); i++) {
                size_t j = (i + 1) % vertices.size();
                edges.push_back(Edge(vertices[i], vertices[j]));
            }
        }

        inline bool contains(const Point3D& p, bool includeBoundary = true) const {
            if (vertices.size() < 3) return false;

            for (const auto& v : vertices) {
                if (v == p) return true;
            }

            if (includeBoundary) {
                for (const auto& edge : edges) {
                    if (isPointOnEdge(p, edge.p1, edge.p2)) {
                        return true;
                    }
                }
            }

            // 由于现在是在2D平面上工作，我们只需要检查2D投影
            // 注意：这个函数现在在局部坐标系中调用，所以z坐标应该为0或接近0
            int n = static_cast<int>(vertices.size());
            for (int i = 0; i < n; i++) {
                int j = (i + 1) % n;
                double cross = (vertices[j].x - vertices[i].x) * (p.y - vertices[i].y) -
                    (vertices[j].y - vertices[i].y) * (p.x - vertices[i].x);
                if (cross < -1e-9) {
                    return false;
                }
            }
            return true;
        }

        inline bool containsEdge(const Edge& edge) const {
            return contains(edge.p1, true) && contains(edge.p2, true);
        }

        inline double area() const {
            if (vertices.size() < 3) return 0.0;

            // 计算3D多边形的面积（假设多边形是平面多边形）
            // 使用三角形扇方法
            double area = 0.0;
            size_t n = vertices.size();

            if (n == 3) {
                // 对于三角形，使用叉积计算面积
                Point3D v1 = vertices[1] - vertices[0];
                Point3D v2 = vertices[2] - vertices[0];
                Point3D cross = v1.cross(v2);
                area = cross.length() / 2.0;
            }
            else {
                // 对于多边形，分解为三角形
                Point3D center;
                for (const auto& v : vertices) {
                    center = center + v;
                }
                center = center * (1.0 / n);

                for (size_t i = 0; i < n; i++) {
                    size_t j = (i + 1) % n;
                    Point3D v1 = vertices[i] - center;
                    Point3D v2 = vertices[j] - center;
                    Point3D cross = v1.cross(v2);
                    area += cross.length() / 2.0;
                }
            }

            return area;
        }

    private:
        inline bool isPointOnEdge(const Point3D& p, const Point3D& a, const Point3D& b) const {
            // 在3D空间中检查点是否在边上
            Point3D ab = b - a;
            Point3D ap = p - a;

            // 检查点是否在直线AB上
            Point3D cross = ab.cross(ap);
            if (cross.length() > 1e-9) return false;

            // 检查点是否在线段AB内
            double dot = ap.dot(ab);
            double len2 = ab.dot(ab);

            return dot >= -1e-9 && dot <= len2 + 1e-9;
        }
    };

    // 坐标变换类
    class CoordinateTransformer {
    private:
        Point3D origin;      // 局部坐标系原点（对应全局坐标的p1）
        Point3D x_axis;      // 局部坐标系x轴单位向量
        Point3D y_axis;      // 局部坐标系y轴单位向量
        Point3D z_axis;      // 局部坐标系z轴单位向量（三角形法向量）

    public:
        inline CoordinateTransformer(const Point3D& p1, const Point3D& p2, const Point3D& p3) {
            // 构建局部坐标系
            origin = p1;  // 以p1为原点

            // x轴: p1->p2方向
            Point3D vec_x = p2 - p1;
            x_axis = vec_x.normalized();

            // 临时向量: p1->p3
            Point3D vec_temp = p3 - p1;

            // z轴: x轴与临时向量的叉积（三角形法向量）
            Point3D vec_z = vec_x.cross(vec_temp);
            z_axis = vec_z.normalized();

            // y轴: z轴与x轴的叉积（确保右手坐标系）
            Point3D vec_y = z_axis.cross(x_axis);
            y_axis = vec_y.normalized();

            // 验证坐标系是否有效
            if (x_axis.length() < 1e-12 || y_axis.length() < 1e-12 || z_axis.length() < 1e-12) {
                // 退化情况：三点共线或重合
                // 使用默认坐标系
                x_axis = Point3D(1, 0, 0, 0);
                y_axis = Point3D(0, 1, 0, 0);
                z_axis = Point3D(0, 0, 1, 0);
            }
        }

        // 全局坐标转局部坐标（2D，z=0）
        inline Point3D globalToLocal(const Point3D& global) const {
            Point3D vec = global - origin;

            // 计算在局部坐标系中的坐标
            double local_x = vec.dot(x_axis);
            double local_y = vec.dot(y_axis);
            double local_z = vec.dot(z_axis);  // 理论上应该接近0

            // 返回局部2D坐标（z设为0），保留原始值和法线
            Point3D local(local_x, local_y, 0, global.value);
            // 法线也需要转换到局部坐标系
            local.nx = global.nx * x_axis.x + global.ny * x_axis.y + global.nz * x_axis.z;
            local.ny = global.nx * y_axis.x + global.ny * y_axis.y + global.nz * y_axis.z;
            local.nz = global.nx * z_axis.x + global.ny * z_axis.y + global.nz * z_axis.z;
            return local;
        }

        // 局部坐标（2D）转全局坐标
        inline Point3D localToGlobal(const Point3D& local) const {
            // local.z应该为0，但我们忽略它
            Point3D global = origin +
                x_axis * local.x +
                y_axis * local.y;

            // 保留原始值和法线
            global.value = local.value;

            // 法线从局部坐标系转换回全局坐标系
            global.nx = local.nx * x_axis.x + local.ny * y_axis.x + local.nz * z_axis.x;
            global.ny = local.nx * x_axis.y + local.ny * y_axis.y + local.nz * z_axis.y;
            global.nz = local.nx * x_axis.z + local.ny * y_axis.z + local.nz * z_axis.z;

            return global;
        }

        // 获取变换信息（调试用）
        inline void printTransformInfo() const {
            cout << "坐标变换信息:" << endl;
            cout << "  原点: (" << origin.x << ", " << origin.y << ", " << origin.z << ")" << endl;
            cout << "  X轴: (" << x_axis.x << ", " << x_axis.y << ", " << x_axis.z << ")" << endl;
            cout << "  Y轴: (" << y_axis.x << ", " << y_axis.y << ", " << y_axis.z << ")" << endl;
            cout << "  Z轴: (" << z_axis.x << ", " << z_axis.y << ", " << z_axis.z << ")" << endl;
        }
    };

    // 三角形拆分类
    class TriangleSplitter {
    private:
        Point3D vertices[3];
        vector<double> isoValues;
        map<double, vector<Point3D>> isoPoints;
        vector<Edge> splitEdges;
        vector<Polygon> resultPolygons;
        CoordinateTransformer* transformer;

    public:
        inline TriangleSplitter(const Point3D& p1, const Point3D& p2, const Point3D& p3,
            const vector<double>& isoList) {
            vertices[0] = p1;
            vertices[1] = p2;
            vertices[2] = p3;
            isoValues = isoList;
            sort(isoValues.begin(), isoValues.end());

            // 创建坐标变换器
            transformer = new CoordinateTransformer(p1, p2, p3);
        }

        inline ~TriangleSplitter() {
            delete transformer;
        }

        inline vector<Polygon> split() {
            isoPoints.clear();
            splitEdges.clear();
            resultPolygons.clear();

            // 将全局坐标转换为局部坐标
            Point3D local_vertices[3];
            for (int i = 0; i < 3; i++) {
                local_vertices[i] = transformer->globalToLocal(vertices[i]);
            }

            // 在局部坐标系中执行分割
            insertEdgePoints(local_vertices);
            connectIsoPoints();
            buildPolygons(local_vertices);

            // 将结果从局部坐标转换回全局坐标
            for (auto& poly : resultPolygons) {
                for (auto& vertex : poly.vertices) {
                    vertex = transformer->localToGlobal(vertex);
                }
                poly.buildEdges();  // 重新构建边
            }

            return resultPolygons;
        }

        inline void printSplitEdges() const {
            cout << "分割边数量: " << splitEdges.size() << endl;
            for (size_t i = 0; i < splitEdges.size(); i++) {
                const Edge& edge = splitEdges[i];
                cout << "边 " << i << ": (" << edge.p1.x << "," << edge.p1.y << "," << edge.p1.z << ") -> ("
                    << edge.p2.x << "," << edge.p2.y << "," << edge.p2.z << ")" << endl;
            }
        }

        inline size_t getSplitEdgesCount() const {
            return splitEdges.size();
        }

        inline void printTransformInfo() const {
            transformer->printTransformInfo();
        }

    private:
        inline void insertEdgePoints(const Point3D local_vertices[3]) {
            vector<Edge> triangleEdges = {
                Edge(local_vertices[0], local_vertices[1]),
                Edge(local_vertices[1], local_vertices[2]),
                Edge(local_vertices[2], local_vertices[0])
            };

            for (double isoVal : isoValues) {
                for (const auto& edge : triangleEdges) {
                    double v1 = edge.p1.value;
                    double v2 = edge.p2.value;
                    double minVal = min(v1, v2);
                    double maxVal = max(v1, v2);

                    if (isoVal >= minVal - 1e-9 && isoVal <= maxVal + 1e-9) {
                        if (fabs(isoVal - v1) < 1e-9) {
                            // 等值线恰好通过端点p1
                            isoPoints[isoVal].push_back(edge.p1);
                        }
                        else if (fabs(isoVal - v2) < 1e-9) {
                            // 等值线恰好通过端点p2
                            isoPoints[isoVal].push_back(edge.p2);
                        }
                        else {
                            // 计算插值参数t
                            double t = (isoVal - v1) / (v2 - v1);

                            // 计算插值点坐标
                            double new_x = edge.p1.x + t * (edge.p2.x - edge.p1.x);
                            double new_y = edge.p1.y + t * (edge.p2.y - edge.p1.y);
                            double new_z = edge.p1.z + t * (edge.p2.z - edge.p1.z);

                            // 计算插值点法线（使用两端点法线插值）
                            double new_nx = edge.p1.nx + t * (edge.p2.nx - edge.p1.nx);
                            double new_ny = edge.p1.ny + t * (edge.p2.ny - edge.p1.ny);
                            double new_nz = edge.p1.nz + t * (edge.p2.nz - edge.p1.nz);

                            // 创建新点
                            Point3D newPoint(new_x, new_y, new_z, isoVal, new_nx, new_ny, new_nz);

                            // 计算边长度
                            double edgeLength = sqrt(
                                (edge.p2.x - edge.p1.x) * (edge.p2.x - edge.p1.x) +
                                (edge.p2.y - edge.p1.y) * (edge.p2.y - edge.p1.y) +
                                (edge.p2.z - edge.p1.z) * (edge.p2.z - edge.p1.z)
                            );

                            // 计算插值点到两个端点的距离
                            double distToP1 = sqrt(
                                (newPoint.x - edge.p1.x) * (newPoint.x - edge.p1.x) +
                                (newPoint.y - edge.p1.y) * (newPoint.y - edge.p1.y) +
                                (newPoint.z - edge.p1.z) * (newPoint.z - edge.p1.z)
                            );

                            double distToP2 = sqrt(
                                (newPoint.x - edge.p2.x) * (newPoint.x - edge.p2.x) +
                                (newPoint.y - edge.p2.y) * (newPoint.y - edge.p2.y) +
                                (newPoint.z - edge.p2.z) * (newPoint.z - edge.p2.z)
                            );

                            // 检查是否非常接近端点（距离小于边长的1/50）
                            const double MIN_RELATIVE_DISTANCE = 0.02; // 1/50 = 0.02

                            if (distToP1 / edgeLength < MIN_RELATIVE_DISTANCE) {
                                // 距离p1太近，在距离p1为MIN_RELATIVE_DISTANCE处插入点
                                // 计算从p1到p2的单位向量
                                Point3D direction = edge.p2 - edge.p1;
                                direction = direction.normalized();

                                // 在距离p1为MIN_RELATIVE_DISTANCE * edgeLength处插入点
                                double adjustedT = MIN_RELATIVE_DISTANCE * edgeLength /
                                    sqrt(direction.x * direction.x +
                                        direction.y * direction.y +
                                        direction.z * direction.z);

                                Point3D adjustedPoint = edge.p1 + direction * adjustedT;
                                adjustedPoint.value = isoVal;

                                // 插值法线
                                adjustedPoint.nx = edge.p1.nx + adjustedT * (edge.p2.nx - edge.p1.nx);
                                adjustedPoint.ny = edge.p1.ny + adjustedT * (edge.p2.ny - edge.p1.ny);
                                adjustedPoint.nz = edge.p1.nz + adjustedT * (edge.p2.nz - edge.p1.nz);

                                isoPoints[isoVal].push_back(adjustedPoint);
                            }
                            else if (distToP2 / edgeLength < MIN_RELATIVE_DISTANCE) {
                                // 距离p2太近，在距离p2为MIN_RELATIVE_DISTANCE处插入点
                                // 计算从p2到p1的单位向量
                                Point3D direction = edge.p1 - edge.p2;
                                direction = direction.normalized();

                                // 在距离p2为MIN_RELATIVE_DISTANCE * edgeLength处插入点
                                double adjustedT = MIN_RELATIVE_DISTANCE * edgeLength /
                                    sqrt(direction.x * direction.x +
                                        direction.y * direction.y +
                                        direction.z * direction.z);

                                Point3D adjustedPoint = edge.p2 + direction * adjustedT;
                                adjustedPoint.value = isoVal;

                                // 插值法线
                                adjustedPoint.nx = edge.p2.nx + adjustedT * (edge.p1.nx - edge.p2.nx);
                                adjustedPoint.ny = edge.p2.ny + adjustedT * (edge.p1.ny - edge.p2.ny);
                                adjustedPoint.nz = edge.p2.nz + adjustedT * (edge.p1.nz - edge.p2.nz);

                                isoPoints[isoVal].push_back(adjustedPoint);
                            }
                            else {
                                // 正常情况，直接使用插值点
                                isoPoints[isoVal].push_back(newPoint);
                            }
                        }
                    }
                }
            }

            for (auto& entry : isoPoints) {
                removeDuplicatePoints(entry.second);
            }
        }

        inline void removeDuplicatePoints(vector<Point3D>& points) {
            set<Point3D> uniqueSet(points.begin(), points.end());
            points.assign(uniqueSet.begin(), uniqueSet.end());
        }

        inline void connectIsoPoints() {
            for (const auto& entry : isoPoints) {
                const vector<Point3D>& points = entry.second;
                if (points.size() == 2) {
                    splitEdges.push_back(Edge(points[0], points[1]));
                }
            }

            sort(splitEdges.begin(), splitEdges.end(),
                [](const Edge& a, const Edge& b) {
                return a.p1.value < b.p1.value;
            });
        }

        inline void buildPolygons(const Point3D local_vertices[3]) {
            if (splitEdges.empty()) {
                resultPolygons.push_back(Polygon({ local_vertices[0], local_vertices[1], local_vertices[2] }));
                return;
            }

            vector<Polygon> currentPolygons = { Polygon({local_vertices[0], local_vertices[1], local_vertices[2]}) };

            for (const Edge& splitEdge : splitEdges) {
                vector<Polygon> newPolygons;

                for (const Polygon& poly : currentPolygons) {
                    if (isEdgeInsidePolygon(splitEdge, poly)) {
                        vector<Polygon> splitResult = splitPolygonWithEdge(poly, splitEdge);
                        newPolygons.insert(newPolygons.end(), splitResult.begin(), splitResult.end());
                    }
                    else {
                        newPolygons.push_back(poly);
                    }
                }

                currentPolygons = newPolygons;
            }

            resultPolygons = currentPolygons;
        }

        inline bool isEdgeInsidePolygon(const Edge& edge, const Polygon& poly) {
            return poly.containsEdge(edge);
        }

        inline vector<Polygon> splitPolygonWithEdge(const Polygon& poly, const Edge& edge) {
            vector<Polygon> result;

            vector<Point3D> vertices = poly.vertices;
            int n = static_cast<int>(vertices.size());

            insertPointOnPolygon(edge.p1, vertices);
            insertPointOnPolygon(edge.p2, vertices);

            int idx1 = -1, idx2 = -1;
            for (int i = 0; i < vertices.size(); i++) {
                if (vertices[i] == edge.p1) idx1 = i;
                if (vertices[i] == edge.p2) idx2 = i;
            }

            if (idx1 == -1 || idx2 == -1) {
                result.push_back(poly);
                return result;
            }

            if (idx1 > idx2) {
                swap(idx1, idx2);
            }

            vector<Point3D> poly1Verts;
            for (int i = idx1; i <= idx2; i++) {
                poly1Verts.push_back(vertices[i]);
            }

            vector<Point3D> poly2Verts;
            for (int i = idx2; i < vertices.size(); i++) {
                poly2Verts.push_back(vertices[i]);
            }
            for (int i = 0; i <= idx1; i++) {
                poly2Verts.push_back(vertices[i]);
            }

            if (poly1Verts.size() >= 3 && poly1Verts.front() == poly1Verts.back()) {
                poly1Verts.pop_back();
            }
            if (poly2Verts.size() >= 3 && poly2Verts.front() == poly2Verts.back()) {
                poly2Verts.pop_back();
            }

            if (poly1Verts.size() >= 3) {
                result.push_back(Polygon(poly1Verts));
            }
            if (poly2Verts.size() >= 3) {
                result.push_back(Polygon(poly2Verts));
            }

            return result;
        }

        inline void insertPointOnPolygon(const Point3D& point, vector<Point3D>& vertices) {
            for (const auto& v : vertices) {
                if (v == point) return;
            }

            for (size_t i = 0; i < vertices.size(); i++) {
                size_t j = (i + 1) % vertices.size();
                if (isPointOnEdge(point, vertices[i], vertices[j])) {
                    vertices.insert(vertices.begin() + j, point);
                    return;
                }
            }
        }

        inline bool isPointOnEdge(const Point3D& p, const Point3D& a, const Point3D& b) const {
            double cross = (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x);
            if (fabs(cross) > 1e-9) return false;

            double dot = (p.x - a.x) * (b.x - a.x) + (p.y - a.y) * (b.y - a.y);
            double len2 = (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);

            return dot >= -1e-9 && dot <= len2 + 1e-9;
        }
    };

    // ============================================================================
    // 新增：三角形网格类（完整版）
    // ============================================================================

    // 节点信息结构（用于节点合并）
    struct NodeKey {
        double x, y, z;

        NodeKey(double x, double y, double z) : x(x), y(y), z(z) {}

        inline bool operator<(const NodeKey& other) const {
            if (fabs(x - other.x) > 1e-9) return x < other.x;
            if (fabs(y - other.y) > 1e-9) return y < other.y;
            return z < other.z;
        }
    };

    // Nastran读取结果结构
    struct NastranReadResult {
        bool success;
        size_t nodes_read;
        size_t elements_read;
        size_t duplicate_nodes_merged;
        string error_message;

        inline NastranReadResult()
            : success(false), nodes_read(0), elements_read(0),
            duplicate_nodes_merged(0) {}
    };

    // Nastran写入结果结构
    struct NastranWriteResult {
        bool success;
        size_t nodes_written;
        size_t elements_written;
        string error_message;

        inline NastranWriteResult()
            : success(false), nodes_written(0), elements_written(0) {}
    };

    // Nastran写入选项
    struct NastranWriteOptions {
        bool include_temp_cards = true;      // 是否包含TEMP卡片
        bool include_comments = true;        // 是否包含注释
        bool sort_nodes_by_id = true;        // 是否按ID排序节点
        int precision = 10;                  // 输出精度
        string header_comment = "";          // 文件头部注释
    };

    // 三角形网格类
    class TriangleMesh {
    private:
        vector<double> node_coords;      // x,y,z交替存储 [x1,y1,z1, x2,y2,z2, ...]
        vector<double> node_values;      // 每个节点的物理量值
        vector<double> node_normals;     // 每个节点的法线 [nx1,ny1,nz1, nx2,ny2,nz2, ...]
        vector<int> elements;            // 三角形单元 [n1,n2,n3, n4,n5,n6, ...]

    public:
        inline TriangleMesh() {}

        // 获取节点数量
        inline size_t getNodeCount() const {
            return node_coords.size() / 3;
        }

        // 获取三角形数量
        inline size_t getTriangleCount() const {
            return elements.size() / 3;
        }

        // 获取节点坐标
        inline const vector<double>& getNodeCoords() const {
            return node_coords;
        }

        // 获取节点值
        inline const vector<double>& getNodeValues() const {
            return node_values;
        }

        // 获取节点法线
        inline const vector<double>& getNodeNormals() const {
            return node_normals;
        }

        // 获取三角形单元
        inline const vector<int>& getElements() const {
            return elements;
        }

        // 获取指定节点的坐标
        inline vector<double> getNodeCoords(int nodeIndex) const {
            vector<double> coords(3);
            if (nodeIndex >= 0 && nodeIndex * 3 + 2 < node_coords.size()) {
                coords[0] = node_coords[nodeIndex * 3];
                coords[1] = node_coords[nodeIndex * 3 + 1];
                coords[2] = node_coords[nodeIndex * 3 + 2];
            }
            return coords;
        }

        // 获取指定节点的值
        inline double getNodeValue(int nodeIndex) const {
            if (nodeIndex >= 0 && nodeIndex < node_values.size()) {
                return node_values[nodeIndex];
            }
            return 0.0;
        }

        // 获取指定节点的法线
        inline vector<double> getNodeNormal(int nodeIndex) const {
            vector<double> normal(3, 0.0);
            if (nodeIndex >= 0 && nodeIndex * 3 + 2 < node_normals.size()) {
                normal[0] = node_normals[nodeIndex * 3];
                normal[1] = node_normals[nodeIndex * 3 + 1];
                normal[2] = node_normals[nodeIndex * 3 + 2];
            }
            return normal;
        }

        // 设置指定节点的值
        inline bool setNodeValue(int nodeIndex, double value) {
            if (nodeIndex >= 0 && nodeIndex < node_values.size()) {
                node_values[nodeIndex] = value;
                return true;
            }
            return false;
        }

        // 设置指定节点的法线
        inline bool setNodeNormal(int nodeIndex, double nx, double ny, double nz) {
            if (nodeIndex >= 0 && nodeIndex * 3 + 2 < node_normals.size()) {
                node_normals[nodeIndex * 3] = nx;
                node_normals[nodeIndex * 3 + 1] = ny;
                node_normals[nodeIndex * 3 + 2] = nz;
                return true;
            }
            return false;
        }

        // 批量设置节点值
        inline bool setNodeValues(const vector<double>& values) {
            if (values.size() == node_values.size()) {
                node_values = values;
                return true;
            }
            return false;
        }

        // 批量设置节点法线
        inline bool setNodeNormals(const vector<double>& normals) {
            if (normals.size() == node_normals.size()) {
                node_normals = normals;
                return true;
            }
            return false;
        }

        // 添加节点（自动合并重复节点）
        inline int addNode(double x, double y, double z, double value, double nx = 0, double ny = 0, double nz = 1.0) {
            // 查找是否已存在相同坐标的节点（值可以不同）
            //for (size_t i = 0; i < node_values.size(); i++) {
            //    size_t idx = i * 3;
            //    if (fabs(node_coords[idx] - x) < 1e-9 &&
            //        fabs(node_coords[idx + 1] - y) < 1e-9 &&
            //        fabs(node_coords[idx + 2] - z) < 1e-9) {
            //        return static_cast<int>(i); // 返回已有节点索引
            //    }
            //}

            // 添加新节点
            node_coords.push_back(x);
            node_coords.push_back(y);
            node_coords.push_back(z);
            node_values.push_back(value);
            node_normals.push_back(nx);
            node_normals.push_back(ny);
            node_normals.push_back(nz);

            return static_cast<int>(node_values.size() - 1);
        }

        // 添加三角形
        inline void addTriangle(int n1, int n2, int n3) {
            elements.push_back(n1);
            elements.push_back(n2);
            elements.push_back(n3);
        }

        // 从点列表创建三角形（批量添加）
        inline void addTriangles(const vector<int>& triangle_indices) {
            if (triangle_indices.size() % 3 != 0) {
                cerr << "警告: 三角形索引数量必须是3的倍数" << endl;
                return;
            }
            elements.insert(elements.end(), triangle_indices.begin(), triangle_indices.end());
        }

        // 清空网格
        inline void clear() {
            node_coords.clear();
            node_values.clear();
            node_normals.clear();
            elements.clear();
        }

        // 验证网格有效性
        inline bool isValid() const {
            if (node_coords.size() != node_values.size() * 3) {
                cerr << "错误: 节点坐标与节点值数量不匹配" << endl;
                return false;
            }
            if (node_coords.size() != node_normals.size()) {
                cerr << "错误: 节点坐标与节点法线数量不匹配" << endl;
                return false;
            }
            if (elements.size() % 3 != 0) {
                cerr << "错误: 三角形单元索引数量必须是3的倍数" << endl;
                return false;
            }

            // 检查所有节点索引有效
            int maxNodeIndex = static_cast<int>(node_values.size() - 1);
            for (size_t i = 0; i < elements.size(); i++) {
                int nodeIdx = elements[i];
                if (nodeIdx < 0 || nodeIdx > maxNodeIndex) {
                    cerr << "错误: 节点索引 " << nodeIdx << " 超出范围 [0, " << maxNodeIndex << "]" << endl;
                    return false;
                }
            }

            return true;
        }

        // 获取网格边界
        inline void getBoundingBox(double& minX, double& minY, double& minZ,
            double& maxX, double& maxY, double& maxZ) const {
            if (node_coords.empty()) {
                minX = minY = minZ = maxX = maxY = maxZ = 0.0;
                return;
            }

            minX = maxX = node_coords[0];
            minY = maxY = node_coords[1];
            minZ = maxZ = node_coords[2];

            for (size_t i = 0; i < node_values.size(); i++) {
                double x = node_coords[i * 3];
                double y = node_coords[i * 3 + 1];
                double z = node_coords[i * 3 + 2];

                if (x < minX) minX = x;
                if (x > maxX) maxX = x;
                if (y < minY) minY = y;
                if (y > maxY) maxY = y;
                if (z < minZ) minZ = z;
                if (z > maxZ) maxZ = z;
            }
        }

        // 计算网格总面积
        inline double calculateTotalArea() const {
            double totalArea = 0.0;
            size_t triCount = getTriangleCount();

            for (size_t t = 0; t < triCount; t++) {
                int n1 = elements[t * 3];
                int n2 = elements[t * 3 + 1];
                int n3 = elements[t * 3 + 2];

                // 获取顶点坐标
                const double* p1 = &node_coords[n1 * 3];
                const double* p2 = &node_coords[n2 * 3];
                const double* p3 = &node_coords[n3 * 3];

                // 计算三角形面积（3D面积，使用叉积）
                double v1x = p2[0] - p1[0];
                double v1y = p2[1] - p1[1];
                double v1z = p2[2] - p1[2];

                double v2x = p3[0] - p1[0];
                double v2y = p3[1] - p1[1];
                double v2z = p3[2] - p1[2];

                // 计算叉积
                double cross_x = v1y * v2z - v1z * v2y;
                double cross_y = v1z * v2x - v1x * v2z;
                double cross_z = v1x * v2y - v1y * v2x;

                double area = 0.5 * sqrt(cross_x * cross_x + cross_y * cross_y + cross_z * cross_z);
                totalArea += area;
            }

            return totalArea;
        }

        // 计算节点值的范围
        inline void getValueRange(double& minValue, double& maxValue) const {
            if (node_values.empty()) {
                minValue = maxValue = 0.0;
                return;
            }

            minValue = maxValue = node_values[0];
            for (size_t i = 1; i < node_values.size(); i++) {
                if (node_values[i] < minValue) minValue = node_values[i];
                if (node_values[i] > maxValue) maxValue = node_values[i];
            }
        }

        // 计算三角形单元的法线
        inline vector<double> calculateTriangleNormals() const {
            vector<double> triNormals;
            size_t triCount = getTriangleCount();

            for (size_t t = 0; t < triCount; t++) {
                int n1 = elements[t * 3];
                int n2 = elements[t * 3 + 1];
                int n3 = elements[t * 3 + 2];

                // 获取顶点坐标
                const double* p1 = &node_coords[n1 * 3];
                const double* p2 = &node_coords[n2 * 3];
                const double* p3 = &node_coords[n3 * 3];

                // 计算三角形两条边
                double v1x = p2[0] - p1[0];
                double v1y = p2[1] - p1[1];
                double v1z = p2[2] - p1[2];

                double v2x = p3[0] - p1[0];
                double v2y = p3[1] - p1[1];
                double v2z = p3[2] - p1[2];

                // 计算叉积得到法线
                double nx = v1y * v2z - v1z * v2y;
                double ny = v1z * v2x - v1x * v2z;
                double nz = v1x * v2y - v1y * v2x;

                // 归一化
                double length = sqrt(nx * nx + ny * ny + nz * nz);
                if (length > 1e-12) {
                    nx /= length;
                    ny /= length;
                    nz /= length;
                }

                triNormals.push_back(nx);
                triNormals.push_back(ny);
                triNormals.push_back(nz);
            }

            return triNormals;
        }

        // 自动计算节点法线（通过相邻单元法线均值）
        inline bool calculateNodeNormals() {
            size_t nodeCount = getNodeCount();
            if (nodeCount == 0) return false;

            // 初始化节点法线存储
            node_normals.assign(nodeCount * 3, 0.0);
            vector<int> nodeAdjacentCount(nodeCount, 0);

            // 计算三角形单元法线
            vector<double> triNormals = calculateTriangleNormals();
            size_t triCount = getTriangleCount();

            // 遍历所有三角形，将法线累加到每个顶点
            for (size_t t = 0; t < triCount; t++) {
                int n1 = elements[t * 3];
                int n2 = elements[t * 3 + 1];
                int n3 = elements[t * 3 + 2];

                double nx = triNormals[t * 3];
                double ny = triNormals[t * 3 + 1];
                double nz = triNormals[t * 3 + 2];

                // 累加到每个顶点
                node_normals[n1 * 3] += nx;
                node_normals[n1 * 3 + 1] += ny;
                node_normals[n1 * 3 + 2] += nz;
                nodeAdjacentCount[n1]++;

                node_normals[n2 * 3] += nx;
                node_normals[n2 * 3 + 1] += ny;
                node_normals[n2 * 3 + 2] += nz;
                nodeAdjacentCount[n2]++;

                node_normals[n3 * 3] += nx;
                node_normals[n3 * 3 + 1] += ny;
                node_normals[n3 * 3 + 2] += nz;
                nodeAdjacentCount[n3]++;
            }

            // 归一化节点法线
            for (size_t i = 0; i < nodeCount; i++) {
                if (nodeAdjacentCount[i] > 0) {
                    double nx = node_normals[i * 3];
                    double ny = node_normals[i * 3 + 1];
                    double nz = node_normals[i * 3 + 2];

                    double length = sqrt(nx * nx + ny * ny + nz * nz);
                    if (length > 1e-12) {
                        node_normals[i * 3] = nx / length;
                        node_normals[i * 3 + 1] = ny / length;
                        node_normals[i * 3 + 2] = nz / length;
                    }
                    else {
                        // 如果法线长度为0，使用默认法线
                        node_normals[i * 3] = 0;
                        node_normals[i * 3 + 1] = 0;
                        node_normals[i * 3 + 2] = 1.0;
                    }
                }
                else {
                    // 没有相邻单元的节点，使用默认法线
                    node_normals[i * 3] = 0;
                    node_normals[i * 3 + 1] = 0;
                    node_normals[i * 3 + 2] = 1.0;
                }
            }

            return true;
        }

        // 从Nastran格式文件读取
        inline NastranReadResult readFromNastran(const string& filename, bool mergeDuplicateNodes = true) {
            NastranReadResult result;

            ifstream file(filename);
            if (!file.is_open()) {
                result.error_message = "无法打开文件: " + filename;
                return result;
            }

            // 临时存储：Nastran ID -> 内部索引
            map<int, int> nastranIdToIndex;
            // 坐标映射用于合并节点
            map<NodeKey, int> coordToIndex;
            // 临时存储节点值（Nastran ID -> 值）
            map<int, double> tempValues;

            string line;
            int lineNumber = 0;

            try {
                // 首先读取所有GRID卡片和TEMP卡片
                while (getline(file, line)) {
                    lineNumber++;

                    // 跳过空行和注释
                    if (line.empty() || line[0] == '$') {
                        continue;
                    }

                    // 转换为大写
                    string upperLine = line;
                    transform(upperLine.begin(), upperLine.end(), upperLine.begin(), ::toupper);

                    // 检查是否以GRID开头
                    if (upperLine.find("GRID") == 0) {
                        // 读取下一行（Z坐标行）
                        string nextLine;
                        if (!getline(file, nextLine)) {
                            result.error_message = "文件格式错误: 期望Z坐标行";
                            return result;
                        }
                        lineNumber++;

                        // 解析GRID行
                        int nodeId = 0;
                        double x = 0.0, y = 0.0, z = 0.0;

                        if (parseGridLine(line, nextLine, nodeId, x, y, z)) {
                            if (nodeId <= 0) {
                                continue;
                            }

                            NodeKey key(x, y, z);

                            if (mergeDuplicateNodes) {
                                auto it = coordToIndex.find(key);
                                if (it != coordToIndex.end()) {
                                    // 合并重复节点
                                    nastranIdToIndex[nodeId] = it->second;
                                    result.duplicate_nodes_merged++;
                                    continue;
                                }
                            }

                            // 添加新节点，初始值设为0，法线设为默认值
                            int internalIndex = addNode(x, y, z, 0.0, 0.0, 0.0, 1.0);
                            nastranIdToIndex[nodeId] = internalIndex;
                            coordToIndex[key] = internalIndex;
                            result.nodes_read++;
                        }
                    }
                    // 处理TEMP卡片（存储到临时映射中）
                    else if (upperLine.find("TEMP") == 0) {
                        parseTempLine(line, tempValues);
                    }
                }

                // 现在应用TEMP卡片中的值到节点
                for (const auto& kv : tempValues) {
                    int nastranId = kv.first;
                    double value = kv.second;

                    auto it = nastranIdToIndex.find(nastranId);
                    if (it != nastranIdToIndex.end()) {
                        int internalIndex = it->second;
                        node_values[internalIndex] = value;
                    }
                }

                // 重置文件流到开头，读取CTRIA3和CQUAD4卡片
                file.clear();
                file.seekg(0);

                while (getline(file, line)) {
                    string upperLine = line;
                    transform(upperLine.begin(), upperLine.end(), upperLine.begin(), ::toupper);

                    // 处理CTRIA3卡片
                    if (upperLine.find("CTRIA3") == 0) {
                        parseCTRIA3Line(line, nastranIdToIndex, result);
                    }
                    // 处理CQUAD4卡片（转换为两个三角形）
                    else if (upperLine.find("CQUAD4") == 0) {
                        parseCQUAD4Line(line, nastranIdToIndex, result);
                    }
                }

                file.close();

                if (result.elements_read == 0) {
                    result.error_message = "文件中未找到有效的单元数据";
                    return result;
                }

                // 自动计算节点法线
                calculateNodeNormals();

                result.success = true;
                return result;
            }
            catch (const exception& e) {
                result.error_message = string("解析错误 (第") +
                    to_string(lineNumber) + "行): " + e.what();
                if (file.is_open()) {
                    file.close();
                }
                return result;
            }
        }

        // 写入Nastran格式文件
        inline NastranWriteResult writeToNastran(const string& filename,
            const NastranWriteOptions& options = NastranWriteOptions()) const {
            NastranWriteResult result;

            if (!isValid()) {
                result.error_message = "网格无效，无法写入";
                return result;
            }

            ofstream file(filename);
            if (!file.is_open()) {
                result.error_message = "无法创建文件: " + filename;
                return result;
            }

            file << fixed << setprecision(options.precision);

            // 写入头部注释
            if (options.include_comments) {
                file << "$ Nastran format file generated by TriangleMesh class\n";
                if (!options.header_comment.empty()) {
                    file << "$ " << options.header_comment << "\n";
                }
                file << "$ Nodes: " << getNodeCount() << "\n";
                file << "$ Triangles: " << getTriangleCount() << "\n";
                file << "$ Total Area: " << calculateTotalArea() << "\n";

                // 计算值范围
                double minVal, maxVal;
                getValueRange(minVal, maxVal);
                file << "$ Value Range: [" << minVal << ", " << maxVal << "]\n";
                file << "$\n";
            }

            // 写入节点
            file << "$ Nodes\n";
            size_t nodeCount = getNodeCount();

            // 如果需要按ID排序，则创建一个索引列表
            vector<size_t> nodeIndices(nodeCount);
            for (size_t i = 0; i < nodeCount; i++) {
                nodeIndices[i] = i;
            }

            if (options.sort_nodes_by_id) {
                // 已经是按顺序的，不需要排序
            }

            for (size_t idx : nodeIndices) {
                int id = static_cast<int>(idx + 1);
                double x = node_coords[idx * 3];
                double y = node_coords[idx * 3 + 1];
                double z = node_coords[idx * 3 + 2];
                double value = node_values[idx];

                // GRID卡片
                file << "GRID   *"
                    << setw(16) << right << id
                    << setw(16) << right << 0
                    << setw(16) << right << x
                    << setw(16) << right << y
                    << " N" << setw(6) << left << id << "\n"
                    << "*N" << setw(6) << left << id
                    << setw(16) << right << z << "\n";

                // 节点值注释
                if (options.include_comments) {
                    file << "$ Value: " << setprecision(12) << value
                        << setprecision(options.precision) << "\n";
                    // 添加法线信息注释
                    file << "$ Normal: (" << node_normals[idx * 3] << ", "
                        << node_normals[idx * 3 + 1] << ", "
                        << node_normals[idx * 3 + 2] << ")\n";
                }
            }

            // 写入单元
            file << "\n$ Elements\n";
            size_t triCount = getTriangleCount();

            for (size_t i = 0; i < triCount; i++) {
                int elemId = static_cast<int>(i + 1);
                int n1 = elements[i * 3] + 1; // 转换为1-based索引
                int n2 = elements[i * 3 + 1] + 1;
                int n3 = elements[i * 3 + 2] + 1;

                file << "CTRIA3  "
                    << setw(8) << right << elemId
                    << setw(8) << right << 0
                    << setw(8) << right << n1
                    << setw(8) << right << n2
                    << setw(8) << right << n3 << "\n";
            }

            // 写入结果部分（TEMP卡片）
            if (options.include_temp_cards) {
                file << "\n$ Results - Node Values\n";
                file << "BEGIN BULK\n";
                for (size_t i = 0; i < nodeCount; i++) {
                    int id = static_cast<int>(i + 1);
                    double value = node_values[i];
                    file << "TEMP    "
                        << setw(8) << right << 1
                        << setw(8) << right << id
                        << setw(16) << right << value << "\n";
                }
                file << "ENDDATA\n";
            }
            else {
                file << "ENDDATA\n";
            }

            file.close();

            result.success = true;
            result.nodes_written = nodeCount;
            result.elements_written = triCount;
            return result;
        }

        // 从CSV文件读取节点值
        inline bool readNodeValuesFromCSV(const string& filename) {
            ifstream file(filename);
            if (!file.is_open()) {
                cerr << "无法打开CSV文件: " << filename << endl;
                return false;
            }

            vector<double> new_values;
            string line;

            // 跳过可能的标题行
            while (getline(file, line)) {
                if (!line.empty() && line[0] != '#') {
                    // 检查是否包含非数字字符来判断是否为标题
                    bool is_header = false;
                    for (char c : line) {
                        if (isalpha(c)) {
                            is_header = true;
                            break;
                        }
                    }
                    if (!is_header) {
                        // 回退一行
                        file.seekg(-static_cast<int>(line.length()) - 1, ios::cur);
                        break;
                    }
                }
            }

            // 读取数据
            while (getline(file, line)) {
                if (line.empty() || line[0] == '#') continue;

                stringstream ss(line);
                string token;
                vector<string> tokens;

                while (getline(ss, token, ',')) {
                    tokens.push_back(token);
                }

                if (tokens.size() >= 1) {
                    try {
                        double value = stod(tokens[0]);
                        new_values.push_back(value);
                    }
                    catch (...) {
                        cerr << "警告: 无法解析值: " << tokens[0] << endl;
                    }
                }
            }

            file.close();

            if (new_values.size() != node_values.size()) {
                cerr << "错误: CSV文件中的值数量(" << new_values.size()
                    << ")与节点数量(" << node_values.size() << ")不匹配" << endl;
                return false;
            }

            node_values = new_values;
            return true;
        }

        // 根据等值线拆分网格
        inline TriangleMesh splitByIsolines(const vector<double>& isoValues) const {
            TriangleMesh resultMesh;

            if (isoValues.empty()) {
                // 如果没有等值线，直接复制原网格
                resultMesh.node_coords = node_coords;
                resultMesh.node_values = node_values;
                resultMesh.node_normals = node_normals;
                resultMesh.elements = elements;
                return resultMesh;
            }

            // 遍历所有三角形
            size_t triCount = getTriangleCount();
            for (size_t t = 0; t < triCount; t++) {
                int n1_idx = elements[t * 3];
                int n2_idx = elements[t * 3 + 1];
                int n3_idx = elements[t * 3 + 2];

                // 获取三角形顶点
                Point3D p1(
                    node_coords[n1_idx * 3],
                    node_coords[n1_idx * 3 + 1],
                    node_coords[n1_idx * 3 + 2],
                    node_values[n1_idx],
                    node_normals[n1_idx * 3],
                    node_normals[n1_idx * 3 + 1],
                    node_normals[n1_idx * 3 + 2]
                );

                Point3D p2(
                    node_coords[n2_idx * 3],
                    node_coords[n2_idx * 3 + 1],
                    node_coords[n2_idx * 3 + 2],
                    node_values[n2_idx],
                    node_normals[n2_idx * 3],
                    node_normals[n2_idx * 3 + 1],
                    node_normals[n2_idx * 3 + 2]
                );

                Point3D p3(
                    node_coords[n3_idx * 3],
                    node_coords[n3_idx * 3 + 1],
                    node_coords[n3_idx * 3 + 2],
                    node_values[n3_idx],
                    node_normals[n3_idx * 3],
                    node_normals[n3_idx * 3 + 1],
                    node_normals[n3_idx * 3 + 2]
                );

                // 拆分三角形为多边形
                TriangleSplitter splitter(p1, p2, p3, isoValues);
                vector<Polygon> polygons = splitter.split();

                // 将每个多边形拆分为三角形并添加到结果网格
                for (const Polygon& poly : polygons) {
                    triangulatePolygon(poly, resultMesh);
                }
            }

            return resultMesh;
        }

        // 生成等值线处的边（用于可视化）
        inline vector<Edge> extractIsolineEdges(double isoValue) const {
            vector<Edge> isoEdges;

            size_t triCount = getTriangleCount();
            for (size_t t = 0; t < triCount; t++) {
                int n1_idx = elements[t * 3];
                int n2_idx = elements[t * 3 + 1];
                int n3_idx = elements[t * 3 + 2];

                // 获取三角形顶点和值
                Point3D p1(
                    node_coords[n1_idx * 3],
                    node_coords[n1_idx * 3 + 1],
                    node_coords[n1_idx * 3 + 2],
                    node_values[n1_idx],
                    node_normals[n1_idx * 3],
                    node_normals[n1_idx * 3 + 1],
                    node_normals[n1_idx * 3 + 2]
                );

                Point3D p2(
                    node_coords[n2_idx * 3],
                    node_coords[n2_idx * 3 + 1],
                    node_coords[n2_idx * 3 + 2],
                    node_values[n2_idx],
                    node_normals[n2_idx * 3],
                    node_normals[n2_idx * 3 + 1],
                    node_normals[n2_idx * 3 + 2]
                );

                Point3D p3(
                    node_coords[n3_idx * 3],
                    node_coords[n3_idx * 3 + 1],
                    node_coords[n3_idx * 3 + 2],
                    node_values[n3_idx],
                    node_normals[n3_idx * 3],
                    node_normals[n3_idx * 3 + 1],
                    node_normals[n3_idx * 3 + 2]
                );

                // 检查每条边是否有交点
                vector<Point3D> intersections;

                // 边1: p1-p2
                if ((p1.value - isoValue) * (p2.value - isoValue) < 0) {
                    double t_val = (isoValue - p1.value) / (p2.value - p1.value);
                    Point3D inter(
                        p1.x + t_val * (p2.x - p1.x),
                        p1.y + t_val * (p2.y - p1.y),
                        p1.z + t_val * (p2.z - p1.z),
                        isoValue,
                        p1.nx + t_val * (p2.nx - p1.nx),
                        p1.ny + t_val * (p2.ny - p1.ny),
                        p1.nz + t_val * (p2.nz - p1.nz)
                    );
                    intersections.push_back(inter);
                }

                // 边2: p2-p3
                if ((p2.value - isoValue) * (p3.value - isoValue) < 0) {
                    double t_val = (isoValue - p2.value) / (p3.value - p2.value);
                    Point3D inter(
                        p2.x + t_val * (p3.x - p2.x),
                        p2.y + t_val * (p3.y - p2.y),
                        p2.z + t_val * (p3.z - p2.z),
                        isoValue,
                        p2.nx + t_val * (p3.nx - p2.nx),
                        p2.ny + t_val * (p3.ny - p2.ny),
                        p2.nz + t_val * (p3.nz - p2.nz)
                    );
                    intersections.push_back(inter);
                }

                // 边3: p3-p1
                if ((p3.value - isoValue) * (p1.value - isoValue) < 0) {
                    double t_val = (isoValue - p3.value) / (p1.value - p3.value);
                    Point3D inter(
                        p3.x + t_val * (p1.x - p3.x),
                        p3.y + t_val * (p1.y - p3.y),
                        p3.z + t_val * (p1.z - p3.z),
                        isoValue,
                        p3.nx + t_val * (p1.nx - p3.nx),
                        p3.ny + t_val * (p1.ny - p3.ny),
                        p3.nz + t_val * (p1.nz - p3.nz)
                    );
                    intersections.push_back(inter);
                }

                // 如果有两个交点，则添加一条边
                if (intersections.size() == 2) {
                    isoEdges.push_back(Edge(intersections[0], intersections[1]));
                }
            }

            return isoEdges;
        }

        // 打印网格信息
        inline void printInfo() const {
            cout << "三角形网格信息:" << endl;
            cout << "  节点数量: " << getNodeCount() << endl;
            cout << "  三角形数量: " << getTriangleCount() << endl;
            cout << "  总面积: " << calculateTotalArea() << endl;

            double minX, minY, minZ, maxX, maxY, maxZ;
            getBoundingBox(minX, minY, minZ, maxX, maxY, maxZ);
            cout << "  包围盒: [" << minX << ", " << maxX << "] x ["
                << minY << ", " << maxY << "] x [" << minZ << ", " << maxZ << "]" << endl;

            double minVal, maxVal;
            getValueRange(minVal, maxVal);
            cout << "  值范围: [" << minVal << ", " << maxVal << "]" << endl;

            if (!isValid()) {
                cout << "  警告: 网格无效!" << endl;
            }
        }

        // 打印详细网格数据
        inline void printDetails() const {
            printInfo();

            cout << "\n节点列表:" << endl;
            for (size_t i = 0; i < getNodeCount(); i++) {
                cout << "  节点 " << i << ": ("
                    << node_coords[i * 3] << ", "
                    << node_coords[i * 3 + 1] << ", "
                    << node_coords[i * 3 + 2] << ") 值="
                    << node_values[i] << " 法线=("
                    << node_normals[i * 3] << ", "
                    << node_normals[i * 3 + 1] << ", "
                    << node_normals[i * 3 + 2] << ")" << endl;
            }

            cout << "\n三角形列表:" << endl;
            for (size_t i = 0; i < getTriangleCount(); i++) {
                cout << "  三角形 " << i << ": ["
                    << elements[i * 3] << ", "
                    << elements[i * 3 + 1] << ", "
                    << elements[i * 3 + 2] << "]" << endl;
            }
        }

    private:
        // 解析GRID行
        inline bool parseGridLine(const string& line1, const string& line2,
            int& node_id, double& x, double& y, double& z) {
            string l1 = line1;
            l1.erase(0, l1.find_first_not_of(" \t"));

            if (l1.size() < 4 || l1.substr(0, 4) != "GRID") {
                return false;
            }

            stringstream ss;
            string data_part1;

            // 找到*的位置
            size_t star_pos = l1.find('*');
            if (star_pos != string::npos) {
                data_part1 = l1.substr(star_pos + 1);
            }
            else {
                data_part1 = l1.substr(4);
            }

            // 移除开头的空格
            data_part1.erase(0, data_part1.find_first_not_of(" \t"));
            ss.str(data_part1);

            int cp_id;
            if (!(ss >> node_id >> cp_id >> x >> y)) {
                return false;
            }

            // 解析第二行获取Z坐标
            string l2 = line2;
            l2.erase(0, l2.find_first_not_of(" \t"));

            if (l2.size() >= 2 && l2[0] == '*' && l2[1] == 'N') {
                const size_t z_start = 8;
                if (z_start < l2.size()) {
                    string z_str = l2.substr(z_start - 1);
                    z_str.erase(0, z_str.find_first_not_of(" \t"));
                    stringstream ss_z(z_str);
                    if (!(ss_z >> z)) {
                        z = 0.0;
                    }
                }
                else {
                    z = 0.0;
                }
            }
            else {
                stringstream ss_z(l2);
                if (!(ss_z >> z)) {
                    z = 0.0;
                }
            }

            return true;
        }

        // 解析TEMP行
        inline void parseTempLine(const string& line, map<int, double>& tempValues) {
            string trimmedLine = line;
            trimmedLine.erase(0, trimmedLine.find_first_not_of(" \t"));

            stringstream ss(trimmedLine.substr(4)); // 跳过"TEMP"
            int sid, gid;
            double value;

            // 尝试读取第一组数据
            if (ss >> sid >> gid >> value) {
                tempValues[gid] = value;

                // 尝试读取更多组数据（有些Nastran文件一行可能有多个数据）
                int gid2;
                double value2;
                while (ss >> gid2 >> value2) {
                    tempValues[gid2] = value2;
                }
            }
        }

        // 解析CTRIA3行
        inline void parseCTRIA3Line(const string& line, const map<int, int>& nastranIdToIndex,
            NastranReadResult& result) {
            string trimmedLine = line;
            trimmedLine.erase(0, trimmedLine.find_first_not_of(" \t"));

            stringstream ss(trimmedLine.substr(6)); // 跳过"CTRIA3"
            int elemId, pid, n1, n2, n3;

            if (ss >> elemId >> pid >> n1 >> n2 >> n3) {
                auto it1 = nastranIdToIndex.find(n1);
                auto it2 = nastranIdToIndex.find(n2);
                auto it3 = nastranIdToIndex.find(n3);

                if (it1 != nastranIdToIndex.end() &&
                    it2 != nastranIdToIndex.end() &&
                    it3 != nastranIdToIndex.end()) {

                    addTriangle(it1->second, it2->second, it3->second);
                    result.elements_read++;
                }
            }
        }

        // 解析CQUAD4行
        inline void parseCQUAD4Line(const string& line, const map<int, int>& nastranIdToIndex,
            NastranReadResult& result) {
            string trimmedLine = line;
            trimmedLine.erase(0, trimmedLine.find_first_not_of(" \t"));

            stringstream ss(trimmedLine.substr(6)); // 跳过"CQUAD4"
            int elemId, pid, n1, n2, n3, n4;

            if (ss >> elemId >> pid >> n1 >> n2 >> n3 >> n4) {
                auto it1 = nastranIdToIndex.find(n1);
                auto it2 = nastranIdToIndex.find(n2);
                auto it3 = nastranIdToIndex.find(n3);
                auto it4 = nastranIdToIndex.find(n4);

                if (it1 != nastranIdToIndex.end() &&
                    it2 != nastranIdToIndex.end() &&
                    it3 != nastranIdToIndex.end() &&
                    it4 != nastranIdToIndex.end()) {

                    // 将四边形分割为两个三角形（对角线n1-n3）
                    addTriangle(it1->second, it2->second, it3->second);
                    addTriangle(it1->second, it3->second, it4->second);
                    result.elements_read += 2;
                }
            }
        }

        // 将多边形三角化为三角形
        inline void triangulatePolygon(const Polygon& poly, TriangleMesh& mesh) const {
            const vector<Point3D>& vertices = poly.vertices;
            size_t n = vertices.size();

            if (n < 3) return;

            if (n == 3) {
                // 本身就是三角形
                int idx1 = mesh.addNode(vertices[0].x, vertices[0].y, vertices[0].z,
                    vertices[0].value, vertices[0].nx, vertices[0].ny, vertices[0].nz);
                int idx2 = mesh.addNode(vertices[1].x, vertices[1].y, vertices[1].z,
                    vertices[1].value, vertices[1].nx, vertices[1].ny, vertices[1].nz);
                int idx3 = mesh.addNode(vertices[2].x, vertices[2].y, vertices[2].z,
                    vertices[2].value, vertices[2].nx, vertices[2].ny, vertices[2].nz);
                mesh.addTriangle(idx1, idx2, idx3);
            }
            else {
                // 对于凸多边形，使用三角形扇
                // 这里假设多边形是凸的（由等值线分割产生）
                int firstIdx = mesh.addNode(vertices[0].x, vertices[0].y, vertices[0].z,
                    vertices[0].value, vertices[0].nx, vertices[0].ny, vertices[0].nz);

                for (size_t i = 1; i < n - 1; i++) {
                    int idx2 = mesh.addNode(vertices[i].x, vertices[i].y, vertices[i].z,
                        vertices[i].value, vertices[i].nx, vertices[i].ny, vertices[i].nz);
                    int idx3 = mesh.addNode(vertices[i + 1].x, vertices[i + 1].y, vertices[i + 1].z,
                        vertices[i + 1].value, vertices[i + 1].nx, vertices[i + 1].ny, vertices[i + 1].nz);
                    mesh.addTriangle(firstIdx, idx2, idx3);
                }
            }
        }
    };

    // 辅助函数：计算多边形面积总和
    inline double calculateTotalArea(const vector<Polygon>& polygons) {
        double total = 0.0;
        for (const auto& poly : polygons) {
            total += poly.area();
        }
        return total;
    }

    // 辅助函数：验证面积相等
    inline bool verifyAreaConservation(const Polygon& original, const vector<Polygon>& result) {
        double originalArea = original.area();
        double resultArea = calculateTotalArea(result);
        return fabs(originalArea - resultArea) < 1e-9;
    }

    // 测试函数 - 验证3D三角形分割
    inline int test3DTriangle() {
        cout << "=== 测试3D三角形拆分算法 ===" << endl;

        // 测试1：平面三角形（在xy平面上）
        cout << "\n--- 测试1：平面三角形（在xy平面上）---" << endl;
        {
            Point3D p1(0, 0, 1, 0, 0, 0, 1);
            Point3D p2(1, 0, 2, 10, 0, 0, 1);
            Point3D p3(0.5, 1, 3, 5, 0, 0, 1);

            vector<double> isoValues = { 2.5, 5, 7.5 };

            TriangleSplitter splitter(p1, p2, p3, isoValues);
            vector<Polygon> result = splitter.split();

            cout << "原始三角形（平面）:" << endl;
            cout << "  P1: (" << p1.x << "," << p1.y << "," << p1.z << ") 值=" << p1.value << " 法线=(" << p1.nx << "," << p1.ny << "," << p1.nz << ")" << endl;
            cout << "  P2: (" << p2.x << "," << p2.y << "," << p2.z << ") 值=" << p2.value << " 法线=(" << p2.nx << "," << p2.ny << "," << p2.nz << ")" << endl;
            cout << "  P3: (" << p3.x << "," << p3.y << "," << p3.z << ") 值=" << p3.value << " 法线=(" << p3.nx << "," << p3.ny << "," << p3.nz << ")" << endl;

            cout << "\n等值线物理量: ";
            for (double val : isoValues) cout << val << " ";
            cout << endl;

            cout << "\n拆分后的多边形数量: " << result.size() << endl;

            Polygon original({ p1, p2, p3 });
            double originalArea = original.area();
            double resultArea = calculateTotalArea(result);

            cout << "原始三角形面积: " << originalArea << endl;
            cout << "拆分后总面积: " << resultArea << endl;
            cout << "面积是否相等: " << (fabs(originalArea - resultArea) < 1e-9 ? "是" : "否") << endl;

            // 检查法线信息
            if (!result.empty()) {
                cout << "\n结果多边形法线信息:" << endl;
                for (size_t i = 0; i < result.size(); i++) {
                    cout << "多边形 " << i + 1 << " (顶点数: " << result[i].vertices.size() << "):" << endl;
                    for (const Point3D& v : result[i].vertices) {
                        cout << "   顶点 (" << v.x << ", " << v.y << ", " << v.z << ") 法线=("
                            << v.nx << ", " << v.ny << ", " << v.nz << ")" << endl;
                    }
                }
            }
        }

        // 测试2：倾斜的3D三角形
        cout << "\n--- 测试2：倾斜的3D三角形 ---" << endl;
        {
            Point3D p1(0, 0, 0, 0, 0, 0, 1);
            Point3D p2(1, 0, 1, 10, 0, 0, 1);  // z坐标不为0
            Point3D p3(0, 1, 0.5, 5, 0, 0, 1); // z坐标不为0

            vector<double> isoValues = { 2.5, 5, 7.5 };

            TriangleSplitter splitter(p1, p2, p3, isoValues);

            cout << "变换信息:" << endl;
            splitter.printTransformInfo();

            cout << "\n原始三角形（3D）:" << endl;
            cout << "  P1: (" << p1.x << "," << p1.y << "," << p1.z << ") 值=" << p1.value << " 法线=(" << p1.nx << "," << p1.ny << "," << p1.nz << ")" << endl;
            cout << "  P2: (" << p2.x << "," << p2.y << "," << p2.z << ") 值=" << p2.value << " 法线=(" << p2.nx << "," << p2.ny << "," << p2.nz << ")" << endl;
            cout << "  P3: (" << p3.x << "," << p3.y << "," << p3.z << ") 值=" << p3.value << " 法线=(" << p3.nx << "," << p3.ny << "," << p3.nz << ")" << endl;

            vector<Polygon> result = splitter.split();

            cout << "\n拆分后的多边形数量: " << result.size() << endl;

            Polygon original({ p1, p2, p3 });
            double originalArea = original.area();
            double resultArea = calculateTotalArea(result);

            cout << "原始三角形面积（3D）: " << originalArea << endl;
            cout << "拆分后总面积（3D）: " << resultArea << endl;
            cout << "面积是否相等: " << (fabs(originalArea - resultArea) < 1e-9 ? "是" : "否") << endl;

            // 打印结果多边形
            for (size_t i = 0; i < result.size(); i++) {
                cout << "\n多边形 " << i + 1 << " (顶点数: " << result[i].vertices.size() << "):" << endl;
                for (const Point3D& v : result[i].vertices) {
                    cout << "    (" << v.x << ", " << v.y << ", " << v.z << ") 值=" << v.value
                        << " 法线=(" << v.nx << ", " << v.ny << ", " << v.nz << ")" << endl;
                }
                cout << "  面积: " << result[i].area() << endl;
            }
        }

        // 测试3：更复杂的3D三角形
        cout << "\n--- 测试3：更复杂的3D三角形 ---" << endl;
        {
            Point3D p1(1, 2, 3, 0, 0.577, 0.577, 0.577);
            Point3D p2(4, 6, 5, 8, 0.577, 0.577, 0.577);
            Point3D p3(2, 7, 4, 4, 0.577, 0.577, 0.577);

            vector<double> isoValues = { 1, 2, 3, 4, 5, 6, 7 };

            TriangleSplitter splitter(p1, p2, p3, isoValues);

            cout << "原始三角形（3D）:" << endl;
            cout << "  P1: (" << p1.x << "," << p1.y << "," << p1.z << ") 值=" << p1.value << " 法线=(" << p1.nx << "," << p1.ny << "," << p1.nz << ")" << endl;
            cout << "  P2: (" << p2.x << "," << p2.y << "," << p2.z << ") 值=" << p2.value << " 法线=(" << p2.nx << "," << p2.ny << "," << p2.nz << ")" << endl;
            cout << "  P3: (" << p3.x << "," << p3.y << "," << p3.z << ") 值=" << p3.value << " 法线=(" << p3.nx << "," << p3.ny << "," << p3.nz << ")" << endl;

            vector<Polygon> result = splitter.split();

            cout << "\n拆分后的多边形数量: " << result.size() << endl;

            Polygon original({ p1, p2, p3 });
            double originalArea = original.area();
            double resultArea = calculateTotalArea(result);

            cout << "原始三角形面积（3D）: " << originalArea << endl;
            cout << "拆分后总面积（3D）: " << resultArea << endl;
            cout << "面积是否相等: " << (fabs(originalArea - resultArea) < 1e-9 ? "是" : "否") << endl;

            // 验证所有多边形都在同一平面上
            if (!result.empty()) {
                cout << "\n验证所有结果多边形都在同一平面上:" << endl;
                for (size_t i = 0; i < result.size(); i++) {
                    if (result[i].vertices.size() >= 3) {
                        // 计算多边形的法向量
                        Point3D v1 = result[i].vertices[1] - result[i].vertices[0];
                        Point3D v2 = result[i].vertices[2] - result[i].vertices[0];
                        Point3D normal = v1.cross(v2);
                        cout << "多边形 " << i + 1 << " 几何法向量: ("
                            << normal.x << ", " << normal.y << ", " << normal.z << ")" << endl;
                        // 显示顶点法线
                        cout << "多边形 " << i + 1 << " 顶点法线:" << endl;
                        for (const Point3D& v : result[i].vertices) {
                            cout << "   顶点法线: (" << v.nx << ", " << v.ny << ", " << v.nz << ")" << endl;
                        }
                    }
                }
            }
        }

        return 0;
    }

    // ============================================================================
    // 测试函数
    // ============================================================================

    inline void testTriangleMeshAdvanced() {
        cout << "\n=== 测试改进的三角形网格类 ===" << endl;

        // 测试1：创建网格并设置节点值和法线
        cout << "\n--- 测试1：创建网格并设置节点值和法线 ---" << endl;
        {
            TriangleMesh mesh;

            // 添加节点（带法线）
            mesh.addNode(0, 0, 0, 10, 0, 0, 1);
            mesh.addNode(1, 0, 0, 20, 0, 0, 1);
            mesh.addNode(0.5, 1, 0, 15, 0, 0, 1);

            // 添加三角形
            mesh.addTriangle(0, 1, 2);

            mesh.printInfo();

            // 修改节点值和法线
            cout << "\n修改节点值和法线:" << endl;
            mesh.setNodeValue(0, 5);
            mesh.setNodeNormal(0, 0.707, 0, 0.707);
            mesh.setNodeValue(1, 25);
            mesh.setNodeNormal(1, 0, 0.707, 0.707);
            mesh.setNodeValue(2, 10);
            mesh.setNodeNormal(2, 0.577, 0.577, 0.577);

            cout << "节点0的值: " << mesh.getNodeValue(0) << " (期望: 5)" << endl;
            cout << "节点0的法线: (" << mesh.getNodeNormal(0)[0] << ", "
                << mesh.getNodeNormal(0)[1] << ", " << mesh.getNodeNormal(0)[2] << ")" << endl;

            // 测试法线自动计算
            cout << "\n测试法线自动计算:" << endl;
            TriangleMesh mesh2;
            mesh2.addNode(0, 0, 0, 10);
            mesh2.addNode(1, 0, 0, 20);
            mesh2.addNode(0.5, 1, 0, 15);
            mesh2.addTriangle(0, 1, 2);

            cout << "计算前法线: (" << mesh2.getNodeNormal(0)[0] << ", "
                << mesh2.getNodeNormal(0)[1] << ", " << mesh2.getNodeNormal(0)[2] << ")" << endl;

            mesh2.calculateNodeNormals();

            cout << "计算后法线: (" << mesh2.getNodeNormal(0)[0] << ", "
                << mesh2.getNodeNormal(0)[1] << ", " << mesh2.getNodeNormal(0)[2] << ")" << endl;
        }

        // 测试2：网格拆分和面积守恒
        cout << "\n--- 测试2：网格拆分和面积守恒 ---" << endl;
        {
            TriangleMesh mesh;

            // 创建3D三角形（带法线）
            mesh.addNode(0, 0, 0, 0, 0, 0, 1);
            mesh.addNode(1, 0, 1, 10, 0, 0, 1);
            mesh.addNode(0.5, 1, 0.5, 5, 0, 0, 1);
            mesh.addTriangle(0, 1, 2);

            double originalArea = mesh.calculateTotalArea();
            cout << "原始网格:" << endl;
            cout << "  面积: " << originalArea << endl;

            // 拆分网格
            vector<double> isoValues = { 2, 4, 6, 8 };
            TriangleMesh splitMesh = mesh.splitByIsolines(isoValues);

            double splitArea = splitMesh.calculateTotalArea();
            cout << "\n拆分后的网格:" << endl;
            cout << "  节点数: " << splitMesh.getNodeCount() << endl;
            cout << "  三角形数: " << splitMesh.getTriangleCount() << endl;
            cout << "  面积: " << splitArea << endl;

            double areaDiff = fabs(originalArea - splitArea);
            cout << "  面积差: " << areaDiff << endl;
            cout << "  面积守恒: " << (areaDiff < 1e-9 ? "是" : "否") << endl;

            // 检查法线是否传递
            if (splitMesh.getNodeCount() > 0) {
                cout << "\n拆分后第一个节点的法线: (" << splitMesh.getNodeNormal(0)[0] << ", "
                    << splitMesh.getNodeNormal(0)[1] << ", " << splitMesh.getNodeNormal(0)[2] << ")" << endl;
            }

            // 写入Nastran文件
            NastranWriteOptions options;
            options.header_comment = "拆分后的三角形网格（带法线）";
            options.include_comments = true;
            options.include_temp_cards = true;

            NastranWriteResult writeResult = splitMesh.writeToNastran("split_mesh_with_normals.nas", options);
            if (writeResult.success) {
                cout << "\n拆分结果已写入文件: split_mesh_with_normals.nas" << endl;
                cout << "  写入节点数: " << writeResult.nodes_written << endl;
                cout << "  写入三角形数: " << writeResult.elements_written << endl;
            }
        }

        // 测试3：等值线提取
        cout << "\n--- 测试3：等值线提取 ---" << endl;
        {
            TriangleMesh mesh;

            // 创建包含多个三角形的网格
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    double x = i * 0.5;
                    double y = j * 0.5;

                    int n1 = mesh.addNode(x, y, 0, x + y, 0, 0, 1);
                    int n2 = mesh.addNode(x + 0.5, y, 0, x + 0.5 + y, 0, 0, 1);
                    int n3 = mesh.addNode(x + 0.25, y + 0.5, 0, x + 0.25 + y + 0.5, 0, 0, 1);

                    mesh.addTriangle(n1, n2, n3);
                }
            }

            cout << "网格信息:" << endl;
            mesh.printInfo();

            // 提取等值线
            double isoValue = 1.0;
            vector<Edge> isoEdges = mesh.extractIsolineEdges(isoValue);

            cout << "\n等值线 " << isoValue << " 的边数量: " << isoEdges.size() << endl;

            // 显示前几条边
            int max_edges_to_show = min(5, (int)isoEdges.size());
            for (int i = 0; i < max_edges_to_show; i++) {
                const Edge& edge = isoEdges[i];
                cout << "  边 " << i << ": ("
                    << edge.p1.x << ", " << edge.p1.y << ") -> ("
                    << edge.p2.x << ", " << edge.p2.y << ")" << endl;
                cout << "     起点法线: (" << edge.p1.nx << ", " << edge.p1.ny << ", " << edge.p1.nz << ")" << endl;
                cout << "     终点法线: (" << edge.p2.nx << ", " << edge.p2.ny << ", " << edge.p2.nz << ")" << endl;
            }
        }

        cout << "\n=== 所有测试完成 ===" << endl;
    }

    // 函数2: 使用二分查找优化（利用set有序性）
    inline bool insertWithToleranceOptimized(std::set<double>& data, double value, double TOL) {
        auto lower = data.lower_bound(value - TOL);
        auto upper = data.upper_bound(value + TOL);

        // 检查[lower, upper)范围内是否有元素
        for (auto it = lower; it != upper; ++it) {
            if (std::fabs(*it - value) < TOL) {
                return false;  // 存在接近的值，不插入
            }
        }

        data.insert(value);
        return true;  // 插入成功
    }

    // ============================================================================
    // 实测用例函数 Nastran文件以x值为物理量10份等值线拆分
    // ============================================================================

    inline void testIsolineSplit() {
        cout << "=== 开始实测用例：三角形网格等值线拆分 ===" << endl;

        // 1. 创建三角形网格对象
        TriangleMesh input_tri_mesh;

        // 2. 从Nastran文件读取网格
        cout << "\n步骤1: 从文件 d:/fh.nas 读取网格..." << endl;
        NastranReadResult readResult = input_tri_mesh.readFromNastran("d:/fh.nas", true);

        if (!readResult.success) {
            cout << "错误: " << readResult.error_message << endl;
            cout << "请检查文件 d:/fh.nas 是否存在且格式正确" << endl;
            return;
        }

        cout << "读取成功!" << endl;
        cout << "  读取节点数: " << readResult.nodes_read << endl;
        cout << "  读取三角形数: " << readResult.elements_read << endl;
        cout << "  合并重复节点数: " << readResult.duplicate_nodes_merged << endl;

        input_tri_mesh.printInfo();

        // 3. 将每个节点的物理量设置为该节点的x坐标值
        cout << "\n步骤2: 设置节点物理量为x坐标值..." << endl;
        size_t nodeCount = input_tri_mesh.getNodeCount();

        for (size_t i = 0; i < nodeCount; i++) {
            // 获取节点坐标
            vector<double> coords = input_tri_mesh.getNodeCoords(static_cast<int>(i));
            if (coords.size() >= 1) {
                double x_value = coords[0];  // x坐标
                input_tri_mesh.setNodeValue(static_cast<int>(i), x_value);
            }
        }

        // 验证设置结果
        cout << "已设置 " << nodeCount << " 个节点的物理量" << endl;

        // 4. 获取x坐标的最大值和最小值
        cout << "\n步骤3: 获取x坐标范围..." << endl;
        double minX, minY, minZ, maxX, maxY, maxZ;
        input_tri_mesh.getBoundingBox(minX, minY, minZ, maxX, maxY, maxZ);

        cout << "x坐标范围: [" << minX << ", " << maxX << "]" << endl;

        // 5. 将x坐标范围分成ISO_VALUES_NUM份，构建等值线物理量列表

        const int ISO_VALUES_NUM = 16;

        cout << "\n步骤4: 构建等值线列表..." << endl;
        vector<double> isoValues;

        // 分成ISO_VALUES_NUM份，需要ISO_VALUES_NUM -1条等值线
        double step = (maxX - minX) / ISO_VALUES_NUM;

        for (int i = 1; i < ISO_VALUES_NUM; i++) {
            double isoValue = minX + i * step;
            isoValues.push_back(isoValue);
            cout << "  等值线 " << i << ": " << isoValue << endl;
        }

        cout << "共生成 " << isoValues.size() << " 条等值线" << endl;

        // 6. 使用等值线列表拆分input_tri_mesh
        cout << "\n步骤5: 拆分网格..." << endl;
        cout << "开始拆分，这可能需要一些时间..." << endl;

        TriangleMesh output_tri_mesh = input_tri_mesh.splitByIsolines(isoValues);

        cout << "拆分完成!" << endl;
        cout << "输出网格信息:" << endl;
        output_tri_mesh.printInfo();

        // 验证面积守恒
        double inputArea = input_tri_mesh.calculateTotalArea();
        double outputArea = output_tri_mesh.calculateTotalArea();
        double areaDiff = fabs(inputArea - outputArea);

        cout << "\n面积守恒验证:" << endl;
        cout << "  输入网格面积: " << inputArea << endl;
        cout << "  输出网格面积: " << outputArea << endl;
        cout << "  面积差异: " << areaDiff << endl;
        cout << "  是否守恒: " << (areaDiff < 1e-6 ? "是" : "否") << endl;

        // 7. 将output_tri_mesh写入到Nastran文件
        cout << "\n步骤6: 将结果写入文件 d:/fh_result.nas ..." << endl;

        NastranWriteOptions options;
        options.header_comment = "等值线拆分结果 - 基于x坐标的等值线";
        options.include_comments = true;
        options.include_temp_cards = true;
        options.precision = 10;

        NastranWriteResult writeResult = output_tri_mesh.writeToNastran("d:/fh_result.nas", options);

        if (writeResult.success) {
            cout << "写入成功!" << endl;
            cout << "  写入节点数: " << writeResult.nodes_written << endl;
            cout << "  写入三角形数: " << writeResult.elements_written << endl;
            cout << "  输出文件: d:/fh_result.nas" << endl;

            // 显示一些统计信息
            cout << "\n统计信息:" << endl;
            cout << "  输入网格 -> 输出网格:" << endl;
            cout << "    节点数: " << input_tri_mesh.getNodeCount() << " -> " << output_tri_mesh.getNodeCount() << endl;
            cout << "    三角形数: " << input_tri_mesh.getTriangleCount() << " -> " << output_tri_mesh.getTriangleCount() << endl;
            cout << "    节点数增加: " << (output_tri_mesh.getNodeCount() - input_tri_mesh.getNodeCount()) << endl;
            cout << "    三角形数增加: " << (output_tri_mesh.getTriangleCount() - input_tri_mesh.getTriangleCount()) << endl;
        }
        else {
            cout << "写入失败: " << writeResult.error_message << endl;
        }

        cout << "\n=== 实测用例完成 ===" << endl;
    }

    // ============================================================================
    // 新增测试函数：基于节点距离原点的距离构造物理量，物理量最大值分成10份作为等值线
    // ============================================================================

    inline void testIsolineSplitByDistance() {
        cout << "=== 开始实测用例：三角形网格等值线拆分（基于距离） ===" << endl;

        // 1. 创建三角形网格对象
        TriangleMesh input_tri_mesh;

        // 2. 从Nastran文件读取网格
        cout << "\n步骤1: 从文件 d:/fh.nas 读取网格..." << endl;
        NastranReadResult readResult = input_tri_mesh.readFromNastran("d:/fh.nas", true);

        if (!readResult.success) {
            cout << "错误: " << readResult.error_message << endl;
            cout << "请检查文件 d:/fh.nas 是否存在且格式正确" << endl;
            return;
        }

        cout << "读取成功!" << endl;
        cout << "  读取节点数: " << readResult.nodes_read << endl;
        cout << "  读取三角形数: " << readResult.elements_read << endl;
        cout << "  合并重复节点数: " << readResult.duplicate_nodes_merged << endl;

        input_tri_mesh.printInfo();

        // 3. 将每个节点的物理量设置为该节点到原点的距离
        cout << "\n步骤2: 设置节点物理量为到原点的距离..." << endl;
        size_t nodeCount = input_tri_mesh.getNodeCount();
        double maxDistance = 0.0;
        double minDistance = std::numeric_limits<double>::max();

        for (size_t i = 0; i < nodeCount; i++) {
            // 获取节点坐标
            vector<double> coords = input_tri_mesh.getNodeCoords(static_cast<int>(i));
            if (coords.size() >= 3) {
                // 计算到原点的距离：sqrt(x² + y² + z²)
                double x = coords[0];
                double y = coords[1];
                double z = coords[2];
                double distance = sqrt(x * x + y * y + z * z);

                // 更新最小和最大距离
                if (distance < minDistance) minDistance = distance;
                if (distance > maxDistance) maxDistance = distance;

                // 设置节点物理量为距离值
                input_tri_mesh.setNodeValue(static_cast<int>(i), distance);
            }
        }

        cout << "已设置 " << nodeCount << " 个节点的物理量（距离值）" << endl;
        cout << "距离范围: [" << minDistance << ", " << maxDistance << "]" << endl;

        // 4. 将距离范围分成ISO_VALUES_NUM份，构建等值线物理量列表
        cout << "\n步骤3: 构建等值线列表（基于距离）..." << endl;
        vector<double> isoValues;

        const int ISO_VALUES_NUM = 16;

        // 分成ISO_VALUES_NUM份，需要ISO_VALUES_NUM - 1条等值线（从最小距离到最大距离之间均匀分布）
        double step = (maxDistance - minDistance) / ISO_VALUES_NUM;

        // 生成ISO_VALUES_NUM - 1条等值线（第1份到第ISO_VALUES_NUM - 1份）
        for (int i = 1; i < ISO_VALUES_NUM; i++) {
            double isoValue = minDistance + i * step;
            isoValues.push_back(isoValue);
            cout << "  等值线 " << i << ": " << isoValue << " (距离)" << endl;
        }

        cout << "共生成 " << isoValues.size() << " 条等值线" << endl;
        cout << "等值线范围: [" << (minDistance + step) << ", " << (maxDistance - step) << "]" << endl;

        // 5. 使用等值线列表拆分input_tri_mesh
        cout << "\n步骤4: 拆分网格..." << endl;
        cout << "开始拆分，这可能需要一些时间..." << endl;

        TriangleMesh output_tri_mesh = input_tri_mesh.splitByIsolines(isoValues);

        cout << "拆分完成!" << endl;
        cout << "输出网格信息:" << endl;
        output_tri_mesh.printInfo();

        // 验证面积守恒
        double inputArea = input_tri_mesh.calculateTotalArea();
        double outputArea = output_tri_mesh.calculateTotalArea();
        double areaDiff = fabs(inputArea - outputArea);

        cout << "\n面积守恒验证:" << endl;
        cout << "  输入网格面积: " << inputArea << endl;
        cout << "  输出网格面积: " << outputArea << endl;
        cout << "  面积差异: " << areaDiff << endl;
        cout << "  是否守恒: " << (areaDiff < 1e-6 ? "是" : "否") << endl;

        // 6. 将output_tri_mesh写入到Nastran文件
        cout << "\n步骤5: 将结果写入文件 d:/fh_result_distance.nas ..." << endl;

        NastranWriteOptions options;
        options.header_comment = "等值线拆分结果 - 基于距离原点的距离";
        options.include_comments = true;
        options.include_temp_cards = true;
        options.precision = 10;

        NastranWriteResult writeResult = output_tri_mesh.writeToNastran("d:/fh_result_distance.nas", options);

        if (writeResult.success) {
            cout << "写入成功!" << endl;
            cout << "  写入节点数: " << writeResult.nodes_written << endl;
            cout << "  写入三角形数: " << writeResult.elements_written << endl;
            cout << "  输出文件: d:/fh_result_distance.nas" << endl;

            // 显示一些统计信息
            cout << "\n统计信息:" << endl;
            cout << "  输入网格 -> 输出网格:" << endl;
            cout << "    节点数: " << input_tri_mesh.getNodeCount() << " -> " << output_tri_mesh.getNodeCount() << endl;
            cout << "    三角形数: " << input_tri_mesh.getTriangleCount() << " -> " << output_tri_mesh.getTriangleCount() << endl;
            cout << "    节点数增加: " << (output_tri_mesh.getNodeCount() - input_tri_mesh.getNodeCount()) << endl;
            cout << "    三角形数增加: " << (output_tri_mesh.getTriangleCount() - input_tri_mesh.getTriangleCount()) << endl;

            // 输出距离范围信息
            cout << "\n距离范围信息:" << endl;
            cout << "  最小距离: " << minDistance << endl;
            cout << "  最大距离: " << maxDistance << endl;
            cout << "  距离差值: " << (maxDistance - minDistance) << endl;
            cout << "  等值线间隔: " << step << endl;
        }
        else {
            cout << "写入失败: " << writeResult.error_message << endl;
        }

        cout << "\n=== 基于距离的等值线拆分测试完成 ===" << endl;
    }

    // 辅助函数：验证等值线是否正确分布
    inline void verifyIsolineDistribution(const TriangleMesh& mesh, const vector<double>& isoValues) {
        cout << "\n验证等值线分布..." << endl;

        // 检查每个等值线是否在节点值范围内
        double minValue, maxValue;
        mesh.getValueRange(minValue, maxValue);

        cout << "节点值范围: [" << minValue << ", " << maxValue << "]" << endl;

        for (size_t i = 0; i < isoValues.size(); i++) {
            double isoVal = isoValues[i];
            if (isoVal < minValue || isoVal > maxValue) {
                cout << "警告: 等值线 " << i << " (值=" << isoVal << ") 超出节点值范围!" << endl;
            }
            else {
                cout << "等值线 " << i << " (值=" << isoVal << ") 在节点值范围内" << endl;
            }
        }
    }

} // namespace TriangleMeshIsoLine

#endif // TRIANGLEMESHISOLINE_H