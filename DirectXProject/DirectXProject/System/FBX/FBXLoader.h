#ifndef __FBX_LOADER_H__
#define __FBX_LOADER_H__

#include <fbxsdk.h>
#include <vector>
#include <string>

#pragma comment(lib, "libfbxsdk-mt.lib")
#pragma comment(lib, "libxml2-mt.lib")
#pragma comment(lib, "zlib-mt.lib")

namespace ggfbx
{

//----- ��`
// 
struct Vector2
{
	float x, y;
};
struct Vector3
{
	float x, y, z;
};
union Matrix
{
	struct
	{
		float _11, _12, _13, _14;
		float _21, _22, _23, _24;
		float _31, _32, _33, _34;
		float _41, _42, _43, _44;
	};
	float m[4][4];
	float a[16];
};
struct Color
{
	float r, g, b, a;
};


// ��]����
enum RotateOrder
{
	ROTATE_XYZ,
	ROTATE_ZXY,
};
// �}�b�s���O���
enum Mapping
{
	MAPPING_VERTEX,
	MAPPING_INDEX,
};


// ���b�V�����
struct MeshInfo
{
	// �@�����
	struct NormalInfo
	{
		typedef std::vector<Vector3> List;
		Mapping	mapping;
		List	list;
	};
	// UV���
	struct UVInfo
	{
		typedef std::vector<Vector2> List;
		Mapping	mapping;
		List	list;
	};
	typedef std::vector<UVInfo> UVList;
	// �X�L��
	struct SkinInfo
	{
		struct WeightInfo
		{
			struct WeightValue
			{
				long index;
				float weight;
			};
			static const long WEIGHT_NUM_MAX = 4;
			WeightValue value[WEIGHT_NUM_MAX];
		};
		struct BoneInfo
		{
			std::string name;
			float offset[4][4];
		};
		typedef std::vector<WeightInfo> WeightList;
		typedef std::vector<BoneInfo> BoneList;

		WeightList		weightList;
		BoneList		boneList;
	};

	typedef std::vector<long> IndexList;
	typedef std::vector<Vector3> VertexList;
	typedef std::string MaterialName;

	IndexList		indexList;		///< �C���f�b�N�X���
	VertexList		vertexList;		///< ���_���
	NormalInfo		normalInfo;		///< �@�����
	UVList			uvList;			///< UV���
	MaterialName	materialName;	///< �}�e���A����(�}�e���A���ŕ������Ă�̂ň��
	SkinInfo		skinInfo;		///< �X�L�����
	Matrix			transform;		///< �p�����
};
typedef std::vector<MeshInfo> MeshList;

// �{�[�����
struct BoneInfo
{
	typedef std::vector<long> ChildList;

	std::string		name;		///< �{�[����
	Vector3			translate;	///< �ړ��l
	Vector3			rotation;	///< ��]�l
	Vector3			scale;		///< �g��k��
	Vector3			preRotate;	///< ���O��]
	RotateOrder		order;		///< ��]����
	ChildList		children;	///< �q�v�f

};
typedef std::vector<BoneInfo> BoneList;

// �{�[���ꗗ
struct BoneListInfo
{
	int root;
	BoneList list;
};

// �}�e���A�����
struct MaterialInfo
{
	struct Lambert
	{
		Color	ambient;
		Color	diffuse;
		Color	emissive;
		Vector3	bump;
		float	transparency;
	};
	struct Phong
	{
		Color	specular;
		float	shininess;
		float	reflectivity;
	};
	typedef std::vector<std::string> TextureList;

	std::string name;			///< �}�e���A����
	TextureList textureList;	///< �e�N�X�`����
	Lambert		*pLambert;		///< �����o�[�g���
	Phong		*pPhong;		///< �t�H�����
};
typedef std::vector<MaterialInfo> MaterialList;

// �A�j���[�V�������
struct AnimationInfo
{
	struct CurveInfo
	{
		enum Channels
		{
			TRANSLATION_X,
			TRANSLATION_Y,
			TRANSLATION_Z,
			ROTATION_X,
			ROTATION_Y,
			ROTATION_Z,
			SCALING_X,
			SCALING_Y,
			SCALING_Z,
			CHANNEL_MAX
		};
		struct KeyInfo
		{
			long	frame;
			float	value;
		};
		typedef std::vector<KeyInfo> List;

		std::string		name;					///< �Ή�����{�[����
		List			keyList[CHANNEL_MAX];	///< �`�����l�����Ƃ̃L�[���
	};
	typedef std::vector<CurveInfo> List;

	long		totalFrame;		///< �A�j���[�V��������
	List		curveList;		///< �A�j�����
};

/// ���O�o�͊֐��^��`
typedef void(*LogCallbackFunc)(const char *);


/// @brief ������
void Initialize();

/// @brief �I������
void Terminate();

/// @brief FBX�ǂݍ���
/// @param [in] fileName �t�@�C����
/// @param [out] meshList ���b�V���ǂݍ��ݐ�
/// @param [out] boneList �{�[���ǂݍ��ݐ�
/// @param [out] materiaList �}�e���A���ǂݍ��ݐ�
/// @param [out] anime �A�j���[�V�����ǂݍ��ݐ�
/// @param [in] isMirror ���f�����]
/// @return �ǂݍ��݌���
bool Load(const char* pFileName, MeshList* pMeshList, BoneListInfo* pBoneListInfo, MaterialList* pMaterialList, AnimationInfo* pAnime, bool isMirror = false);

/// @brief �������
/// @param [out] meshList ���b�V��
/// @param [out] boneList �{�[��
/// @param [out] materiaList �}�e���A��
/// @param [out] anime �A�j���[�V����
void Cleanup(MeshList* pMeshList, BoneListInfo* pBoneListInfo, MaterialList* pMaterialList, AnimationInfo* pAnime);


/// @brief ���O�����֐��̐ݒ�
/// @param [in] func �R�[���o�b�N�֐�
void SetLogCallbackFunc(LogCallbackFunc func);

/// @brief ���O�o��
/// @param[in] format �o�̓t�H�[�}�b�g
/// @param[in] ... �ό���
void LogOutput(const char *format, ...);

}; // fbx

#endif // __FBX_LOADER_H__