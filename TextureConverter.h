#pragma once
#include <string>
#include "DirectXTex/DirectXTex.h"

class TextureConverter
{
public:

	//�e�N�X�`����WIC����DDS�ɕϊ�����
	void ConvertTectureWiCToDDs(const std::string& filePath);

private:

	//�e�N�X�`���t�@�C���ǂݍ���
	void LoadWICTextureFromFile(const std::string& filePath);

	//�}���`�o�C�g����������C�h������ɕϊ�
	static std::wstring ConvertMultiByteStringToWideString(const std::string& mString);

	//�t�H���_�p�X�ƃt�@�C�����𕪗�����
	void SeparateFilePath(const std::wstring& filePath);

	//DDS�e�N�X�`���Ƃ��ăt�@�C�������o��
	void SaveDDSTextureToFile();

private:

	//�摜�̏��
	DirectX::TexMetadata metadata_;
	//�摜�C���[�W�̃R���e�i
	DirectX::ScratchImage scratchImage_;

	//�f�B���N�g���p�X
	std::wstring directoryPath_;
	//�t�@�C����
	std::wstring fileName_;
	//�t�@�C���g���q
	std::wstring fileExt_;

};