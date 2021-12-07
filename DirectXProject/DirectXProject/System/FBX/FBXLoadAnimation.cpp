#include "FBXLoadAnimation.h"

namespace ggfbx {

void GetKeyInfos(FbxAnimCurve *pCurve, AnimationInfo::CurveInfo::List &list);
void CalcAnimation(FbxAnimLayer* pLayer, FbxNode* pNode, AnimationInfo &anime, bool isMirror);
void MirrorKeys(AnimationInfo::CurveInfo::List& list);
void Display(const char *pChannel, AnimationInfo::CurveInfo::List &list);

//-----------------------------------------------------
//
/// @brief �f�[�^�N���A
//
//-----------------------------------------------------
void CleanupAnimation(AnimationInfo &anime)
{
	AnimationInfo::List::iterator it = anime.curveList.begin();
	while (it != anime.curveList.end())
	{
		for (int i = 0; i < AnimationInfo::CurveInfo::CHANNEL_MAX; ++i)
		{
			it->keyList[i].clear();
		}
		++it;
	}
}

//-----------------------------------------------------
//
/// @brief �f�[�^�擾
//
//-----------------------------------------------------
void GetAnimation(FbxScene *pScene, AnimationInfo &anime, bool isMirror)
{
	CleanupAnimation(anime);
	int animeStackCount = pScene->GetSrcObjectCount<FbxAnimStack>();
	if (animeStackCount == 0)
	{
		return;
	}
	FbxAnimStack* pStack = pScene->GetSrcObject<FbxAnimStack>(0);
	int animeLayerCount = pStack->GetMemberCount<FbxAnimLayer>();
	if (animeLayerCount == 0)
	{
		return;
	}
	LogOutput("Animation[stack/layer] : %d/%d\n", animeStackCount, animeLayerCount);
	FbxTime time;
	time.SetTime(0, 0, 0, 1, 0, 0, FbxTime::eFrames60);
	anime.totalFrame = static_cast<long>((pStack->GetLocalTimeSpan().GetDuration().Get() + (time.Get() >> 1)) / time.Get());
	CalcAnimation(pStack->GetMember<FbxAnimLayer>(0), pScene->GetRootNode(), anime, isMirror);
}

//-----------------------------------------------------
//
/// @brief �m�[�h���̃A�j���[�V���������o��
//
//-----------------------------------------------------
void CalcAnimation(FbxAnimLayer* pLayer, FbxNode* pNode, AnimationInfo &anime, bool isMirror)
{
	// �e�v�f�̃L�[�𒊏o
	FbxAnimCurve *pCurves[] =
	{
		pNode->LclTranslation.GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_X),
		pNode->LclTranslation.GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_Y),
		pNode->LclTranslation.GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_Z),
		pNode->LclRotation.GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_X),
		pNode->LclRotation.GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_Y),
		pNode->LclRotation.GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_Z),
		pNode->LclScaling.GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_X),
		pNode->LclScaling.GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_Y),
		pNode->LclScaling.GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_Z),
	};
	AnimationInfo::CurveInfo info;
	for (int i = 0; i < AnimationInfo::CurveInfo::CHANNEL_MAX; ++i)
	{
		GetKeyInfos(pCurves[i], info.keyList[i]);
	}
	
	if (isMirror)
	{
		MirrorKeys(info.keyList[AnimationInfo::CurveInfo::TRANSLATION_X]);
		MirrorKeys(info.keyList[AnimationInfo::CurveInfo::ROTATION_Y]);
		MirrorKeys(info.keyList[AnimationInfo::CurveInfo::ROTATION_Z]);
	}
	for (int i = 0; i < AnimationInfo::CurveInfo::CHANNEL_MAX; ++i)
	{
		if (info.keyList[i].size() > 0)
		{
			info.name = pNode->GetInitialName();
			anime.curveList.push_back(info);

			LogOutput("========== AnimationData[No.%d/Name:%s] ==========\n", anime.curveList.size() - 1, info.name.c_str());
			Display("TransX ", info.keyList[AnimationInfo::CurveInfo::TRANSLATION_X]);
			Display("TransY ", info.keyList[AnimationInfo::CurveInfo::TRANSLATION_Y]);
			Display("TransZ ", info.keyList[AnimationInfo::CurveInfo::TRANSLATION_Z]);
			Display("RotX   ", info.keyList[AnimationInfo::CurveInfo::ROTATION_X]);
			Display("RotY   ", info.keyList[AnimationInfo::CurveInfo::ROTATION_Y]);
			Display("RotZ   ", info.keyList[AnimationInfo::CurveInfo::ROTATION_Z]);
			Display("ScaleX ", info.keyList[AnimationInfo::CurveInfo::SCALING_X]);
			Display("ScaleY ", info.keyList[AnimationInfo::CurveInfo::SCALING_Y]);
			Display("ScaleZ ", info.keyList[AnimationInfo::CurveInfo::SCALING_Z]);
			break;
		}
	}

	// �q�v�f����
	for (int i = 0; i < pNode->GetChildCount(); ++i)
	{
		CalcAnimation(pLayer, pNode->GetChild(i), anime, isMirror);
	}
}

//-----------------------------------------------------
//
/// @brief �L�[���擾
//
//-----------------------------------------------------
void GetKeyInfos(FbxAnimCurve *pCurve, AnimationInfo::CurveInfo::List &list)
{
	if (pCurve != NULL)
	{
		FbxTime time;
		time.SetTime(0, 0, 0, 1, 0, 0, FbxTime::eFrames60);

		list.resize(pCurve->KeyGetCount());
		AnimationInfo::CurveInfo::List::iterator it = list.begin();
		while (it != list.end())
		{
			int index = static_cast<int>(it - list.begin());
			it->frame = static_cast<int>((pCurve->KeyGetTime(index).Get() + (time.Get() >> 1)) / time.Get()); // 0.5�����Đ؂�グ
			it->value = pCurve->KeyGetValue(index);
			++it;
		}
	}
}

//-----------------------------------------------------
//
/// @brief �L�[�l�̃~���[�����O
//
//-----------------------------------------------------
void MirrorKeys(AnimationInfo::CurveInfo::List& list)
{
	AnimationInfo::CurveInfo::List::iterator it = list.begin();
	while (it != list.end())
	{
		it->value *= -1.0f;
		++it;
	}
}

//-----------------------------------------------------
//
/// @brief ���O�o��
//
//-----------------------------------------------------
void Display(const char *pChannel, AnimationInfo::CurveInfo::List &list)
{
	LogOutput("%s(KeyCount:%d)[frame,key] : ", pChannel, list.size());
	AnimationInfo::CurveInfo::List::iterator it = list.begin();
	while (it != list.end())
	{
		LogOutput("[%d,%.3f]", it->frame, it->value);
		++it;
	}
	LogOutput("\n");
}

}; // fbx