#pragma once
#include <string>
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#include "externals/DirectXTex/DirectXTex.h"
#include "externals/DirectXTex/d3dx12.h"

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

	//str -> wstr変換関数
	static std::wstring ConvertMultiByteStringToWideString(const std::string& str);

	//フォルダパスとファイル名を分離する関数
	void SeparateFilePath(const std::wstring& filePath);

	//DDSテクスチャとしてファイル書き出し
	void SaveDDSTextureToFile();

private:

	DirectX::TexMetadata metadata_; //メタデータ
	DirectX::ScratchImage scratchImage_; //スクラッチイメージ

	std::wstring directoryPath_; //ディレクトリパス
	std::wstring fileName_; //ファイル名
	std::wstring fileExt_; //拡張子
};

