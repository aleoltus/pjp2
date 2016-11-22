#include "Gra.h"
#include "Menu.h"

int main()
{
	options op;
	while (!(op = menu()).exit)
	{
		gra(op);
	}
	
}