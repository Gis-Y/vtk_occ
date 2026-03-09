#ifndef _THREE_ANIMATION_HXX
#define _THREE_ANIMATION_HXX

#include <vector>
#include <string>
#include <map>
#include <set>
#include <iostream>
#include <iomanip>
#include <fstream>

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <cstring>
#include<windows.h>

#include "threeMath.hxx"

using namespace std;

namespace three {

class GLTFFrame
{
public:
	vector<double> position;
	vector<double> normals;
	int vertexCounts;
	int morphTargetInfluenceCounts;
	int boneCounts;

	void getPosition(const vector<double> &initPosition, 
						const vector< vector<double> > &morphAttributes,
						const vector<size_t> &skinIndex,
						const vector<double> &skinWeight,
						const vector<double> &bindMatrix,
						const vector<double> &bindMatrixInverse,
						const vector<double> &transformMatrix,
						const vector<double> &morphTargetInfluence,
						const vector<double> &boneInversesMatrix,
						const vector<double> &boneMatrixWorldMatrix,
						bool morphTargetsRelative
						)
	{
		position.assign(initPosition.begin(), initPosition.end());

		if (morphAttributes.size() > 0)
		{
			MorphAction(morphAttributes, morphTargetInfluence, morphTargetsRelative);
		}

		if (boneInversesMatrix.size() > 0)
		{
			SketelonAction(skinIndex, skinWeight, bindMatrix, bindMatrixInverse, boneMatrixWorldMatrix, boneInversesMatrix);
		}

		// TransformationAction(transformMatrix);
	}

	void TransformationAction(const vector<double> &transformMatrix)
	{
		// Transform Position
		for (unsigned int i = 0; i < position.size() / 3; i++)
		{
			Vector4<double> vertex(position[i * 3], position[i * 3 + 1], position[i * 3 + 2], 1);
			Matrix4 mat(transformMatrix);
			Vector4<double> vertexTrans;

			vertexTrans = vertex.applyMatrix4(mat);

			position[i * 3] = vertexTrans.x;
			position[i * 3 + 1] = vertexTrans.y;
			position[i * 3 + 2] = vertexTrans.z;
		}
	}
	void MorphAction(const vector< vector<double> > &morphAttributes, const vector<double> &morphTargetInfluence, bool morphTargetsRelative)
	{
		// Morph Position
		for (unsigned int a = 0; a < position.size() / 3; a++)
		{
			Vector3 _pos, _morph;
			_pos.fromBufferAttribute(position, a);
			_morph.set(0, 0, 0);

			for (size_t i = 0; i < morphAttributes.size(); i++) {
				double influence = morphTargetInfluence[i];
				//const morphAttribute = morphAttributes[i];
				Vector3 _temp;
				_temp.fromBufferAttribute( morphAttributes[i], a);

				if (morphTargetsRelative) {
					_morph.addScaledVector(_temp, influence);
				} else {
					_morph.addScaledVector(_temp.sub(_pos), influence);
				}
			}
					
			_pos.add(_morph);

			position[a * 3] = _pos.x;
			position[a * 3 + 1] = _pos.y;
			position[a * 3 + 2] = _pos.z;
		}
	}

