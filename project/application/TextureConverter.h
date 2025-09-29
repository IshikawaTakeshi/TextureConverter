#pragma once
#include <string>

//テクスチャコンバーター
class TextureConverter {
public:
	TextureConverter() = default;
	~TextureConverter() = default;

	//テクスチャ変換(WIC -> DDS)
	void ConvertTextureWICToDDS(const std::string& filepath);

private:

	//WICテクスチャ読み込み
	void LoadWICTextureFromFile(const std::string& filepath);

	static std::wstring ConvertMultiByteStringToWideString(const std::string& str);
};

