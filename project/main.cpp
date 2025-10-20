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

	//COM���C�u����������
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	//�e�N�X�`���R���o�[�^�[����
	TextureConverter converter;
	//�e�N�X�`���ϊ�
	converter.ConvertTextureWICToDDS(argv[kFilePath]);

	//COM���C�u�����I��
	CoUninitialize();

	//�I��
	//system("pause");
	return 0;
}