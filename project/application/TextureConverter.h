#pragma once
#include <string>
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#include "externals/DirectXTex/DirectXTex.h"
#include "externals/DirectXTex/d3dx12.h"

//�e�N�X�`���R���o�[�^�[
class TextureConverter {
public:
	TextureConverter() = default;
	~TextureConverter() = default;

	//�e�N�X�`���ϊ�(WIC -> DDS)
	void ConvertTextureWICToDDS(const std::string& filepath);

private:

	//WIC�e�N�X�`���ǂݍ���
	void LoadWICTextureFromFile(const std::string& filepath);

	//str -> wstr�ϊ��֐�
	static std::wstring ConvertMultiByteStringToWideString(const std::string& str);

	//�t�H���_�p�X�ƃt�@�C�����𕪗�����֐�
	void SeparateFilePath(const std::wstring& filePath);

	//DDS�e�N�X�`���Ƃ��ăt�@�C�������o��
	void SaveDDSTextureToFile();

private:

	DirectX::TexMetadata metadata_; //���^�f�[�^
	DirectX::ScratchImage scratchImage_; //�X�N���b�`�C���[�W

	std::wstring directoryPath_; //�f�B���N�g���p�X
	std::wstring fileName_; //�t�@�C����
	std::wstring fileExt_; //�g���q
};

