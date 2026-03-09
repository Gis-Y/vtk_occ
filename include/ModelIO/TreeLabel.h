/*******************************************************************************

用于显示模型的结构树

根节点名称定义为Model

节点分为两种类型，一种为TreeShape，包含子节点，但不包含具体的Face的数据，faceId均为0；
一种为TreeFace，从属于TreeShape，存储了真正的Face数据，faceId大于0；


*******************************************************************************/


#pragma once

#include <TopoDS_Shape.hxx>
#include <TDF_Label.hxx>
#include <string>
#include <vector>

//#define UPPERBOUND		2147483647
#define UPPERBOUND			1999999999

using namespace std;

enum TreeType
{
	TreeShape,
	TreeFace
};
struct TreeLabel
{
	TreeLabel()
	{
		id = 0;
		Name = "Model";
	}
	TreeLabel(const TopoDS_Shape &_Shape, TreeType _NodeType = TreeShape, int _Level = 0) :
		Shape(_Shape), NodeType(_NodeType), Level(_Level)
	{
		Name = GetName();
	}
	TreeLabel(const TDF_Label &_Label, const TopoDS_Shape &_Shape, TreeType _NodeType = TreeShape, int _Level = 0) :
		Label(_Label), Shape(_Shape), NodeType(_NodeType), Level(_Level)
	{
		Name = GetName();
	}

	TDF_Label Label;
	TopoDS_Shape Shape;
	TreeType NodeType;
	int id;
	int Level;	
	string Name;
	//int HashCode;
	//int faceId;
	//int sid;
	//vector<int> subFaceHashCodes;

	friend bool operator < (const TreeLabel& lhs, const TreeLabel& rhs)
	{
		return lhs.id < rhs.id;
	}
	friend bool operator == (const TreeLabel& lhs, const TreeLabel& rhs)
	{
		return lhs.id == rhs.id;
	}

	string GetName()
	{
		TCollection_ExtendedString str = "";

		if (Label.IsNull() && Shape.IsNull())
		{
			str = "Model";
		}

		if (!Label.IsNull())
		{
			Handle(TDataStd_Name) anAttribute;

			if (Label.FindAttribute(TDataStd_Name::GetID(), anAttribute))
			{
				str = anAttribute->Get();
			}
		}

		if (str == "")
		{
			switch (Shape.ShapeType())
			{
			case TopAbs_COMPOUND:
				str = "Compound";
				break;
			case TopAbs_COMPSOLID:
				str = "CSolid";
				break;
			case TopAbs_SOLID:
				str = "Solid";
				break;
			case  TopAbs_SHELL:
				str = "Shell";
				break;
			case TopAbs_FACE:
				str = "Face";
				break;
			case TopAbs_WIRE:
				str = "Wire";
				break;
			case TopAbs_EDGE:
				str = "Edge";
				break;
			case TopAbs_VERTEX:
				str = "Vertex";
				break;
			case TopAbs_SHAPE:
				str = "Shape";
				break;
			default:
				break;
			}
		}

		char* ch = new char[str.LengthOfCString() + 1];
		str.ToUTF8CString(ch);
		return ch;
	}

	string GetJsonName()
	{
		string tail;

		//if (id < 10)
		//{
		//	tail = "_0000" + to_string(id);
		//}
		//else if (id < 100)
		//{
		//	tail = "_000" + to_string(id);
		//}
		//else if (id < 1000)
		//{
		//	tail = "_00" + to_string(id);
		//}
		//else if (id < 10000)
		//{
		//	tail = "_0" + to_string(id);
		//}
		//else
		//{
		//	tail = "_" + to_string(id);
		//}

		tail = "_" + to_string(id);
		return Name + tail;
	}
};
