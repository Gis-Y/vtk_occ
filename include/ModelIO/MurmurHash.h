/*******************************************************************************

程序说明

计算Hash值的函数，参考7.8.0版本的计算哈希值函数，调用方式与7.8.0一致

*******************************************************************************/

#ifndef MURMURHASH_H
#define MURMURHASH_H

#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <TopLoc_Datum3D.hxx>
#include <TopLoc_SListOfItemLocation.hxx>
#include <TopLoc_Location.hxx>
#include <TopLoc_ItemLocation.hxx>
#include <string>

namespace opencascade
{
	namespace MurmurHash
	{
		inline uint64_t shift_mix(uint64_t theV)
		{
			return theV ^ (theV >> 47);
		}

		// Loads n bytes, where 1 <= n < 8.
		inline uint64_t load_bytes(const char* thePnt, int theNb)
		{
			uint64_t aRes = 0;
			--theNb;
			do
				aRes = (aRes << 8) + static_cast<unsigned char>(thePnt[theNb]);
			while (--theNb >= 0);
			return aRes;
		}
		template <typename T>
		inline T unaligned_load(const char* thePnt)
		{
			T aRes;
			memcpy(&aRes, thePnt, sizeof(aRes));
			return aRes;
		}

		//=======================================================================
		//function : MurmurHash64A
		//purpose  :
		//=======================================================================
		inline uint64_t MurmurHash64A(const void* theKey, int theLen, uint64_t theSeed)
		{
			static constexpr uint64_t aMul = (((uint64_t)0xc6a4a793UL) << 32UL)
				+ (uint64_t)0x5bd1e995UL;
			const char* const aBuf = static_cast<const char*>(theKey);

			// Remove the bytes not divisible by the sizeof(uint64_t).  This
			// allows the main loop to process the data as 64-bit integers.
			const uint64_t aLenAligned = theLen & ~(uint64_t)0x7;
			const char* const anEnd = aBuf + aLenAligned;
			uint64_t aHash = theSeed ^ (theLen * aMul);
			for (const char* aPnt = aBuf; aPnt != anEnd; aPnt += 8)
			{
				const uint64_t aData = shift_mix(unaligned_load<uint64_t>(aPnt) * aMul) * aMul;
				aHash ^= aData;
				aHash *= aMul;
			}
			if ((theLen & 0x7) != 0)
			{
				const uint64_t data = load_bytes(anEnd, theLen & 0x7);
				aHash ^= data;
				aHash *= aMul;
			}
			aHash = shift_mix(aHash) * aMul;
			aHash = shift_mix(aHash);
			return aHash;
		}

		//=======================================================================
		//function : MurmurHash2A
		//purpose  :
		//=======================================================================
		inline uint32_t MurmurHash2A(const void* theKey, int theLen, uint32_t theSeed)
		{
			const uint32_t aMul = 0x5bd1e995;
			uint32_t aHash = theSeed ^ theLen;
			const char* aBuf = static_cast<const char*>(theKey);

			// Mix 4 bytes at a time into the hash.
			while (theLen >= 4)
			{
				uint32_t aKey = unaligned_load<uint32_t>(aBuf);
				aKey *= aMul;
				aKey ^= aKey >> 24;
				aKey *= aMul;
				aHash *= aMul;
				aHash ^= aKey;
				aBuf += 4;
				theLen -= 4;
			}

			uint32_t aKey;
			// Handle the last few bytes of the input array.
			switch (theLen)
			{
			case 3:
				aKey = static_cast<unsigned char>(aBuf[2]);
				aHash ^= aKey << 16;
				Standard_FALLTHROUGH
			case 2:
				aKey = static_cast<unsigned char>(aBuf[1]);
				aHash ^= aKey << 8;
				Standard_FALLTHROUGH
			case 1:
				aKey = static_cast<unsigned char>(aBuf[0]);
				aHash ^= aKey;
				aHash *= aMul;
			};

			// Do a few final mixes of the hash.
			aHash ^= aHash >> 13;
			aHash *= aMul;
			aHash ^= aHash >> 15;
			return aHash;
		}


