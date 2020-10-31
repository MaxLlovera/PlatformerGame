
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>

Map::Map() : Module(), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

// L06: DONE 7: Ask for the value of a custom property
int Properties::GetProperty(const char* value, int defaultValue) const
{
	ListItem<Property*>* property = list.start;
	while (property != NULL) 
	{
		if (strcmp(property->data->name.GetString(), value) == 0)
		{
			return property->data->value;
		}
		property = property->next;
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

		float half_width = data.tileWidth * 0.5f;
		float half_height = data.tileHeight * 0.5f;
		ret.x = int((x / half_width + y / half_height) / 2);
		ret.y = int((y / half_height - (x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

// L06: TODO ?? 3: Pick the right Tileset based on a tile id
TileSet* Map::GetTilesetFromTileId(int id) const
{
	ListItem<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	while (item->next != NULL)
	{
		if (id > (set->numTilesWidth * set->numTilesHeight))
		{
			item = item->next;
			set = item->data;
		}
		else return set;
	}
	return set;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };
	// L04: DONE 7: Get relative Tile rectangle
	int relativeTileId = id - firstgid;
	rect.w = tileWidth;
	rect.h = tileHeight;
	rect.x = margin + ((rect.w + spacing) * (relativeTileId % numTilesWidth));
	rect.y = margin + ((rect.h + spacing) * (relativeTileId / numTilesWidth));
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
	data.tilesets.clear();

	// L04: DONE 2: clean up all layer data
	// Remove all layers
	ListItem<MapLayer*>* item2;
	item2 = data.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.clear();

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
    if(ret == true)
    {
        // L03: DONE 3: Create and call a private function to load and fill all your map data
		ret = LoadMap();
	}

    // L03: DONE 4: Create and call a private function to load a tileset
    // remember to support more any number of tilesets!
	pugi::xml_node tileset;
	for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true) ret = LoadTilesetDetails(tileset, set);
		if (ret == true) ret = LoadTilesetImage(tileset, set);

		data.tilesets.add(set);
	}

	// L04: DONE 4: Iterate all layers and load each of them
	// Load layer info
	pugi::xml_node layer;
	for (layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if (ret == true) data.layers.add(lay);
	}
    
    if(ret == true)
    {
        // L03: TODO 5: LOG all the data loaded iterate all tilesets and LOG everything



		// L04: TODO 4: LOG the info for each loaded layer
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


		if (orientation == "orthogonal")
		{
			data.type = MapTypes::MAPTYPE_ORTHOGONAL;
		}
		else if (orientation == "isometric")
		{
			data.type = MapTypes::MAPTYPE_ISOMETRIC;
		}
		else if (orientation == "staggered")
		{
			data.type = MapTypes::MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MapTypes::MAPTYPE_UNKNOWN;
		}
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
		properties.list.add(property);
	}
	return ret;
}