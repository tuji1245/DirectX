#include "FBXLoader.h"
#include "FBXLoadMesh.h"
#include "FBXLoadBone.h"
#include "FBXLoadMaterial.h"
#include "FBXLoadAnimation.h"

namespace ggfbx
{

FbxManager *g_pManager = NULL;
LogCallbackFunc g_LogFunc = NULL;

//-----------------------------------------------------
//
/// @brief ������
//
//-----------------------------------------------------
void Initialize()
{
	g_pManager = FbxManager::Create();

	FbxIOSettings *pIO = FbxIOSettings::Create(g_pManager, IOSROOT);
	g_pManager->SetIOSettings(pIO);
	LogOutput("[FBXLoadInitialize]\nstart\n");
}

//-----------------------------------------------------
//
/// @brief �I������
//
//-----------------------------------------------------
void Terminate()
{
	g_pManager->Destroy();
	LogOutput("[FBXLoadInitialize]\nend\n");
}

//-----------------------------------------------------
//
/// @brief FBX�ǂݍ���
/// @param [in] fileName �t�@�C����
/// @param [out] meshList ���b�V���ǂݍ��ݐ�
/// @param [out] boneListInfo �{�[���ǂݍ��ݐ�
/// @param [out] materialList �}�e���A���ǂݍ��ݐ�
/// @param [out] anime �A�j���[�V�����ǂݍ��ݐ�
/// @param [in] isMirror ���f�����]
/// @return �ǂݍ��݌���
//
//-----------------------------------------------------
bool Load(const char* pFileName, MeshList* pMeshList, BoneListInfo* pBoneListInfo, MaterialList* pMaterialList, AnimationInfo* pAnime, bool isMirror)
{
	LogOutput("[FBXLoad]\n");
	LogOutput("FileName: %s\n", pFileName);

	// �ǂݍ��ݏ�����
	FbxImporter *pImporter = FbxImporter::Create(g_pManager, "");
	if (!pImporter->Initialize(pFileName, -1, g_pManager->GetIOSettings()))
	{
		LogOutput("\nfailed...\n");
		return false;
	}

	// �ǂݍ���
	FbxScene *pScene = FbxScene::Create(g_pManager, "originalScene");
	pImporter->Import(pScene);
	// �O�p��
	FbxGeometryConverter geometryConverter(g_pManager);
	geometryConverter.Triangulate(pScene, true);
	geometryConverter.RemoveBadPolygonsFromMeshes(pScene);
	// �}�e���A���ŕ���
	geometryConverter.SplitMeshesPerMaterial(pScene, true);

	// ���b�V���f�[�^�擾
	if (pMeshList != nullptr)
	{
		GetMesh(pScene, *pMeshList, isMirror);
	}

	// �}�e���A���f�[�^�擾
	if (pMaterialList != nullptr)
	{
		GetMaterial(pScene, *pMaterialList);
	}

	// �{�[���f�[�^�擾
	if (pBoneListInfo != nullptr)
	{
		GetBone(pScene, *pBoneListInfo, isMirror);
	}

	// �A�j���[�V�����f�[�^�擾
	if (pAnime != nullptr)
	{
		GetAnimation(pScene, *pAnime, isMirror);
	}

	pImporter->Destroy();
	LogOutput("\nsucsess!\n");
	return true;
}

//-----------------------------------------------------
//
/// @brief �������
/// @param [out] meshList ���b�V��
/// @param [out] boneList �{�[��
/// @param [out] materiaList �}�e���A��
/// @param [out] anime �A�j���[�V����
//
//-----------------------------------------------------
void Cleanup(MeshList* pMeshList, BoneListInfo* pBoneListInfo, MaterialList* pMaterialList, AnimationInfo* pAnime)
{
	if (pMeshList != nullptr) { CleanupMesh(*pMeshList); }
	if (pBoneListInfo != nullptr) { CleanupBone(*pBoneListInfo); }
	if (pMaterialList != nullptr) { CleanupMaterial(*pMaterialList); }
	if (pAnime != nullptr) { CleanupAnimation(*pAnime); }
}

//-----------------------------------------------------
//
/// @brief ���O�����֐��̐ݒ�
/// @param [in] func �R�[���o�b�N�֐�
//
//-----------------------------------------------------
void SetLogCallbackFunc(LogCallbackFunc func)
{
	g_LogFunc = func;
}

//-----------------------------------------------------
//
/// @brief ���O�o��
/// @param[in] format �o�̓t�H�[�}�b�g
/// @param[in] ... �ό���
//
//-----------------------------------------------------
void LogOutput(const char *format, ...)
{
	if (g_LogFunc != NULL){
		va_list args;
		char buf[1024];
		va_start(args, format);
		vsprintf_s(buf, sizeof(buf), format, args);
		g_LogFunc(buf);
		va_end(args);
	}
}

}; // fbx

// EOF