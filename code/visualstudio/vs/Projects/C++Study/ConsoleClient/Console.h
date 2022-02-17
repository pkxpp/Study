#pragma once

class Console
{
public:
	static Console& Instance()
	{
		static Console s_Console;
		return s_Console;
	}

protected:
	Console();
	virtual ~Console();

public:
	/*
	 *	Attention: this function will be redirected stdout!!!
	 */
	int Create();
	void Destroy();

private:
	BOOL m_bAllocConsole;
	FILE* m_pOutput;
};

#define g_GetConsole() (Console::Instance())