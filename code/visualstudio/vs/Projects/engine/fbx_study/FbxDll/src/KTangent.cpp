#include "stdafx.h"
#include "KTangent.h"
#include "TangentSpaceCalculation.h"
//#include "max.h"

#ifndef DEBUG_NEW
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif


#define ASSERT assert


FLOAT KVec3Dot( CONST KVec3 *pV1, CONST KVec3 *pV2 )
{
	return pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z;
}

KVec3* KVec3Cross( KVec3 *pOut, CONST KVec3 *pV1, CONST KVec3 *pV2 )
{
	KVec3 v;

	v.x = pV1->y * pV2->z - pV1->z * pV2->y;
	v.y = pV1->z * pV2->x - pV1->x * pV2->z;
	v.z = pV1->x * pV2->y - pV1->y * pV2->x;

	*pOut = v;
	return pOut;
}

void KVec3Normalize(KVec3 *p)
{
	float flen = sqrt(p->x * p->x + p->y *p->y + p->z * p->z);
	p->x /= flen;
	p->y /= flen;
	p->z /= flen;
}

struct _MeshData 
{
	struct Face 
	{
		UINT v[3];
		UINT t[3];
		UINT n[3];
	};
	std::vector<Point3> vertices;
	std::vector<Point3> texcoords;
	std::vector<Point3> normals;
	std::vector<Face> faces;
};


class CTriangleInputProxy :public ITriangleInputProxy
{
public:
	void Init(_MeshData* data)
	{
		m_data = data;
	}

	unsigned int GetTriangleCount() const
	{
		return (unsigned int)m_data->faces.size();
	}

	void GetTriangleIndices( const unsigned int indwTriNo, unsigned int outdwPos[3], unsigned int outdwNorm[3], unsigned int outdwUV[3] ) const
	{
		ASSERT(indwTriNo < GetTriangleCount());
		const _MeshData::Face& face = m_data->faces[indwTriNo];
		for(int i=0;i<3;i++)
		{
			outdwPos[i]=face.v[i];
			outdwUV[i]=face.t[i];
			outdwNorm[i]=face.n[i];
		}
	}

	void GetPos( const unsigned int indwPos, float outfPos[3] ) const
	{
		ASSERT(indwPos<m_data->vertices.size());
		const Point3 &ref=m_data->vertices[indwPos];
		outfPos[0]=ref.x;
		outfPos[1]=ref.y;
		outfPos[2]=ref.z;
	}

	void GetUV( const unsigned int indwPos, float outfUV[2] ) const
	{
		ASSERT(indwPos<m_data->texcoords.size());
		const Point3 &ref=m_data->texcoords[indwPos];
		outfUV[0]=ref.x;
		outfUV[1]=ref.y;
	}

private:
	_MeshData* m_data;
};

struct Vector3Comparer: public std::binary_function<Point3, Point3, bool>
{
	bool operator() (const Point3 &a, const Point3 &b) const
	{
		if (a.x < b.x) return true;
		if (a.x > b.x) return false;

		if (a.y < b.y) return true;
		if (a.y > b.y) return false;

		if (a.z < b.z) return true;
		if (a.z > b.z) return false;

		return false;
	}
};



