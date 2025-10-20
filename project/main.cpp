#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <windows.h>

#include "application/CommandLineArgument.h"
#include "application/TextureConverter.h"

int main(int argc,char* argv[]) {
	
	const int kApplicationPath = static_cast<int>(CommandLineArgument::kApplicationPath);
	const int kFilePath = static_cast<int>(CommandLineArgument::kFilePath);
	const int NumArgument = static_cast<int>(CommandLineArgument::NumArgument);

	assert(argc >= NumArgument);

	//COMライブラリ初期化
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	//テクスチャコンバーター生成
	TextureConverter converter;
	//テクスチャ変換
	converter.ConvertTextureWICToDDS(argv[kFilePath]);

	//COMライブラリ終了
	CoUninitialize();

	//終了
	//system("pause");
	return 0;
}