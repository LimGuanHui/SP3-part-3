#include "MapLoad.h"

namespace MAPLOADER
{

	MapLoad::MapLoad(void)
		: theScreen_Height(0)
		, theScreen_Width(0)
		, theNumOfTiles_Height(0)
		, theNumOfTiles_Width(0)
		, theMap_Height(0)
		, theMap_Width(0)
		, theNumOfTiles_MapHeight(0)
		, theNumOfTiles_MapWidth(0)
		, theTileSize(0)
	{
		theScreenMap.clear();
	}

	MapLoad::~MapLoad(void)
	{
		theScreenMap.clear();
	}

	void MapLoad::Init(const int theScreen_Height, const int theScreen_Width,
		const int theNumOfTiles_Height, const int theNumOfTiles_Width,
		const int theMap_Height, const int theMap_Width,
		int theTileSize)
	{
		this->theScreen_Height = theScreen_Height;
		this->theScreen_Width = theScreen_Width;
		this->theNumOfTiles_Height = theNumOfTiles_Height;
		this->theNumOfTiles_Width = theNumOfTiles_Width;
		this->theMap_Height = theMap_Height;
		this->theMap_Width = theMap_Width;
		this->theTileSize = theTileSize;

		theNumOfTiles_MapHeight = (int)(theMap_Height / theTileSize);
		theNumOfTiles_MapWidth = (int)(theMap_Width / theTileSize);

		theScreenMap.resize(theNumOfTiles_MapHeight);
		for (int i = 0; i < theNumOfTiles_MapHeight; ++i)
			theScreenMap[i].resize(theNumOfTiles_MapWidth);
	}

	bool MapLoad::LoadMap(const string mapName)
	{
		if (LoadFile(mapName) == true)
		{
			printf("Map (%s) has been successfully loaded!\n", mapName.c_str());
			return true;
		}

		return false;
	}

	bool MapLoad::LoadFile(const string mapName)
	{
		int theLineCounter = 0;
		int theMaxNumOfColumns = 0;

		theScreenMap.clear();


		theScreenMap.resize(theNumOfTiles_MapHeight);
		for (int i = 0; i < theNumOfTiles_MapHeight; ++i)
			theScreenMap[i].resize(theNumOfTiles_MapWidth);

		ifstream file(mapName.c_str());
		if (file.is_open())
		{
			int i = 0, k = 0;
			while (file.good())
			{
				string aLineOfText = "";
				getline(file, aLineOfText);

				if (theLineCounter >= theNumOfTiles_MapHeight)
					break;

				// If this line is not a comment line, then process it
				if (!(aLineOfText.find("//*") == NULL) && aLineOfText != "")
				{
					if (theLineCounter == 0)
					{
						 //This is the first line of the map data file
						string token;
						istringstream iss(aLineOfText);
						while (getline(iss, token, ','))
						{
							 //Count the number of columns
							theMaxNumOfColumns = atoi(token.c_str());
						}
						if (theMaxNumOfColumns != theNumOfTiles_MapWidth)
							return false;
					}
					else
					{
						int theColumnCounter = 0;

						string token;
						istringstream iss(aLineOfText);
						while (getline(iss, token, ',') && (theColumnCounter<theNumOfTiles_MapWidth))
						{
							theScreenMap[theLineCounter][theColumnCounter++] = atoi(token.c_str());
						}
					}
				}

				theLineCounter++;
			}
		}
		return true;
	}

	int MapLoad::GetNumOfTiles_Height(void)
	{
		return theNumOfTiles_Height;
	}

	int MapLoad::GetNumOfTiles_Width(void)
	{
		return theNumOfTiles_Width;
	}

	int MapLoad::GetTileSize(void)
	{
		return theTileSize;
	}

	 //Get the number of tiles for height of the map
	int MapLoad::getNumOfTiles_MapHeight(void)
	{
		return theNumOfTiles_MapHeight;
	}

	 //Get the number of tiles for width of the map
	int MapLoad::getNumOfTiles_MapWidth(void)
	{
		return theNumOfTiles_MapWidth;
	}

	MapLoad* LoadMap()
	{
		return new MapLoad();
	}
}

