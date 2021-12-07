#include "Main.h"
#include "Defines.h"
#include "System/Graphics/DirectX.h"
#include "System/Input.h"
#include "System/Graphics/Geometory.h"
#include <System/Graphics/Shader.h>
#include <App/FBXPlayer.h>
#include <System/FBX/FBXLoader.h>

#include <System/Graphics/Text.h>

// �J�����̕ϊ��s��
struct ViewProj
{
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 proj;
};
// �A�j���ϊ��s��
struct Animation
{
	DirectX::XMFLOAT4X4 bone[250];
};


VertexShader* g_pVS;
VertexShader* g_pVSAnime;
PixelShader* g_pPS;
ConstantBuffer* g_pBuffer;
ConstantBuffer* g_pVP; // ViewProj
ConstantBuffer* g_pAnimeCB;
FBXPlayer* g_pFBXPlayer;
Text* g_pText;

// �v���g�^�C�v�錾
void WriteMeshBoneMatrix(int mesh);

HRESULT Init(HWND hWnd, UINT width, UINT height)
{
	// DirectX
	InitDX(hWnd, width, height, false);
	// Geometory
	InitGeometory();
	// ����
	InitInput();
	// fbxsdk�̏�����
	/* namespace �������O�̕ϐ���֐����ǂ����Ă��錾���Ȃ��Ƃ����Ȃ��A
	�@�܂��͂�����x�@�\���Ƃɂ܂Ƃ߂Ă����������ɁA
	  ��ޕ����E�O���[�s���O���邽�߂Ɏg����
	*/
	ggfbx::Initialize();

	//--- 
	HRESULT hr;
	g_pVS = new VertexShader(VertexShader::VS_NORMAL);
	g_pVSAnime = new VertexShader(VertexShader::VS_ANIMATION);
	g_pPS = new PixelShader();
	g_pBuffer = new ConstantBuffer();
	g_pVP = new ConstantBuffer();
	g_pAnimeCB = new ConstantBuffer();
	g_pFBXPlayer = new FBXPlayer();
	g_pText = new Text;

	hr = g_pVS->Create("Assets/VertexShader.cso");
	hr = g_pVSAnime->Create("Assets/VertexAnimation.cso");
	hr = g_pPS->Create("Assets/PixelShader.cso");
	hr = g_pBuffer->Create(sizeof(DirectX::XMFLOAT4X4));
	hr = g_pVP->Create(sizeof(ViewProj));
	hr = g_pAnimeCB->Create(sizeof(Animation));
	hr = g_pFBXPlayer->Load("Assets/Unitychan/unitychan.fbx");
	g_pFBXPlayer->LoadAnime("Assets/Unitychan/run.fbx");
	g_pFBXPlayer->LoadAnime("Assets/Unitychan/jump.fbx");
	g_pFBXPlayer->PlayAnime(0, true);
	hr = g_pText->Create(2);

	return S_OK;
}
void Uninit()
{
	delete g_pText;
	delete g_pFBXPlayer;
	delete g_pAnimeCB;
	delete g_pVP;
	delete g_pBuffer;
	delete g_pPS;
	delete g_pVSAnime;
	delete g_pVS;
	ggfbx::Terminate();
	UninitInput();
	UninitDX();
}

void Update()
{
	UpdateInput();

	//--- �A�j���[�V�����J��
	if (!g_pFBXPlayer->IsPlayAnime(1))
	{
		if (IsKeyTrigger(VK_SPACE))
		{
			g_pFBXPlayer->PlayBlendAnime(1, 0.3f, false);
		}
	}
	// ��Ұ��݂��Đ����ĂȂ���΁A����A�j���ɑJ��
	if (!g_pFBXPlayer->IsPlayAnime())
	{
		g_pFBXPlayer->PlayBlendAnime(0, 0.3f, true);
	}
	g_pFBXPlayer->Step(1.f);

	// ���[���h�ϊ��s��
	// �V�F�[�_�[�̍s��͏c���сACPU�̍s��͉�����
	// �c���тɕϊ�����K�v������B
	// XMMatrixTranspose ... �s��̓]�u���s��
	DirectX::XMMATRIX world;
	DirectX::XMFLOAT4X4 fWorld;
	static float t = 0;
	world =
		DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f) * 
		DirectX::XMMatrixRotationY(
			DirectX::XMConvertToRadians(t += 1.0f));
	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMStoreFloat4x4(&fWorld, world);
	g_pBuffer->Write(&fWorld);

	// �r���[�v���W�F�N�V����
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX proj;
	ViewProj vp;
	view = DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(2, 0, -2, 0),
		DirectX::XMVectorSet(0, 0, 0, 0),
		DirectX::XMVectorSet(0, 1, 0, 0));
	proj = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XM_PI / 3.f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT,
		0.1f, 1000.f
	);
	view = DirectX::XMMatrixTranspose(view);
	proj = DirectX::XMMatrixTranspose(proj);
	DirectX::XMStoreFloat4x4(&vp.view, view);
	DirectX::XMStoreFloat4x4(&vp.proj, proj);
	g_pVP->Write(&vp);


}

void Draw()
{
	BeginDrawDX();

	// �g�p�������V�F�[�_��ݒ�
	g_pVSAnime->Bind();
	g_pPS->Bind();
	// �g�p����萔�o�b�t�@��ݒ�
	g_pBuffer->Bind(0);
	g_pVP->Bind(1);
	g_pAnimeCB->Bind(2);

	g_pFBXPlayer->Draw(WriteMeshBoneMatrix);

	g_pVS->Bind();
	g_pFBXPlayer->DebugDraw();

	g_pText->Draw("a");

	// ����ȍ~�ʂ̃I�u�W�F�N�g��`�悷��Ƃ��ɁA
	// �V�F�[�_�[��؂�ւ���K�v������Γs�x�K�v��Bind�֐����Ăяo��



	//DrawCube();

	//g_pModel->Draw();

	EndDrawDX();
}


void WriteMeshBoneMatrix(int mesh)
{
	// �A�j���[�V�����̕ϊ��s���ݒ�
	Animation anime;
	DirectX::XMMATRIX bone;
	/* �S�g�̍����ꊇ�Őݒ�
	for (int i = 0; i < 250; ++i)
	{
		anime.bone[i] = g_pFBXPlayer->GetBone(i);
		bone = DirectX::XMLoadFloat4x4(&anime.bone[i]);
		bone = DirectX::XMMatrixTranspose(bone);
		DirectX::XMStoreFloat4x4(&anime.bone[i], bone);
	}
	*/
	// ���b�V�����Ƃɍ���ݒ�
	std::vector<FBXPlayer::MeshInverse>& meshInv = g_pFBXPlayer->GetMeshInverse();
	for (int i = 0; i < meshInv[mesh].num; ++i)
	{
		anime.bone[i] = g_pFBXPlayer->GetBone(
			meshInv[mesh].pList[i].boneIndex);
		bone = DirectX::XMLoadFloat4x4(&anime.bone[i]);
		bone = DirectX::XMMatrixTranspose(
			meshInv[mesh].pList[i].invMat * bone);
		DirectX::XMStoreFloat4x4(&anime.bone[i], bone);
	}
	g_pAnimeCB->Write(&anime);
}

// EOF