/**
 * @file Font.cpp
 * @brief winapi�Ńt�H���g�쐬�Asdf�Ƃ��ăe�N�X�`����������
 * @author ryota.ssk
 */
#include <System/Graphics/Font.h>

Font::Font()
{
}
Font::~Font()
{
}
Texture* Font::GetTexture()
{
	return m_pTexture;
}

const Font::Info& Font::GetInfo(const char* pChara, BYTE* pReadByte)
{
	// �擪�����̃e�N�X�`���擾
	//Code code = GetCode(pChara);
	Info info;
	/*
	int texIdx = FindInfo(code);
	if (texIdx != -1) {
		info = m_fontList[texIdx];
	}
	else {
		info = CreateInfo(code);
		for (texIdx = 0; texIdx < m_fontList.size(); ++texIdx) {
			if (m_fontList[texIdx].code == 0) {
				break;
			}
		}
		if (texIdx == m_fontList.size()) {
			--texIdx;
			TEXTURE->Unregister(m_fontList[texIdx].pTexture);
		}
	}

	// �e�N�X�`���L���b�V���X�V
	for (int i = texIdx; i > 0; --i) {
		m_fontList[i] = m_fontList[i - 1];
	}
	m_fontList[0] = info;

	// �ǂݍ��ݏ���
	if (pReadByte != NULL) {
		*pReadByte = code.len;
	}*/

	return info;//m_fontList[0];
}

/**
 * @brief ���݂̃t�H���g���ŕ`��T�C�Y���v�Z
 * @param[in] text ������
 * @return �`��T�C�Y
 */
SIZE Font::CalcSize(const char *text)
{
/*
	SIZE size = { 0, static_cast<LONG>(m_size) };
	UINT idx = 0;
	LONG w = 0;
	while (idx < strlen(text)) {
		Code code = GetCode(&text[idx]);
		if (code.code == '\n') {
			size.cx = max(w, size.cx);
			size.cy += m_size;
			w = 0;
		}
		else {
			w += (m_size * code.len + 1) / 2;
		}
		idx += code.len;
	}
	size.cx = max(w, size.cx);
	return size;*/
	return SIZE();
}

/**
 * @brief �������擾
 */
 /*
Font::Code Font::GetCode(const char *c)
{
	Code code;
	code.c = c;
	code.len = IsDBCSLeadByte(c[0]) ? 2 : 1;
	if (code.len == 2) {
		code.code = (unsigned char)c[0] << 8 | (unsigned char)c[1];
	}
	else {
		code.code = (unsigned char)c[0];
	}
	return code;
}

/**
 * @brief �e�N�X�`������
 */
