#include "RedEclipseDummper.h"

int main()
{
	RedEclipseDummper d("redeclipse.exe");
	d.Parse();
	d.DumpOffsets();
	LogFinal();
}