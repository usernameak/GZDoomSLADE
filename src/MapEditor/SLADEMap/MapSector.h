
#ifndef __MAPSECTOR_H__
#define __MAPSECTOR_H__

#include "MapObject.h"
#include "Utility/Polygon2D.h"

class MapSide;
class MapLine;
class MapVertex;

struct doomsector_t
{
	short	f_height;
	short	c_height;
	char	f_tex[8];
	char	c_tex[8];
	short	light;
	short	special;
	short	tag;
};

struct doom64sector_t
{
	short		f_height;
	short		c_height;
	uint16_t	f_tex;
	uint16_t	c_tex;
	uint16_t	color[5];
	short		special;
	short		tag;
	uint16_t	flags;
};

struct extra_floor_t
{
	enum
	{
		// TODO how does vavoom work?  their wiki is always broken.  i think they have floor and ceiling reversed from zdoom mode
		//VAVOOM,
		SOLID = 1,
		SWIMMABLE = 2,
		NONSOLID = 3,

		DISABLE_LIGHTING = 1,
		LIGHTING_INSIDE_ONLY = 2,
		INNER_FOG_EFFECT = 4,
		FLAT_AT_CEILING = 8,
		USE_UPPER_TEXTURE = 16,
		USE_LOWER_TEXTURE = 32,
		ADDITIVE_TRANSPARENCY = 64,
	};

	plane_t floor_plane;
	plane_t ceiling_plane;
	short effective_height;
	short floor_light;
	short ceiling_light;
	unsigned control_sector_index;
	unsigned control_line_index;
	int floor_type;
	float alpha;
	bool draw_inside;
	unsigned char flags;

	bool disableLighting() { return flags & extra_floor_t::DISABLE_LIGHTING; }
	bool lightingInsideOnly() { return flags & extra_floor_t::LIGHTING_INSIDE_ONLY; }
	bool ceilingOnly() { return flags & extra_floor_t::FLAT_AT_CEILING; }
	bool useUpperTexture() { return flags & extra_floor_t::USE_UPPER_TEXTURE; }
	bool useLowerTexture() { return flags & extra_floor_t::USE_LOWER_TEXTURE; }
	bool additiveTransparency() { return flags & extra_floor_t::ADDITIVE_TRANSPARENCY; }
};

enum PlaneType
{
	FLOOR_PLANE,
	CEILING_PLANE,
};

class MapSector : public MapObject
{
	friend class SLADEMap;
	friend class MapSide;
private:
	// Basic data
	string		f_tex;
	string		c_tex;
	short		f_height;
	short		c_height;
	short		light;
	short		special;
	short		tag;

	// Internal info
	vector<MapSide*>	connected_sides;
	bbox_t				bbox;
	Polygon2D			polygon;
	bool				poly_needsupdate;
	long				geometry_updated;
	fpoint2_t			text_point;

	// Computed properties from MapSpecials, not directly stored in the map data
	plane_t				plane_floor;
	plane_t				plane_ceiling;

public:
	// TODO maybe make this private, maybe
	vector<extra_floor_t> extra_floors;

	MapSector(SLADEMap* parent = NULL);
	MapSector(string f_tex, string c_tex, SLADEMap* parent = NULL);
	~MapSector();

	void	copy(MapObject* copy) override;

	string		getFloorTex() const { return f_tex; }
	string		getCeilingTex() const { return c_tex; }
	short		getFloorHeight() const { return f_height; }
	short		getCeilingHeight() const { return c_height; }
	short		getLightLevel() const { return light; }
	short		getSpecial() const { return special; }
	short		getTag() const { return tag; }
	plane_t		getFloorPlane() const { return plane_floor; }
	plane_t		getCeilingPlane() const { return plane_ceiling; }

	string	stringProperty(const string& key) override;
	int		intProperty(const string& key) override;
	void	setStringProperty(const string& key, const string& value) override;
	void	setFloatProperty(const string& key, double value) override;
	void	setIntProperty(const string& key, int value) override;
	void	setFloorHeight(short height);
	void	setCeilingHeight(short height);
	void	setFloorPlane(plane_t p) {
		if (plane_floor != p)
			setGeometryUpdated();
		plane_floor = p;
	}
	void	setCeilingPlane(plane_t p) {
		if (plane_ceiling != p)
			setGeometryUpdated();
		plane_ceiling = p;
	}

	template<PlaneType p> short getPlaneHeight();

	template<PlaneType p>
	plane_t	getPlane();
	template<PlaneType p>
	void	setPlane(plane_t plane);

	fpoint2_t			getPoint(uint8_t point) override;
	void				resetBBox() { bbox.reset(); }
	bbox_t				boundingBox();
	vector<MapSide*>&	connectedSides() { return connected_sides; }
	void				resetPolygon() { poly_needsupdate = true; }
	Polygon2D*			getPolygon();
	bool				isWithin(fpoint2_t point);
	double				distanceTo(fpoint2_t point, double maxdist = -1);
	bool				getLines(vector<MapLine*>& list);
	bool				getVertices(vector<MapVertex*>& list);
	bool				getVertices(vector<MapObject*>& list);
	uint8_t				getLight(int where = 0, int extra_floor_index = -1);
	void				changeLight(int amount, int where = 0);
	rgba_t				getColour(int where = 0, bool fullbright = false);
	rgba_t				getFogColour();
	long				geometryUpdatedTime() { return geometry_updated; }
	void				setGeometryUpdated();

	void	connectSide(MapSide* side);
	void	disconnectSide(MapSide* side);

	void	updateBBox();

	void	writeBackup(mobj_backup_t* backup) override;
	void	readBackup(mobj_backup_t* backup) override;

	operator Debuggable() const {
		if (!this)
			return Debuggable("<sector NULL>");

		return Debuggable(S_FMT("<sector %u>", index));
	}
};

// Note: these MUST be inline, or the linker will complain
template<> inline short MapSector::getPlaneHeight<FLOOR_PLANE>() { return getFloorHeight(); }
template<> inline short MapSector::getPlaneHeight<CEILING_PLANE>() { return getCeilingHeight(); }
template<> inline plane_t MapSector::getPlane<FLOOR_PLANE>() { return getFloorPlane(); }
template<> inline plane_t MapSector::getPlane<CEILING_PLANE>() { return getCeilingPlane(); }
template<> inline void MapSector::setPlane<FLOOR_PLANE>(plane_t plane) { setFloorPlane(plane); }
template<> inline void MapSector::setPlane<CEILING_PLANE>(plane_t plane) { setCeilingPlane(plane); }



#endif //__MAPSECTOR_H__
