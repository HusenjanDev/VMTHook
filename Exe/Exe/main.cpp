#include <Windows.h>
#include <stdio.h>

/* our virtual functions that will be hooked by dll. */
class VirtualFunctions
{
public:
	virtual void vFunctionA()
	{
		printf("::vFunctionA()\n");
	}
	virtual void vFunctionB()
	{
		printf("::vFunctionB()\n");
	}
	virtual void vFunctionC()
	{
		printf("::vFunctionC()\n");
	}
};

/* our main. */
int main()
{
	VirtualFunctions* VF = new VirtualFunctions();

	while (true)
	{
		VF->vFunctionA();
		VF->vFunctionB();
		VF->vFunctionC();

		Sleep(500);
		system("CLS");
	}

	return 0;
}