	void SketelonAction(const vector<size_t> &skinIndex,
						const vector<double> &skinWeight,
						const vector<double> &bindMatrix, 
						const vector<double> &bindMatrixInverse,
						const vector<double> &boneMatrixWorld,
						const vector<double> &boneInverses)
	{
		for (unsigned int index = 0; index < position.size() / 3; index++)
		{

			Vector4<size_t> _skinIndex;
			Vector4<double> _skinWeight;

			Vector3 _basePosition;
			_basePosition.fromBufferAttribute(position, index);

			_skinIndex.fromBufferAttribute(skinIndex, index);
			_skinWeight.fromBufferAttribute(skinWeight, index);

			_basePosition.applyMatrix4(bindMatrix);

			Vector3 target;

			for (int i = 0; i < 4; i++) {
				double weight = _skinWeight.getComponent(i);

				if (fabs(weight) > 1e-7) {
					size_t boneIndex = _skinIndex.getComponent(i);

					Matrix4 _matrix;

					vector<double> _boneMatrixWorld;
					vector<double> _boneInverses;
					_boneMatrixWorld.assign(boneMatrixWorld.begin() + boneIndex * 16, boneMatrixWorld.begin() + boneIndex * 16 + 16);
					_boneInverses.assign(boneInverses.begin() + boneIndex * 16, boneInverses.begin() + boneIndex * 16 + 16);
					_matrix.multiplyMatrices(_boneMatrixWorld, _boneInverses);

					Vector3 _vector;

					target.addScaledVector(_vector.copy(_basePosition).applyMatrix4(_matrix), weight);
				}
			}

			target.applyMatrix4(bindMatrixInverse);

			position[index * 3] = target.x;
			position[index * 3 + 1] = target.y;
			position[index * 3 + 2] = target.z;
		}
	}
	void ComputeVertexNormals(const vector<int> index)
	{
		normals.resize(position.size());
		vector< set<int> > nodeAdjTri;
		nodeAdjTri.resize(position.size()/3);           // vertex number: (positon size / 3)

		vector<Vector3> triNormals;
		triNormals.resize(index.size()/3);              // triangle number: (index size / 3)

		for(unsigned int i = 0; i < index.size() / 3; i++)
		{
			Vector3 pt0(position[index[i] * 3], position[index[i] * 3 + 1], position[index[i] * 3 + 2]);
			Vector3 pt1(position[index[i + 1] * 3], position[index[i + 1] * 3 + 1], position[index[i + 1] * 3 + 2]);
			Vector3 pt2(position[index[i + 2] * 3], position[index[i + 2] * 3 + 1], position[index[i + 2] * 3 + 2]);

			Vector3 v1 = pt1 - pt0;
			Vector3 v2 = pt2 - pt1;
			Vector3 vNor = v1 ^ v2;
			vNor.normalize();
			triNormals[i] = vNor;

			nodeAdjTri[index[i]].insert(i);
			nodeAdjTri[index[i + 1]].insert(i);
			nodeAdjTri[index[i + 2]].insert(i);
		}


		for(unsigned int iNodeAdjTri = 0; iNodeAdjTri < nodeAdjTri.size(); iNodeAdjTri++)
		{
			cout << nodeAdjTri[iNodeAdjTri].size() << endl;
			Vector3 _normal;
			for(set<int>::iterator iter = nodeAdjTri[iNodeAdjTri].begin(); iter != nodeAdjTri[iNodeAdjTri].end(); iter++)
			{
				_normal = _normal + triNormals[*iter];
			}
			_normal.x = _normal.x /nodeAdjTri[iNodeAdjTri].size();
			_normal.y = _normal.y /nodeAdjTri[iNodeAdjTri].size();
			_normal.z = _normal.z /nodeAdjTri[iNodeAdjTri].size();
			_normal.normalize();

			normals[iNodeAdjTri * 3] = _normal.x;
			normals[iNodeAdjTri * 3 + 1] = _normal.y;
			normals[iNodeAdjTri * 3 + 2] = _normal.z;
		}
	}
};


class GLTFAnimation
{
public:
	unsigned int aid;
	unsigned int frameCounts;
	vector<GLTFFrame> frames;
};

class GLTFMesh
{
public:
	string id;
	string name;
	string uid;
	unsigned int vertexCounts, indexCounts, morphTargetInfluencesCounts, bonesCounts;
	unsigned int normalsCounts, uvCounts, uv2Counts, colorCounts, skinCounts;
	unsigned int fps, frameCounts;
	unsigned int animationCounts;

	bool morphTargetsRelative;
	string bindMode;

	vector<int> index;

	vector<double> position; // vertexCounts * 3
	vector<double> normal;
	vector<double> color; // vertex * 4
	vector<double> uv;    // vertex * 2
	vector<double> uv2;   // vertex * 2
	vector< vector<double> > morphAttributes;

	vector<double> bindMatrix; // 0 or 16
	vector<double> bindMatrixInverse; // 0 or 16
	vector<double> skinWeight;        // 0 or vertex * 4
	vector<size_t> skinIndex;

	vector<double> transformMatrices;      // frameCounts*16
	vector<double> morphTargetInfluences; //     frameCounts*morphTargetInfluencesCounts*3
	vector<double> boneInversesMatrices;           // boneCounts *16 * vertexCounts
	vector<double> boneMatrixWorldMatrices;           // boneCounts *16 * vertexCounts

	vector<GLTFAnimation> animations;

	void SetAnimationProperty(size_t _fps, size_t  _frameCounts)
	{
		fps = _fps;
		frameCounts = _frameCounts;
	}

