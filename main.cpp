#include <cstdio>
#include <cstdlib>
#include "TextureConverter.h"
#include "Windows.h"
#include <cassert>

enum Argument {
	kApplicationPath,//アプリケーションのパス
	kFilePath,//渡されたファイルのパス

	NumArgument
};

int main(int argc, char* argv[]) {

	assert(argc <= NumArgument);

	//COMライブラリの初期化
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	//テクスチャコンバーター
	TextureConverter converter;

	//テクスチャ変換
	converter.ConvertTectureWiCToDDs(argv[kFilePath]);

	//COMライブラリの終了
	CoUninitialize();

	//OSが勝手に閉じるのを阻止
	system("pause");

	return 0;
}
