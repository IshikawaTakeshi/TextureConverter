#pragma once
#include <string>

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

	static std::wstring ConvertMultiByteStringToWideString(const std::string& str);
};