	void ReadCache(const string &cacheFileBaseName)
	{
		cout << "id: " << id << endl;

		cout << "Read Position..." << endl;
		string positionFile = cacheFileBaseName + ".position" + id;
		getFileContent(positionFile, position);
		vertexCounts = position.size() / 3;

		cout << "Read Index..." << endl;
		string indexFile = cacheFileBaseName + ".index" + id;
		getFileContent(indexFile, index);
		indexCounts = index.size() / 3;

		cout << "Read or Calculate Normal..." << endl;
		string normalFile = cacheFileBaseName + ".normal" + id;
		if (!getFileContent(normalFile, normal))
		{
		        ComputeVertexNormals();
		}

		cout << "Read morphAttributes..." << endl;
		string morphAttributesFile = cacheFileBaseName + ".morphAttributes" + id;
		vector<double> morphAttributesList;
		getFileContent(morphAttributesFile, morphAttributesList);

		morphTargetInfluencesCounts = morphAttributesList.size() / position.size();

		for (unsigned int iMorphTargetInfluencesCount = 0; iMorphTargetInfluencesCount < morphTargetInfluencesCounts; iMorphTargetInfluencesCount++)
		{
			vector<double> morphAttribute;
			for (unsigned int iMorph = 0; iMorph < vertexCounts * 3; iMorph++)
			{
				morphAttribute.push_back(morphAttributesList[iMorphTargetInfluencesCount * vertexCounts * 3 + iMorph]);
			}
			morphAttributes.push_back(morphAttribute);
		}

		cout << "Read Skeleton Weight..." << endl;
		string skinWeightFile = cacheFileBaseName + ".skinWeight" + id;
		getFileContent(skinWeightFile, skinWeight);

		cout << "Read Skeleton Index..." << endl;
		string skinIndexFile = cacheFileBaseName + ".skinIndex" + id;
		getFileContent(skinIndexFile, skinIndex);

		cout << "Read bindMatrix..." << endl;
		string bindMatrixFile = cacheFileBaseName + ".bindMatrix" + id;
		getFileContent(bindMatrixFile, bindMatrix);

		cout << "Read bindMatrixInverse..." << endl;
		string bindMatrixInverseFile = cacheFileBaseName + ".bindMatrixInverse" + id;
		getFileContent(bindMatrixInverseFile, bindMatrixInverse);

		// Read Animation

		cout << "Read transformMatrix..." << endl;
		string transformMatrixsFile = cacheFileBaseName + ".transformMatrix" + id;
		getFileContent(transformMatrixsFile, transformMatrices);
		animationCounts = transformMatrices.size() / frameCounts / 16;

		cout << "Read morphTargetInfluences..." << endl;
		string morphTargetInfluencesFile = cacheFileBaseName + ".morphTargetInfluences" + id;
		getFileContent(morphTargetInfluencesFile, morphTargetInfluences);

		cout << "Read boneInverses..." << endl;
		string boneInversesFile = cacheFileBaseName + ".boneInverses" + id;
		getFileContent(boneInversesFile, boneInversesMatrices);

		cout << "Read boneMatrixWorld..." << endl;
		string boneMatrixWorldFile = cacheFileBaseName + ".boneMatrixWorld" + id;
		//string boneMatrixWorldFile = cacheFileBaseName + ".boneMatrics" + id;
		getFileContent(boneMatrixWorldFile, boneMatrixWorldMatrices);

	}

	void parseAnimation()
	{
		cout <<"parseAnimation ... " << endl;
		for (size_t aid = 0; aid < animationCounts; aid++)
		{
			GLTFAnimation _gltfAnimation;
			_gltfAnimation.aid = aid;
			_gltfAnimation.frameCounts = frameCounts;

			for (size_t frameId = 0; frameId < frameCounts; frameId++)
			{
				GLTFFrame _gltfFrame;
				
				vector<double> transformMatrix;      // frameCounts*16
				vector<double> morphTargetInfluence; //     frameCounts*morphTargetInfluencesCounts*3
				vector<double> boneInversesMatrix;           // boneCounts *16 * vertexCounts
				vector<double> boneMatrixWorldMatrix;           // boneCounts *16 * vertexCounts

				transformMatrix.assign(transformMatrices.begin() + aid * frameCounts + frameId * 16, transformMatrices.begin() + aid * frameCounts + frameId * 16 + 16);
				morphTargetInfluence.assign(morphTargetInfluences.begin() + aid * frameCounts * morphTargetInfluencesCounts + frameId * morphTargetInfluencesCounts,
					morphTargetInfluences.begin() + aid * frameCounts * morphTargetInfluencesCounts + frameId * morphTargetInfluencesCounts + morphTargetInfluencesCounts);

				boneInversesMatrix.assign(boneInversesMatrices.begin()  + aid * frameCounts * bonesCounts * 16 + frameId *  bonesCounts * 16, 
					boneInversesMatrices.begin()  + aid * frameCounts * bonesCounts * 16 + frameId *  bonesCounts * 16 + bonesCounts * 16);
				boneMatrixWorldMatrix.assign(boneMatrixWorldMatrices.begin()  + aid * frameCounts * bonesCounts * 16 + frameId *  bonesCounts * 16, 
					boneMatrixWorldMatrices.begin()  + aid * frameCounts * bonesCounts * 16 + frameId *  bonesCounts * 16 + bonesCounts * 16);

				_gltfFrame.getPosition(position, 
					morphAttributes,
					skinIndex,
					skinWeight,
					bindMatrix,
					bindMatrixInverse,
					transformMatrix,
					morphTargetInfluence,
					boneInversesMatrix,
					boneMatrixWorldMatrix,
					morphTargetsRelative
					);

				_gltfAnimation.frames.push_back(_gltfFrame);
			}

			animations.push_back(_gltfAnimation);
		}

	}

