#include "TextureConverter.h"
#include <windows.h>
#include <cassert>

//==================================================================
// テクスチャ変換(WIC -> DDS)
//==================================================================

void TextureConverter::ConvertTextureWICToDDS(const std::string& filepath) {
	//WICテクスチャ読み込み
	LoadWICTextureFromFile(filepath);

	//DDSテクスチャとしてファイル書き出し
	SaveDDSTextureToFile();
}

//==================================================================
// WICテクスチャ読み込み
//==================================================================

void TextureConverter::LoadWICTextureFromFile(const std::string& filepath) {
	HRESULT hr = S_FALSE;
	//ファイルパスをワイド文字列に変換
	std::wstring wFilePath = ConvertMultiByteStringToWideString(filepath);

	//WICテクスチャ読み込み
	hr = DirectX::LoadFromWICFile(
		wFilePath.c_str(), //ファイルパス
		DirectX::WIC_FLAGS_NONE, //フラグ
		&metadata_, //メタデータ
		scratchImage_); //スクラッチイメージ

	assert(SUCCEEDED(hr));

	SeparateFilePath(wFilePath);
}

//==================================================================
// マルチバイト文字列をワイド文字列に変換
//==================================================================

std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& str) {
	
	//ワイド文字列に変換した際の文字数を計算
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);

	//ワイド文字列
	std::wstring wString;
	wString.resize(filePathBufferSize);

	//ワイド文字列に変換
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &wString[0], filePathBufferSize);

	return wString;
}

void TextureConverter::SeparateFilePath(const std::wstring& filePath) {

	size_t pos1;
	std::wstring exceptExt;

	pos1 = filePath.rfind('.'); //区切り文字の検索

	if (pos1 != std::wstring::npos) {
		//区切り文字の後ろを拡張子として保存
		fileExt_ = filePath.substr(pos1 + 1,filePath.size() - pos1 - 1);
		//区切り文字のの前までを抜き出す
		exceptExt = filePath.substr(0, pos1);
	} else {
		//拡張子が無い場合は空文字を設定
		fileExt_ = L"";
		exceptExt = filePath;
	}

	pos1 = exceptExt.rfind('\\'); //区切り文字の検索
	if (pos1 != std::wstring::npos) {
		//区切り文字の前までをディレクトリパスとして保存
		directoryPath_ = exceptExt.substr(0, pos1 + 1); 
		//区切り文字の後ろをファイル名として保存
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	pos1 = exceptExt.rfind('/'); //区切り文字の検索
	if (pos1 != std::wstring::npos) {
		//区切り文字の前までをディレクトリパスとして保存
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		//区切り文字の後ろをファイル名として保存
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	//区切り文字が無い場合はディレクトリパスを空文字、ファイル名を全てとして保存
	directoryPath_ = L"";
	fileName_ = exceptExt;
}

void TextureConverter::SaveDDSTextureToFile() {

	HRESULT hr = S_FALSE;

	//ミップマップの作成
	DirectX::ScratchImage mipImages{};

	//圧縮フォーマットかどうかの判定
	if (DirectX::IsCompressed(scratchImage_.GetMetadata().format)) {
		mipImages = std::move(scratchImage_); //圧縮フォーマットの場合はそのまま使う

	} else if (scratchImage_.GetMetadata().width == 1 && scratchImage_.GetMetadata().height == 1) {
		mipImages = std::move(scratchImage_); // 1x1はミップマップ不要

	} else { //非圧縮フォーマットの場合はミップマップを作成

		hr = DirectX::GenerateMipMaps(
			scratchImage_.GetImages(),
			scratchImage_.GetImageCount(),
			scratchImage_.GetMetadata(),
			DirectX::TEX_FILTER_SRGB,
			0, mipImages);

		if(SUCCEEDED(hr)) {
			scratchImage_ = std::move(mipImages);
			metadata_ = scratchImage_.GetMetadata();
		}
	}

	//圧縮形式に変換
	DirectX::ScratchImage convertedImage{};
	hr = DirectX::Compress(
		scratchImage_.GetImages(),
		scratchImage_.GetImageCount(),
		metadata_,
		DXGI_FORMAT_BC7_UNORM, //圧縮フォーマット
		DirectX::TEX_COMPRESS_BC7_QUICK | DirectX::TEX_COMPRESS_SRGB_OUT | DirectX::TEX_COMPRESS_PARALLEL, //圧縮フラグ
		1.0f,
		convertedImage);

	if (SUCCEEDED(hr)) {
		scratchImage_ = std::move(convertedImage);
		metadata_ = scratchImage_.GetMetadata();
	}

	//読み込んだテクスチャをSRGBとして扱う
	metadata_.format = DirectX::MakeSRGB(metadata_.format);

	std::wstring filePath = directoryPath_ + fileName_ + L".dds";

	//DDSファイル書き出し
	hr = DirectX::SaveToDDSFile(
		scratchImage_.GetImages(), //イメージ配列
		scratchImage_.GetImageCount(), //イメージ数
		metadata_, //メタデータ
		DirectX::DDS_FLAGS_NONE, //フラグ
		filePath.c_str()); //ファイルパス
	assert(SUCCEEDED(hr));

	
}