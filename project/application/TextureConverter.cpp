#include "TextureConverter.h"
#include <windows.h>
#include <cassert>

//==================================================================
// �e�N�X�`���ϊ�(WIC -> DDS)
//==================================================================

void TextureConverter::ConvertTextureWICToDDS(const std::string& filepath) {
	//WIC�e�N�X�`���ǂݍ���
	LoadWICTextureFromFile(filepath);

	//DDS�e�N�X�`���Ƃ��ăt�@�C�������o��
	SaveDDSTextureToFile();
}

//==================================================================
// WIC�e�N�X�`���ǂݍ���
//==================================================================

void TextureConverter::LoadWICTextureFromFile(const std::string& filepath) {

	//�t�@�C���p�X�����C�h������ɕϊ�
	std::wstring wFilePath = ConvertMultiByteStringToWideString(filepath);

	//WIC�e�N�X�`���ǂݍ���
	HRESULT hr = DirectX::LoadFromWICFile(
		wFilePath.c_str(), //�t�@�C���p�X
		DirectX::WIC_FLAGS_NONE, //�t���O
		&metadata_, //���^�f�[�^
		scratchImage_); //�X�N���b�`�C���[�W

	assert(SUCCEEDED(hr));

	SeparateFilePath(wFilePath);
}

//==================================================================
// �}���`�o�C�g����������C�h������ɕϊ�
//==================================================================

std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& str) {
	
	//���C�h������ɕϊ������ۂ̕��������v�Z
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);

	//���C�h������
	std::wstring wString;
	wString.resize(filePathBufferSize);

	//���C�h������ɕϊ�
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &wString[0], filePathBufferSize);

	return wString;
}

void TextureConverter::SeparateFilePath(const std::wstring& filePath) {

	size_t pos1;
	std::wstring exceptExt;

	pos1 = filePath.rfind('.'); //��؂蕶���̌���

	if (pos1 != std::wstring::npos) {
		//��؂蕶���̌����g���q�Ƃ��ĕۑ�
		fileExt_ = filePath.substr(pos1 + 1,filePath.size() - pos1 - 1);
		//��؂蕶���̂̑O�܂ł𔲂��o��
		exceptExt = filePath.substr(0, pos1);
	} else {
		//�g���q�������ꍇ�͋󕶎���ݒ�
		fileExt_ = L"";
		exceptExt = filePath;
	}

	pos1 = exceptExt.rfind('\\'); //��؂蕶���̌���
	if (pos1 != std::wstring::npos) {
		//��؂蕶���̑O�܂ł��f�B���N�g���p�X�Ƃ��ĕۑ�
		directoryPath_ = exceptExt.substr(0, pos1 + 1); 
		//��؂蕶���̌����t�@�C�����Ƃ��ĕۑ�
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	pos1 = exceptExt.rfind('/'); //��؂蕶���̌���
	if (pos1 != std::wstring::npos) {
		//��؂蕶���̑O�܂ł��f�B���N�g���p�X�Ƃ��ĕۑ�
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		//��؂蕶���̌����t�@�C�����Ƃ��ĕۑ�
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	//��؂蕶���������ꍇ�̓f�B���N�g���p�X���󕶎��A�t�@�C������S�ĂƂ��ĕۑ�
	directoryPath_ = L"";
	fileName_ = exceptExt;
}

void TextureConverter::SaveDDSTextureToFile() {
	//�ǂݍ��񂾃e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata_.format = DirectX::MakeSRGB(metadata_.format);

	std::wstring filePath = directoryPath_ + fileName_ + L".dds";

	//DDS�t�@�C�������o��
	HRESULT hr = DirectX::SaveToDDSFile(
		scratchImage_.GetImages(), //�C���[�W�z��
		scratchImage_.GetImageCount(), //�C���[�W��
		metadata_, //���^�f�[�^
		DirectX::DDS_FLAGS_NONE, //�t���O
		filePath.c_str()); //�t�@�C���p�X
	assert(SUCCEEDED(hr));

}