BOOL ArrangeMeshData(_MeshData& _dst, vector<KPCNT3> &verts, UINT faceCount)
{
	BOOL ret = FALSE;
	std::map<Point3, UINT, Vector3Comparer> map_v, map_t, map_n;
	std::map<Point3, UINT, Vector3Comparer>::iterator iter;
	UINT index;

	_dst.faces.resize(faceCount);
	_dst.vertices.reserve(faceCount * 3);
	_dst.texcoords.reserve(faceCount * 3);
	_dst.normals.reserve(faceCount * 3);

	for (UINT i = 0; i < faceCount; ++i) {
		for (UINT  n = 0; n < 3; ++n) {
			

			int j = n;			

			KVec3 vert_p = verts[i * 3 + j].pos;
			KVec3 vert_n = verts[i * 3 + j].nomral;
			KVec3 vert_t = verts[i * 3 + j].uvw0;

			Point3 pt3Pos(vert_p.x, vert_p.y, vert_p.z);
			Point3 pt3Normal(vert_n.x, vert_n.y, vert_n.z);
			Point3 pt3TextureCoord(vert_t.x, vert_t.y, vert_t.z);

			iter = map_v.find(pt3Pos);
			if (iter == map_v.end()) {
				index = (UINT)_dst.vertices.size();
				_dst.faces[i].v[j] = index;
				_dst.vertices.push_back(pt3Pos);
				map_v.insert(std::make_pair(pt3Pos, index));
			} else {
				_dst.faces[i].v[j] = iter->second;
			}

			iter = map_t.find(pt3TextureCoord);
			if (iter == map_t.end()) {
				index = (UINT)_dst.texcoords.size();
				_dst.faces[i].t[j] = index;
				_dst.texcoords.push_back(pt3TextureCoord);
				map_t.insert(std::make_pair(pt3TextureCoord, index));
			} else {
				_dst.faces[i].t[j] = iter->second;
			}

			iter = map_n.find(pt3Normal);
			if (iter == map_n.end()) {
				index = (UINT)_dst.normals.size();
				_dst.faces[i].n[j] = index;
				_dst.normals.push_back(pt3Normal);
				map_n.insert(std::make_pair(pt3Normal, index));
			} else {
				_dst.faces[i].n[j] = iter->second;
			}
		}
	}

	ret = TRUE;
	return ret;
}

struct CombineIndex 
{
	UINT p;
	UINT t;
	UINT n;
};

void changeYZ(KVec3 &v)
{
	float t = v.z;
	v.z = v.y;
	v.y = t;
}

void changeUV(KVec3 &uvw)
{
	uvw.y = 1.0 - uvw.y;
}

void ComputTangent(vector<KPCNT3> &vertsSrc)
{		
	UINT nFaceNum = (UINT) vertsSrc.size() / 3;

	vector<KPCNT3> verts = vertsSrc;
	if (!ExportOption()->Switch()) // ??
	{
		for(int i = 0; i < nFaceNum; ++i)
		{
			for(int j = 0; j < 3; ++j)
			{
				KPCNT3 &p = verts[i * 3 + j];
				changeYZ(p.pos);
				changeYZ(p.nomral);
			}
		}
	}

	_MeshData meshData;
	CTangentSpaceCalculation<CTriangleInputProxy> tangents;
	CTriangleInputProxy Input;
	
	ArrangeMeshData(meshData, verts, (UINT)nFaceNum);
	CombineIndex ci;

	Input.Init(&meshData);
	tangents.CalculateTangentSpace(Input);
	for (int dwTriNo = 0; dwTriNo < nFaceNum; ++dwTriNo) {
		unsigned int dwBaseIndx[3];
		tangents.GetTriangleBaseIndices(dwTriNo,dwBaseIndx);

		for(DWORD i=0; i<3; ++i) {			
			ci.p = meshData.faces[dwTriNo].v[i];
			ci.t = meshData.faces[dwTriNo].t[i];
			ci.n = dwBaseIndx[i];
			
			Point3 pos = meshData.vertices[ci.p];
			Point3 uvw0 = meshData.texcoords[ci.t];
		
			KVec3 t, b, n;
			tangents.GetBase(ci.n, (float*)&t, (float*)&b, (float*)&n);
			KVec3 v;
			float fDot = KVec3Dot(&n, &t);
			v.x = t.x - n.x * fDot;
			v.y = t.y - n.y * fDot;
			v.z = t.z - n.z * fDot;
			KVec3Normalize(&v);
			KVec4 tangent(v.x, v.y, v.z, 0.0f);
			tangent.w = KVec3Dot(KVec3Cross(&v, &n, &t), &b) < 0 ? -1.0f : 1.0f;
			
			//浮点数可能算爆掉了，这里处理一下
			if(tangent.x != tangent.x) 
			{
				tangent.x = 0;
			}

			if(tangent.y != tangent.y) 
			{
				tangent.y = 0;
			}

			if(tangent.z != tangent.z) 
			{
				tangent.z = 0;
			}

			if(tangent.w != tangent.w) 
			{
				tangent.w = 0;
			}

			vertsSrc[dwTriNo * 3 + i].tangent = tangent;			
		}
	}
}