/*
int Font::FindInfo(const Code &code)
{
	FontInfoList::iterator it = m_fontList.begin();
	while (it != m_fontList.end()) {
		if (it->code == code.code) {
			return static_cast<int>(it - m_fontList.begin());
		}
		++it;
	}
	return -1;
}

bool Font::Create(const char* pText)
{
	// ���������J�E���g


	// �e�N�X�`���쐬
	return true;
}
Font::Info Font::CreateInfo(const Code &code)
{
	Info info = {
		code.code, {0, 0}, {0, static_cast<LONG>(m_size)}, NULL
	};

	const char *faceName = "MS�S�V�b�N";
	UINT penSize = m_edge;

	// �f�o�C�X�R���e�L�X�g�쐬
	HDC hDC = GetDC(NULL);
	HDC memDC = CreateCompatibleDC(hDC);
	ReleaseDC(NULL, hDC);

	// �t�H���g�쐬
	LOGFONTA lf = {};
	lf.lfHeight = m_size;
	lf.lfCharSet = SHIFTJIS_CHARSET;
	lf.lfWeight = 0;
	lf.lfQuality = ANTIALIASED_QUALITY;
	memcpy(lf.lfFaceName, faceName, strlen(faceName));
	HFONT hFont = CreateFontIndirectA(&lf);
	HFONT oldFont = (HFONT)SelectObject(memDC, hFont);

	// �t�H���g�T�C�Y�̌v�Z
	TEXTMETRICA tm;
	GLYPHMETRICS gm;
	MAT2 mat = { {0,1}, {0,0}, {0,0}, {0,1} };
	GetTextMetricsA(memDC, &tm);
	GetGlyphOutlineA(memDC, code.code, GGO_METRICS, &gm, 0, 0, &mat);

	const LONG penHalfSize = penSize / 2;
	RECT charRegion = {
		gm.gmptGlyphOrigin.x - penHalfSize,
		tm.tmAscent - gm.gmptGlyphOrigin.y - penHalfSize,
		gm.gmptGlyphOrigin.x + static_cast<LONG>(gm.gmBlackBoxX) + penHalfSize,
		tm.tmAscent - gm.gmptGlyphOrigin.y + static_cast<LONG>(gm.gmBlackBoxY) + penHalfSize
	};
	info.size.cx = gm.gmBlackBoxX + penSize;
	info.size.cy = gm.gmBlackBoxY + penSize;
	info.adjust.x = charRegion.left;
	info.adjust.y = charRegion.top;

	// api�������ݐ�̗̈���쐬
	BITMAPINFO bmpInfo = {};
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = info.size.cx;
	bmpInfo.bmiHeader.biHeight = -info.size.cy;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 24;
	BYTE *pBit = 0;
	HBITMAP hBitMap = CreateDIBSection(0, &bmpInfo, DIB_RGB_COLORS, (void**)&pBit, 0, 0);
	HBITMAP oldBMP = (HBITMAP)SelectObject(memDC, hBitMap);

	// �w�i��F�ŏ�����
	RECT charWH = { 0, 0, info.size.cx, info.size.cy };
	HBRUSH bgBrush = (HBRUSH)CreateSolidBrush(RGB(0, 0, 255));
	FillRect(memDC, &charWH, bgBrush);
	DeleteObject(bgBrush);

	// �����`��(�p�X�͗΁A�h��͐�
	HPEN   hPen = (HPEN)CreatePen(PS_SOLID, penSize, RGB(0, 255, 0));
	HBRUSH hBrush = (HBRUSH)CreateSolidBrush(RGB(255, 0, 0));
	HPEN   oldPen = (HPEN)SelectObject(memDC, hPen);
	HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
	SetBkMode(memDC, TRANSPARENT);
	BeginPath(memDC);
	TextOutA(memDC, -charRegion.left, -charRegion.top, code.c, code.len);
	EndPath(memDC);
	StrokeAndFillPath(memDC);

	// �쐬�����I�u�W�F�N�g�폜
	SelectObject(memDC, oldPen);
	SelectObject(memDC, oldBrush);
	SelectObject(memDC, oldFont);
	SelectObject(memDC, oldBMP);
	DeleteObject(hBrush);
	DeleteObject(hPen);
	DeleteObject(hFont);


	// BMP�f�[�^���e�N�X�`���f�[�^�ɕϊ�
	/*
	UINT texW = info.size.cx;
	UINT texH = info.size.cy;
	BYTE_COLOR *pTex = NEW BYTE_COLOR[texW * texH];
	const UINT PixelSize = 3;
	const UINT Align = 4;
	UINT BMPPitch = (texW * PixelSize + (Align - 1)) / Align * Align;

	for (UINT y = 0; y < texH; ++y) {
		for (UINT x = 0; x < texW; ++x) {
			int texIdx = texW * y + x;
			int bitIdx = BMPPitch * y + PixelSize * x;
			if (pBit[bitIdx + 0] == 255) {
				// ����
				pTex[texIdx].a = 0;
			}
			else {
				pTex[texIdx].a = 255;
				if (pBit[bitIdx + 1] == 255 && m_edge != 0) {
					// ���������
					pTex[texIdx].r = pTex[texIdx].g = pTex[texIdx].b = 0;
				}
				else if (pBit[bitIdx + 2] == 255) {
					// �����h��Ԃ�
					pTex[texIdx].r = pTex[texIdx].g = pTex[texIdx].b = 255;
				}
				else {
					// ����
					pTex[texIdx].a = 0;
				}
			}
		}
	}

	// �e�N�X�`���쐬
	GraphicsTextureGenerate* pTexture = NEW GraphicsTextureGenerate();
	pTexture->Create(texW, texH, pTex);
	char codeKey[20];
	_itoa_s(info.code, codeKey, sizeof(codeKey), 10);
	GraphicsTexture *pRegTex = TEXTURE->Register(codeKey, pTexture);
	if (pRegTex != pTexture)
	{
		SAFE_DELETE(pTexture);
		pTexture = dynamic_cast<GraphicsTextureGenerate*>(pRegTex);
	}
	info.pTexture = pTexture;
	SAFE_DELETE_ARRAY(pTex);

	DeleteObject(hBitMap);
	DeleteDC(memDC);

	return info;
}*/