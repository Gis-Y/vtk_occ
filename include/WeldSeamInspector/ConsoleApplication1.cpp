// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "ShapeAnalyzer.h"

int main()
{
	ShapeAnalyzer analyzer;
	std::string stepFile = "D:/experience.stp";
	if (!analyzer.Read(stepFile)) {
		std::cout << "Failed to read model file: " << stepFile << std::endl;
		return -1;
	}

	std::cout << "=== 形状分析器 ===" << std::endl;
	std::cout << "成功读取模型文件: " << stepFile << std::endl;

	// 输出形状映射信息
	analyzer.PrintShapeMapping();

	// 构建面ID映射表（外部传入）
	std::cout << "\n=== 构建面ID映射表 ===" << std::endl;
	NCollection_IndexedDataMap<int, FaceProperty> faceMap;
	int nextFaceId = 1;
	analyzer.BuildFaceMap(faceMap, nextFaceId);

	// 输出面映射信息
	analyzer.PrintFaceMapInfo(faceMap);

	// 使用新的形状关系检测功能
	std::cout << "\n=== 开始形状关系检测 ===" << std::endl;
	std::vector<DetectionResult> detectionResults = analyzer.DetectShapeRelations(
		5.0,    // 最大检测距离
		1e-7,   // 重合面积阈值
		1e-3    // 平行度容差
	);

	// 输出检测结果
	analyzer.PrintDetectionResults(detectionResults);

	// 演示如何获取面对信息
	if (!detectionResults.empty()) {
		std::cout << "\n=== 面对详细信息示例 ===" << std::endl;
		for (size_t i = 0; i < std::min((size_t)3, detectionResults.size()); ++i) {
			const auto& result = detectionResults[i];
			std::vector<std::pair<TopoDS_Face, TopoDS_Face>> facePairs;
			analyzer.GetFacePairs(result, facePairs);

			std::cout << "关系 " << (i + 1) << " (" << result.shape1Name
				<< " <-> " << result.shape2Name << ") 的面对数量: "
				<< facePairs.size() << std::endl;
		}
	}

	// 性能统计
	auto perfStats = analyzer.GetLastPerformanceStats();
	std::cout << "\n=== 详细性能统计 ===" << std::endl;
	std::cout << "平均每对形状处理时间: " << (perfStats.processedPairs > 0 ?
		perfStats.totalTime * 1000 / perfStats.processedPairs : 0) << " 毫秒" << std::endl;

	// 转换为JSON格式并保存
	if (!detectionResults.empty()) {
		std::cout << "\n=== JSON转换 ===" << std::endl;

		// 转换为JSON字符串
		std::string jsonString = analyzer.ConvertDetectionResultsToJson(detectionResults, faceMap, nextFaceId);
		std::cout << "JSON格式结果预览（前500字符）:\n"
			<< jsonString.substr(0, 500) << (jsonString.length() > 500 ? "..." : "") << std::endl;

		// 保存为JSON文件
		analyzer.SaveDetectionResultsAsJson(detectionResults, faceMap, nextFaceId, "D:\\detection_results.json");

		// 导出相关面到STEP文件
		analyzer.ExportAttachedFaces(detectionResults, "D:\\detection_results.stp");
	}

	// 执行原有的分析处理（可选）
	std::cout << "\n=== 执行传统分析 ===" << std::endl;
	//analyzer.Process();

	std::cout << "\n=== 所有分析完成 ===" << std::endl;

	// 可选：演示文档管理接口
	// DocumentManagementExample();

	return 0;
}

// 独立的辅助函数，演示如何在外部使用面映射表
void ExternalUsageExample() {
	std::cout << "\n=== 外部使用面映射表示例 ===" << std::endl;

	// 创建外部的面映射表
	NCollection_IndexedDataMap<int, FaceProperty> externalFaceMap;
	int externalNextFaceId = 1;

	ShapeAnalyzer analyzer;
	if (analyzer.Read("D:/experience.stp")) {
		// 构建面映射表
		analyzer.BuildFaceMap(externalFaceMap, externalNextFaceId);

		// 进行形状关系检测
		auto results = analyzer.DetectShapeRelations(5.0, 1e-7, 1e-3);

		// 转换为JSON（使用外部映射表）
		std::string json = analyzer.ConvertDetectionResultsToJson(results, externalFaceMap, externalNextFaceId);

		// 保存结果（使用外部映射表）
		analyzer.SaveDetectionResultsAsJson(results, externalFaceMap, externalNextFaceId, "D:\\external_results.json");

		std::cout << "外部面映射表使用完成，总面数: " << externalFaceMap.Extent() << std::endl;
	}
}

// 演示文档管理接口的使用
void DocumentManagementExample() {
	std::cout << "\n=== 文档管理接口示例 ===" << std::endl;

	// 方法1：创建外部文档并设置
	Handle(TDocStd_Application) app = new TDocStd_Application;
	Handle(TDocStd_Document) externalDoc;
	app->NewDocument("BinXCAF", externalDoc);

	ShapeAnalyzer analyzer;

	// 检查是否有文档
	std::cout << "初始文档状态: " << (analyzer.HasDocument() ? "有文档" : "无文档") << std::endl;

	// 设置外部创建的文档
	analyzer.SetDocument(externalDoc);
	std::cout << "设置文档后状态: " << (analyzer.HasDocument() ? "有文档" : "无文档") << std::endl;

	// 获取文档进行操作
	Handle(TDocStd_Document) currentDoc = analyzer.GetDocument();
	if (!currentDoc.IsNull()) {
		std::cout << "成功获取到文档对象" << std::endl;

		// 可以对文档进行进一步操作
		TDF_Label mainLab = currentDoc->Main();
		Handle(XCAFDoc_ShapeTool) ST = XCAFDoc_DocumentTool::ShapeTool(mainLab);
		std::cout << "文档工具已初始化" << std::endl;
	}

	// 方法2：使用Read方法加载文档，然后获取
	ShapeAnalyzer analyzer2;
	if (analyzer2.Read("D:/experience.stp")) {
		std::cout << "通过Read方法加载文档: " << (analyzer2.HasDocument() ? "成功" : "失败") << std::endl;

		// 获取由Read方法创建的文档
		Handle(TDocStd_Document) loadedDoc = analyzer2.GetDocument();
		if (!loadedDoc.IsNull()) {
			std::cout << "成功获取Read方法创建的文档" << std::endl;
		}
	}
}