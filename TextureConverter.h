#pragma once
#include <string>

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

};