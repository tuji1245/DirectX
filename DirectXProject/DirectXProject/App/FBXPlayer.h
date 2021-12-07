#ifndef __FBX_PLAYER_H__
#define __FBX_PLAYER_H__

#include <System/FBX/FBXLoader.h>
#include <DirectXMath.h>
#include <System/Graphics/DXBuffer.h>

class FBXPlayer
{
public:
	struct FBXVertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT3 normal;
		// ���b�V���ƃ{�[�������т��邽�߂̃f�[�^
		float weight[4]; // ���ꂼ��̍�����ǂ̒��x�e�����󂯂邩
		long index[4]; // �ǂ̍��Ɉ����t���ē�����
	};
	/* �X�L�����b�V���A�j���[�V����
	���𓮂��������ʂ����f���̒��_�ɓK�p����
	���̂܂ܓK�p���Ă��܂��ƁA�������_����ړ����Ă������A�\�����悤��
	���Ă��钸�_���ړ����Ă��܂��B���̖�����������ɂ͈�x���_��
	���_�Ɉړ������Ă��獜�̈ړ���K�p����K�v������B
	���̍ہAFBX���f���͈�̃��b�V��(�Ⴆ�Γ�����)���A�j���[�V����������
	���A�e�����󂯂�K�v�Œ���̍��̏�񂵂������Ȃ�(���𓮂�������
	���̍��̏��͂���Ȃ�)
	���̂��߁A�e���b�V�����Ƃɂǂ̍��̉e�����󂯂�̂����܂Ƃ߂����
	�K�v�ɂȂ�
	*/
	struct MeshInverse
	{
		struct List
		{
			int boneIndex;
			// ���Ƃ��Ƃ̃��f����T�|�[�Y������Ă���
			// ����T�|�[�Y�̈ړ�����ł������A�V���ɃA�j���[�V������
			// �s���K�p���邱�ƂŁA���_���������A�j���[�V�����̈ʒu��
			// �ړ�����
			DirectX::XMMATRIX invMat; // T�|�[�Y�̈ړ���ł������t�s��
		};
		List* pList;
		int num;
	};

	struct MatrixParam
	{
		DirectX::XMFLOAT3 translate;
		DirectX::XMFLOAT3 rotation;
		DirectX::XMFLOAT3 scale;
	};
	struct BoneInfo
	{
		DirectX::XMFLOAT4X4 matrix; // ���̎p��
		DirectX::XMMATRIX preRotate; // ���炩���߉�]���Ă����
		MatrixParam param; // �f�t�H���g�̍��̎p��
	};
	struct AnimeInfo
	{
		float playFrame; // ���ݍĐ����̃t���[��
		bool loop; // �J��Ԃ��Đ����邩�ǂ���
		float speed; // �A�j���[�V�����̍Đ��X�s�[�h
		ggfbx::AnimationInfo* pAnime; // �A�j���̏��
	};

public:
	FBXPlayer();
	~FBXPlayer();

	bool Load(const char* fileName);
	// �߂�l�Ŕz��̂ǂ̈ʒu�ɃA�j���[�V�����̏����i�[�������Ԃ�
	// �ǂݍ��߂Ȃ���������-1��Ԃ�
	int LoadAnime(const char* fileName);

	// �A�j���[�V�����̍Đ��w��
	void PlayAnime(int animeNo, bool isLoop = false);
	// �J�ڱ�Ұ��݂̎w��
	void PlayBlendAnime(int blendNo, float time, bool isLoop = false);
	// �A�j���[�V�����Đ�����
	bool IsPlayAnime(int animeNo = -1);

	// �Ԃ������Update
	// �f���Ɠ����C���[�W�ő�����A�R�}����Ȃǂ��l�����Ƃ��ɁA
	// �P�t���[���ł̏����������ł͂Ȃ��̂ŁA���݂̍Đ���߰�ނ�
	// �ǂꂾ���������邩�ōl����������step�Ƃ����֐���
	void Step(float speed = 1.0f);


	/*�֐��|�C���^
	   ���s�������֐����ǂ��ɔz�u����Ă��邩�������|�C���^
	   �ϐ��̃|�C���^�Ɨp�r�͕ς��Ȃ�
	   �֐����̑O��*��t����()�ň͂��Ɛ錾�ł���
	   �߂�l�A�������Ăяo�������֐��Ƒ����Ȃ��Ƃ����Ȃ�
	 �`�悲�ƂɕʁX�̏������Ăяo�������̂ŁA�����Ɋ֐��|�C���^��ݒ� 
	*/
	void Draw(void(*PreCallBack)(int mesh) = nullptr);

	void DebugDraw();


	DirectX::XMFLOAT4X4 GetBone(int index);
	std::vector<MeshInverse>& GetMeshInverse();

private:
	// �S�Ẵ����������S�ɉ�����邽�߂̊֐�
	void Reset();
	// ���f���̍����ǂ�ȃ|�[�Y������Ă��邩�v�Z����֐�
	void CalcBone(int index, DirectX::XMMATRIX parent);
	// ����`�悷��֐�
	void DrawBone(int index, DirectX::XMFLOAT3 parent);
	// �A�j���[�V�����̎��Ԍo�߂��v�Z
	void CalcAnimeFrame(int animeNo, float speed);
	// �A�j���[�V�����̃t���[���ɉ����č��̃|�[�Y���v�Z
	void CalcAnime(int animeNo, MatrixParam* pBones);
	// �A�j���[�V�����̃t���[���ɑΉ������p�����v�Z
	float CalcKey(ggfbx::AnimationInfo::CurveInfo::List& keys, float frame, float defValue);

private:
	// ���f���̕\��
	DXBuffer* m_pBuffers;
	int m_meshNum;
	std::vector<MeshInverse> m_meshInverse;


	//--- ���̏��
	// ����{��{�̈ʒu���]�̏��A
	// �ǂ̍����ǂ��Ɍq�����Ă邩�A���̖��̂��܂܂��
	// �܂��A�ǂ�����v�Z���n�߂�̂��Ƃ����������܂�
	ggfbx::BoneListInfo* m_pFBXBone; // FBX�̍����
	BoneInfo* m_pBones; // �v���O�������s���̍��̏��
	const static int MaxBlend = 2;
	MatrixParam* m_pBlendBones[MaxBlend]; // �J�ڗp�̍����
	//--- �A�j���[�V�����̏��
	const static int MaxAnime = 10;
	AnimeInfo* m_pAnimes[MaxAnime];
	int m_playAnimeNo; // ���ݍĐ����̃A�j���ԍ�

	int m_blendAnimeNo; // �J�ڒ��̃A�j���ԍ�
	float m_blendFrame; // �J�ڎ���
};

#endif
