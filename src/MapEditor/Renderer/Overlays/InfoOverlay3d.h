
#ifndef __INFO_OVERLAY_3D_H__
#define __INFO_OVERLAY_3D_H__

#include "MapEditor/Edit/Edit3D.h"

class SLADEMap;
class GLTexture;
class MapObject;
class InfoOverlay3D
{
public:
	InfoOverlay3D();
	~InfoOverlay3D();

	void	update(int item_index, MapEditor::ItemType item_type, int extra_floor_index, SLADEMap* map);
	void	draw(int bottom, int right, int middle, float alpha = 1.0f);
	void	drawTexture(float alpha, int x, int y);
	void	reset() { texture = nullptr; object = nullptr; }

private:
	vector<string>		info;
	vector<string>		info2;
	MapEditor::ItemType	current_type;
	int				    current_floor_index;
	string				texname;
	GLTexture*			texture;
	bool				thing_icon;
	MapObject*			object;
	long				last_update;
};

#endif//__INFO_OVERLAY_3D_H__
