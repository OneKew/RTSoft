#include <iostream>
#include <ctime>
using namespace std;
int maxS = 0, tmp = 0, H, W;
bool** Map;
void find_area(int i, int j)
{
	tmp += 1;
	Map[i][j] = 0;
	if (i > 0)   if (Map[i - 1][j]) find_area(i - 1, j);
	if (i < H-1) if (Map[i + 1][j]) find_area(i + 1, j);
	if (j > 0)   if (Map[i][j - 1]) find_area(i, j - 1);
	if (j < W-1) if (Map[i][j + 1]) find_area(i, j + 1);
}
int main()
{

	//---------------------------------Auto generated Array Input------------------------------------
	cout << "Input Map's scale (Height and Width)" << endl;
	cin >> H >> W;
	srand(time(0));
	Map = new bool* [H];
	for (int i = 0; i < H; i++)
	{
		Map[i] = new bool[W];
	}
	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
		{
			Map[i][j] = rand() % 2;
		}

	//----------------------------------Showing the Array-----------------------------------------
	cout << "Random Generated Map" << endl;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++)
			cout << Map[i][j] << " ";
		cout << endl;
	}

	//-----------------------------------Finding the largest island--------------------------------
	for (int i = 0; i < H; i++) 
		for (int j = 0; j < W; j++)
		{
			if (Map[i][j]) find_area(i, j);
			if (tmp > maxS) maxS = tmp;
			tmp = 0;
		}

	//-----------------------------------Max Square output-----------------------------------------
	cout << "Max island square on the map: " << maxS << endl;

	//-----------------------------------Array deleting----------------------------------
	for (int i = 0; i < H; ++i)
		delete[] Map[i];
	delete[] Map;

	return 0;
}

