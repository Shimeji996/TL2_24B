#include "TextureConverter.h"
#include <Windows.h>


using namespace DirectX;

void TextureConverter::ConvertTectureWiCToDDs(const std::string& filePath) {

	//テクスチャファイルを読み込む
	LoadWICTextureFromFile(filePath);

	//DDS形式に変換して書き出す
	SaveDDSTextureToFile();

}

void TextureConverter::LoadWICTextureFromFile(const std::string& filePath) {

	//ファイルパスをワイド文字列に変換
	std::wstring wFilePath = ConvertMultiByteStringToWideString(filePath);

	//テクスチャ読み込み
	HRESULT result = LoadFromWICFile(wFilePath.c_str(), WIC_FLAGS_NONE, &metadata_, scratchImage_);
	assert(SUCCEEDED(result));

	//フォルダパスとファイル名を分離する
	SeparateFilePath(wFilePath);

}

std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& mString) {

	//ワイド文字列に変換した際の文字数を計算
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, nullptr, 0);

	//ワイド文字列
	std::wstring wString;
	wString.resize(filePathBufferSize);

	//ワイド文字列に変換
	MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, &wString[0], filePathBufferSize);

	return wString;
}

void TextureConverter::SeparateFilePath(const std::wstring& filePath) {

	size_t pos1;
	std::wstring exceptExt;

	//区切り文字'.'が出てくる一番最後の部分を検索
	pos1 = filePath.rfind('.');
	//検索がヒットしたら
	if (pos1 != std::wstring::npos) {
		//区切り文字の後ろをファイル拡張子として保存
		fileExt_ = filePath.substr(pos1 + 1, filePath.size() - pos1 - 1);
		//区切り文字の前までを抜き出す
		exceptExt = filePath.substr(0, pos1);
	}
	else {
		fileExt_ = L"";
		exceptExt = filePath;
	}

	//区切り文字'\\'が出てくる一番最後の部分を検索
	pos1 = exceptExt.rfind('\\');
	//検索がヒットしたら
	if (pos1 != std::wstring::npos) {
		//区切り文字の前までをディレクトリパスとして保存
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		//区切り文字の後ろをファイル名として保存
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	//区切り文字'/'が出てくる一番最後の部分を検索
	pos1 = exceptExt.rfind('/');
	//検索がヒットしたら
	if (pos1 != std::wstring::npos) {
		//区切り文字の前までをディレクトリパスとして保存
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		//区切り文字の後ろをファイル名として保存
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	//区切り文字ガないのでファイル名のみとして扱う
	directoryPath_ = L"";
	fileName_ = exceptExt;

}

void TextureConverter::SaveDDSTextureToFile() {

	HRESULT result;
	ScratchImage mipChain;
	//ミップマップ生成
	result = GenerateMipMaps(scratchImage_.GetImages(), scratchImage_.GetImageCount(), scratchImage_.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		//イメージとメタデータをミニマップ数で置き換える
		scratchImage_ = std::move(mipChain);
		metadata_ = scratchImage_.GetMetadata();
	}

	//圧縮形式に変換
	ScratchImage converted;
	result = Compress(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metadata_, DXGI_FORMAT_BC7_UNORM_SRGB, TEX_COMPRESS_BC7_QUICK | TEX_COMPRESS_SRGB_OUT | TEX_COMPRESS_PARALLEL, 1.0f, converted);
	if (SUCCEEDED(result)) {
		scratchImage_ = std::move(converted);
		metadata_ = scratchImage_.GetMetadata();
	}

	//読み込んだテクスチャをSRGBとして扱う
	metadata_.format = MakeSRGB(metadata_.format);

	//出力ファイル名を設定する
	std::wstring filePath = directoryPath_ + fileName_ + L".dds";

	//DDSファイル書き出し
	result = SaveToDDSFile(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metadata_, DDS_FLAGS_NONE, filePath.c_str());
	assert(SUCCEEDED(result));

}