		template <typename T1, typename T = size_t>
		typename std::enable_if<sizeof(T) == 8, uint64_t>::type
			hash_combine(const T1& theValue, const int theLen = sizeof(T1), const T theSeed = 0xA329F1D3A586ULL)
		{
			return MurmurHash::MurmurHash64A(&theValue, theLen, theSeed);
		}

		template <typename T1, typename T = size_t>
		typename std::enable_if<sizeof(T) != 8, T>::type
			hash_combine(const T1& theValue, const int theLen = sizeof(T1), const T theSeed = 0xc70f6907U)
		{
			return static_cast<T>(MurmurHash::MurmurHash2A(&theValue, theLen, theSeed));
		}

		template <typename T = size_t>
		constexpr T optimalSeed()
		{
			return sizeof(T) == 8 ? static_cast<T>(0xA329F1D3A586ULL) : static_cast<T>(0xc70f6907U);
		}
	};

	template <typename T1, typename T = size_t>
	T hash(const T1 theValue) noexcept
	{
		return opencascade::MurmurHash::hash_combine<T1, T>(theValue);
	}

	template <typename T1, typename T = size_t>
	T hashBytes(const T1* theKey, int theLen)
	{
		return opencascade::MurmurHash::hash_combine<T1, T>(*theKey, theLen);
	}

	template <typename T1, typename T = size_t>
	T hash_combine(const T1 theValue, const int theLen, const T theSeed)
	{
		return opencascade::MurmurHash::hash_combine<T1, T>(theValue, theLen, theSeed);
	}
};


namespace std
{
	inline size_t GetLocationHashCode(const TopLoc_Location& loc)
	{
		gp_Trsf T = loc.Transformation();
		TopLoc_SListOfItemLocation myItems;
		Handle(TopLoc_Datum3D) D = new TopLoc_Datum3D(T);
		myItems.Construct(TopLoc_ItemLocation(D, 1));

		// Hashing base on IsEqual function
		if (myItems.IsEmpty())
		{
			return 0;
		}
		size_t aHash = opencascade::MurmurHash::optimalSeed<size_t>();
		TopLoc_SListOfItemLocation items = myItems;
		size_t aCombined[3];
		while (items.More())
		{
			//aCombined[0] = std::hash<Handle(TopLoc_Datum3D)>{}(items.Value().myDatum);
			aCombined[0] = static_cast<size_t>(reinterpret_cast<std::uintptr_t>(D.get()));
			aCombined[1] = opencascade::hash(1);
			aCombined[2] = aHash;
			aHash = opencascade::hashBytes(aCombined, sizeof(aCombined));
			items.Next();
		}
		return aHash;
	}

	template <class TheTransientType>
	struct hash<Handle(TheTransientType)>
	{
		size_t operator()(const Handle(TheTransientType)& theHandle) const noexcept
		{
			return static_cast<size_t>(reinterpret_cast<std::uintptr_t>(theHandle.get()));
		}
	};

	template <>
	struct hash<TopLoc_Location>
	{
		size_t operator()(const TopLoc_Location& theLocation) const
		{
			return std::GetLocationHashCode(theLocation);
		}
	};

	template <>
	struct hash<TopoDS_Shape>
	{
		size_t operator()(const TopoDS_Shape& theShape) const noexcept
		{
			const size_t aHL = std::hash<TopLoc_Location>{}(theShape.Location());
			return aHL == 0 ? opencascade::hash(theShape.TShape().get())
				: opencascade::MurmurHash::hash_combine(theShape.TShape().get(), sizeof(void*), aHL);
		}
	};

	template <>
	struct hash<TopoDS_Face>
	{
		size_t operator()(const TopoDS_Face& theShape) const
		{
			return std::hash<TopoDS_Shape>{}(theShape);
		}
	};

	template <>
	struct hash<TopoDS_Edge>
	{
		size_t operator()(const TopoDS_Edge& theShape) const
		{
			return std::hash<TopoDS_Shape>{}(theShape);
		}
	};
};
#endif // MURMURHASH_H