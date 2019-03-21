#include <stdio.h>

//Wersja z 8 kolorami
int main()
{
        for(int i = 0; i<8; i++)
        {
                printf("\e[%dmHello World\n", 30+i);
        }
        return 0;
}



/* Wersja z 256 kolorami
int main()
{
	for(int i = 0; i<256; i++)
	{
		printf("\e[38;5;%dmHello World\n", i);
	}
	return 0;
}
*/