	template<typename T>
	bool getFileContent(const string& filePath, vector<T> &arr)
	{
	        ifstream infile;
	        infile.open(filePath.c_str(), ios::in);
	
	        if (!infile.is_open())
	        {
	            return false;
	        }
	
	        while (!infile.eof())
	        {
				T temp;
				infile >> temp;
				arr.push_back(temp);
	        }
	
	        arr.pop_back();
	
	        infile.close();
	        return true;
	}

	void ComputeVertexNormals()
	{
		normal.resize(position.size());

		if (position.size() !=0 ) {

			if (index.size() > 0) {

				for (unsigned int i = 0, il = index.size(); i < il; i += 3) {

					int vA = index[i + 0];
					int vB = index[i + 1];
					int vC = index[i + 2];

					Vector3 pA(position[vA * 3], position[vA * 3 + 1], position[vA * 3 + 2]);
					Vector3 pB(position[vB * 3], position[vB * 3 + 1], position[vB * 3 + 2]);
					Vector3 pC(position[vC * 3], position[vC * 3 + 1], position[vC * 3 + 2]);

					Vector3 cb = pC - pB;
					Vector3 ab = pA - pB;
					cb = cb ^ ab;

					Vector3 nA(normal[vA * 3], normal[vA * 3 + 1], normal[vA * 3 + 2]);
					Vector3 nB(normal[vB * 3], normal[vB * 3 + 1], normal[vB * 3 + 2]);
					Vector3 nC(normal[vC * 3], normal[vC * 3 + 1], normal[vC * 3 + 2]);

					nA = nA + cb;
					nB = nB + cb;
					nC = nC + cb;

					normal[vA * 3] = nA.x;
					normal[vA * 3 + 1] = nA.y;
					normal[vA * 3 + 2] = nA.z;

					normal[vB * 3] = nB.x;
					normal[vB * 3 + 1] = nB.y;
					normal[vB * 3 + 2] = nB.z;

					normal[vC * 3] = nC.x;
					normal[vC * 3 + 1] = nC.y;
					normal[vC * 3 + 2] = nC.z;
				}

			} else {
				// non-indexed elements (unconnected triangle soup)
				for (unsigned int i = 0, il = position.size(); i < il; i += 3) {

					Vector3 pA(position[i * 3], position[i * 3 + 1], position[i * 3 + 2]);
					Vector3 pB(position[(i + 1) * 3], position[(i + 1) * 3 + 1], position[(i + 1) * 3 + 2]);
					Vector3 pC(position[(i + 2) * 3], position[(i + 2) * 3 + 1], position[(i + 2) * 3 + 2]);

					Vector3 cb = pC - pB;
					Vector3 ab = pA - pB;
					cb = cb ^ ab;

					normal[i * 3] = cb.x;
					normal[i * 3 + 1] = cb.y;
					normal[i * 3 + 2] = cb.z;

					normal[(i + 1) * 3] = cb.x;
					normal[(i + 1) * 3 + 1] = cb.y;
					normal[(i + 1) * 3 + 2] = cb.z;

					normal[(i + 2) * 3] = cb.x;
					normal[(i + 2) * 3 + 1] = cb.y;
					normal[(i + 2) * 3 + 2] = cb.z;
				}
			}

			for (unsigned int i = 0, il = normal.size(); i < il; i += 3) {
				Vector3 _nor(normal[i], normal[i+1], normal[i+2]);
				_nor.normalize();
				normal[i] = _nor.x;
				normal[i + 1] = _nor.y;
				normal[i + 2] = _nor.z;
			}
		}

	}
};

} // three
#endif