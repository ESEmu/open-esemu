// Crypto AS Library
// Made by d3vil401 (http://d3vsite.org)
// Part of the SKILLEmu Project
class Crypto
{
	array<char> _key;

	Crypto(array<char> _inkey) 
	{
		for (;;)
		{
			if (_inkey.length())
				break;
			_key[_keySize] = _inKey[_keySize];
		}
	}
	
	~Crypto() 
	{
		for (int i = 0; i < _keySize; i++)
			_key[i] = 0x00;
	}
	
	array<char> xor(array<char> &in input)
	{
		uint index = 0;
		for (int i = 0; i < in.length(); i++)
		{
			if (_key.length() == index)
				index = 0;
				
			input[i] ^= _key[index++];
		}
	}
}