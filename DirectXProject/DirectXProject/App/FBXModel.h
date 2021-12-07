#ifndef __FBX_MODEL_H__
#define __FBX_MODEL_H__

#include <DirectXMath.h>
#include <System/Graphics/DXBuffer.h>
#include <vector>
#include <string>

class FBXModel
{
private:
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
	};
	struct Blend
	{
		// ���b�V���ƃ{�[�������т��邽�߂̃f�[�^
		float weight[4]; // ���ꂼ��̍�����ǂ̒��x�e�����󂯂邩
		unsigned long index[4]; // �ǂ̍��Ɉ����t���ē�����
	};
	/* �X�L�����b�V���A�j���[�V����
	���𓮂��������ʂ����f���̒��_�ɓK�p����
	���̂܂ܓK�p���Ă��܂��ƁA�������_����ړ����Ă������A�\�����悤�Ƃ��Ă��钸�_���ړ����Ă��܂��B
	���̖�����������ɂ́A��x���_�����_�Ɉړ������Ă��獜�̈ړ���K�p����K�v������B
	���̍ہAFBX���f���͈�̃��b�V��(�Ⴆ�Γ�����)���A�j���[�V���������鎞�A
	�e�����󂯂�K�v�Œ���̍��̏�񂵂������Ȃ�(���𓮂����ꍇ�A���̍��̏��͂���Ȃ�)
	���̂��߁A�e���b�V�����Ƃɂǂ̍��̉e�����󂯂�̂����܂Ƃ߂���񂪕K�v�ɂȂ�
	*/
	struct Skin
	{
		std::string bone;
		// ���Ƃ��Ƃ̃��f����T�|�[�Y������Ă���
		// ����T�|�[�Y�̈ړ�����ł������A�V���ɃA�j���[�V������
		// �s���K�p���邱�ƂŁA���_���������A�j���[�V�����̈ʒu�Ɉړ�����
		DirectX::XMMATRIX inverse; // T�|�[�Y�̈ړ���ł������t�s��
	};
	using SkinList = std::vector<Skin>;

	struct Mesh
	{
		DXBuffer buffer;
		void* pVtx;
		SkinList* pSkinList;
	};
	using MeshList = std::vector<Mesh>;

	using DrawSettingCallBack = void(*)(UINT mesh);

public:
	FBXModel();
	~FBXModel();

	HRESULT Load(const char *fileName);

	/*�֐��|�C���^
	   ���s�������֐����ǂ��ɔz�u����Ă��邩�������|�C���^
	   �ϐ��̃|�C���^�Ɨp�r�͕ς��Ȃ�
	   �֐����̑O��*��t����()�ň͂��Ɛ錾�ł���
	   �߂�l�A�������Ăяo�������֐��Ƒ����Ȃ��Ƃ����Ȃ�
	 ���b�V�����ƂɕʁX�̏������Ăяo�������̂ŁA�����Ɋ֐��|�C���^��ݒ� 
	*/
	void Draw(DrawSettingCallBack CallBackFunc = nullptr);

private:
	// �S�Ẵ����������S�ɉ�����邽�߂̊֐�
	void Reset();

private:
	MeshList m_meshList;
};

#endif
