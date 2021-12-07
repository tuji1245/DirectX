//----------
// @file Font.h
// @brief winapi�ŕ����e�N�X�`���쐬
// @author ryota.ssk
//----------
#ifndef __FONT_H__
#define __FONT_H__

#include <System/Graphics/Texture.h>
#include <vector>

class Font
{
public:
	struct Info
	{
		UINT	code;		// �����R�[�h
		POINT	adjust;		// �����ʒu
		SIZE	size;		// �����T�C�Y
		float	uv[2];		// 
	};

public:
	Font();
	~Font();

	//----------
	// 
	// 
	//----------
	bool Create(const char* pText);
	void Release();
	Texture* GetTexture();

	const Info& GetInfo(const char* pChara, BYTE* pReadByte);

	/**
	 * @brief ���݂̃t�H���g���ŕ`��T�C�Y���v�Z
	 * @param[in] text ������
	 * @return �`��T�C�Y
	 */
	SIZE CalcSize(const char *text);

private:



	//Code GetCode(const char *c);
	//int FindInfo(const Code &code);
	//Info CreateInfo(const Code &code);

private:
	UINT m_size;
	UINT m_edge;
	//FontInfoList m_fontList;
	Texture* m_pTexture;
};

#endif // __WIN_FONT_TEXTURE_H__