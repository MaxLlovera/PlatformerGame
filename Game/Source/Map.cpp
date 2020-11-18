
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>

Map::Map() : Module(), mapLoaded(false)
{
	name.Create("map");
}

// Destructor
Map::~Map()
{
}

// L06: DONE 7: Ask for the value of a custom property
int Properties::GetProperty(const char* value, int defaultValue) const
{
	ListItem<Property*>* item = list.start;

	while (item)
	{
		if (item->data->name == value)
			return item->data->value;
		item = item->next;
	}

	return defaultValue;
}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.Create(config.child("folder").child_value());

	return ret;
}

bool Map::Start()
{
	tileX = app->tex->Load("Assets/maps/x.png");

	return true;
}


void Map::ResetPath(iPoint start)
{
	frontier.Clear();
	visited.Clear();
	breadcrumbs.Clear();

	frontier.Push(start, 0);
	visited.Add(start);
	breadcrumbs.Add(start);

	memset(costSoFar, 0, sizeof(uint) * COST_MAP_SIZE * COST_MAP_SIZE);
}

void Map::DrawPath()
{
	iPoint point;

	// Draw visited
	ListItem<iPoint>* item = visited.start;

	while (item)
	{
		point = item->data;
		TileSet* tileset = GetTilesetFromTileId(26);

		SDL_Rect rec = tileset->GetTileRect(26);
		iPoint pos = MapToWorld(point.x, point.y);

		app->render->DrawTexture(tileset->texture, pos.x, pos.y, &rec);

		item = item->next;
	}

	// Draw frontier
	for (uint i = 0; i < frontier.Count(); ++i)
	{
		point = *(frontier.Peek(i));
		TileSet* tileset = GetTilesetFromTileId(25);

		SDL_Rect rec = tileset->GetTileRect(25);
		iPoint pos = MapToWorld(point.x, point.y);

		app->render->DrawTexture(tileset->texture, pos.x, pos.y, &rec);
	}

	// Draw path
	for (uint i = 0; i < path.Count(); ++i)
	{
		iPoint pos = MapToWorld(path[i].x, path[i].y);
		app->render->DrawTexture(tileX, pos.x, pos.y);
	}
}

int Map::MovementCost(int x, int y) const
{
	int ret = -1;

	if ((x >= 0) && (x < data.width) && (y >= 0) && (y < data.height))
	{
		int id = data.layers.start->next->data->Get(x, y);

		if (id == 0) ret = 3;
		else ret = 0;
	}

	return ret;
}

void Map::ComputePath(int x, int y)
{
	path.Clear();
	iPoint goal = WorldToMap(x, y);

	// L11: TODO 2: Follow the breadcrumps to goal back to the origin
	// add each step into "path" dyn array (it will then draw automatically)
}

bool Map::IsWalkable(int x, int y) const
{
	// L10: TODO 3: return true only if x and y are within map limits
	// and the tile is walkable (tile id 0 in the navigation layer)

	return true;
}

void Map::PropagateBFS()
{
	iPoint curr;
	if (frontier.Pop(curr))
	{
		iPoint neighbors[4];
		neighbors[0].Create(curr.x + 1, curr.y + 0);
		neighbors[1].Create(curr.x + 0, curr.y + 1);
		neighbors[2].Create(curr.x - 1, curr.y + 0);
		neighbors[3].Create(curr.x + 0, curr.y - 1);

		for (uint i = 0; i < 4; ++i)
		{
			if (MovementCost(neighbors[i].x, neighbors[i].y) > 0)
			{
				if (visited.Find(neighbors[i]) == -1)
				{
					frontier.Push(neighbors[i], 0);
					visited.Add(neighbors[i]);

					// L11: TODO 1: Record the direction to the previous node 
					// with the new list "breadcrumps"
				}
			}
		}
	}
}

void Map::PropagateDijkstra()
{
	// L11: TODO 3: Taking BFS as a reference, implement the Dijkstra algorithm
	// use the 2 dimensional array "costSoFar" to track the accumulated costs
	// on each cell (is already reset to 0 automatically)
}

// Draw the map (all requried layers)
void Map::Draw()
{
	if (mapLoaded == false) return;

	// L04: DONE 5: Prepare the loop to draw all tilesets + DrawTexture()
	ListItem<MapLayer*>* layer = data.layers.start;
	// L06: DONE 4: Make sure we draw all the layers and not just the first one	
	while (layer != NULL)
	{
		if (layer->data->properties.GetProperty("NoDrawable") == 0 || colliders)
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = layer->data->Get(x, y);
					if (tileId > 0)
					{
						// L04: DONE 9: Complete the draw function
						TileSet* set = GetTilesetFromTileId(tileId);
						SDL_Rect rect = set->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
					}
				}
			}
		}
		layer = layer->next;
	}
}

void Map::DrawKey()
{
	if (mapLoaded == false) return;

	ListItem<MapLayer*>* layer = data.layers.start;
	while (layer != NULL)
	{

		if (layer->data->name == "Key")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = layer->data->Get(x, y);
					if (tileId > 0)
					{
						// L04: DONE 9: Complete the draw function
						TileSet* set = GetTilesetFromTileId(tileId);
						SDL_Rect rect = set->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
					}
				}
			}
		}
		layer = layer->next;
	}
}

void Map::DrawKeyTaken()
{
	if (mapLoaded == false) return;
	ListItem<MapLayer*>* layer = data.layers.start;
	while (layer != NULL)
	{
		if (layer->data->name == "keytaken")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = layer->data->Get(x, y);
					if (tileId > 0)
					{
						// L04: DONE 9: Complete the draw function
						TileSet* set = GetTilesetFromTileId(tileId);
						SDL_Rect rect = set->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
					}
				}
			}
		}
		layer = layer->next;
	}
}

	

// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	// L05: DONE 1: Add isometric map to world coordinates
	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tileWidth;
		ret.y = y * data.tileHeight;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (data.tileWidth / 2);
		ret.y = (x + y) * (data.tileHeight / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

// L05: DONE 2: Add orthographic world to map coordinates
iPoint Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	// L05: DONE 3: Add the case for isometric maps to WorldToMap
	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tileWidth;
		ret.y = y / data.tileHeight;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{
		float halfWidth = data.tileWidth * 0.5f;
		float halfHeight = data.tileHeight * 0.5f;
		ret.x = int((x / halfWidth + y / halfHeight) / 2);
		ret.y = int((y / halfHeight - (x / halfWidth)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

// L06: DONE 3: Pick the right Tileset based on a tile id
TileSet* Map::GetTilesetFromTileId(int id) const
{
	ListItem<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	while (item)
	{
		if (id < item->data->firstgid)
		{
			set = item->prev->data;
			break;
		}
		set = item->data;
		item = item->next;
	}

	return set;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };

	// L04: DONE 7: Get relative Tile rectangle
	int relativeId = id - firstgid;
	rect.w = tileWidth;
	rect.h = tileHeight;
	rect.x = margin + ((rect.w + spacing) * (relativeId % numTilesWidth));
	rect.y = margin + ((rect.h + spacing) * (relativeId / numTilesWidth));

	return rect;
}

// Called before quitting
bool Map::CleanUp()
{
	LOG("Unloading map");

	// L03: DONE 2: Make sure you clean up any memory allocated from tilesets/map
	// Remove all tilesets
	ListItem<TileSet*>* item;
	item = data.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.Clear();

	// L04: DONE 2: clean up all layer data
	// Remove all layers
	ListItem<MapLayer*>* item2;
	item2 = data.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.Clear();

	// Clean up the pugui tree
	mapFile.reset();

	return true;
}

// Load new map
bool Map::Load(const char* filename)
{
	bool ret = true;
	SString tmp("%s%s", folder.GetString(), filename);

	pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
		ret = false;
	}

	// Load general info
	if(ret == true) ret = LoadMap();

	// L03: DONE 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!
	pugi::xml_node tileset;
	for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true) ret = LoadTilesetDetails(tileset, set);
		if (ret == true) ret = LoadTilesetImage(tileset, set);

		data.tilesets.Add(set);
	}

	// L04: DONE 4: Iterate all layers and load each of them
	// Load layer info
	pugi::xml_node layer;
	for (layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if (ret == true) data.layers.Add(lay);
	}
	
	if(ret == true)
	{
		// L03: T0D0 5: LOG all the data loaded iterate all tilesets and LOG everything
		// L04: T0D0 4: LOG the info for each loaded layer
	}

	mapLoaded = ret;

	return ret;
}

// L03: DONE: Load map general properties
bool Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = mapFile.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		// L03: DONE: Load map general properties
		data.height = map.attribute("height").as_int();
		data.width = map.attribute("width").as_int();
		data.tileHeight = map.attribute("tileheight").as_int();
		data.tileWidth = map.attribute("tilewidth").as_int();

		SString orientation("%s", map.attribute("orientation").value());


		if (orientation == "orthogonal") data.type = MapTypes::MAPTYPE_ORTHOGONAL;

		else if (orientation == "isometric") data.type = MapTypes::MAPTYPE_ISOMETRIC;

		else if (orientation == "staggered") data.type = MapTypes::MAPTYPE_STAGGERED;

		else data.type = MapTypes::MAPTYPE_UNKNOWN;

	}

	return ret;
}

// L03: DONE: Load Tileset attributes
bool Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;

	// L03: DONE: Load Tileset attributes
	set->name = tileset_node.attribute("name").value();
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	set->tileWidth = tileset_node.attribute("tilewidth").as_int();
	set->tileHeight = tileset_node.attribute("tileheight").as_int();

	return ret;
}

// L03: DONE: Load Tileset image
bool Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		// L03: DONE: Load Tileset image
		SString imageSource;
		imageSource.Create("%s%s", folder.GetString(), tileset_node.child("image").attribute("source").value());
		set->texture = app->tex->Load(imageSource.GetString());

		set->texHeight = tileset_node.child("image").attribute("height").as_int();
		set->texWidth = tileset_node.child("image").attribute("width").as_int();

		set->numTilesHeight = set->texHeight / set->tileHeight;
		set->numTilesWidth = set->texWidth / set->tileWidth;		
	}

	return ret;
}

// L04: DONE 3: Create the definition for a function that loads a single layer
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;
	
	// L04: DONE 3: Load a single layer
	layer->height = node.attribute("height").as_int();
	layer->width = node.attribute("width").as_int();
	layer->name = node.attribute("name").value();
	layer->data = new uint[layer->width * layer->height];
	memset(layer->data, 0, layer->width * layer->height);
	pugi::xml_node tilesibling = node.child("data").child("tile");

	for (int i = 0; i < (layer->width * layer->height); i++)
	{
		layer->data[i] = tilesibling.attribute("gid").as_uint();
		tilesibling = tilesibling.next_sibling("tile");
	}
	LoadProperties(node, layer->properties);
	return ret;
}

// L06: DONE 6: Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = true;
	Properties::Property* property = new Properties::Property;
	pugi::xml_node pnode = node;
	for(pnode = node.child("properties").child("property"); pnode; pnode=pnode.next_sibling("property"))
	{
		property->name = pnode.attribute("name").as_string();
		property->value = pnode.attribute("value").as_int();
		properties.list.Add(property);
	}
	return ret;
}