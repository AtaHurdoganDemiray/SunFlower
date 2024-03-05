#ifndef GhoshItem_h
#define GhoshItem_h

class GhoshItem
{
public:
	enum GhsType{ Geo, Tool };
	GhoshItem();
	GhoshItem(const GhoshItem& ghs);
	GhoshItem(const VLine& edge, GhsType type, int index);
	~GhoshItem();


	VStr m_name;
	bool  m_isTurningPoint;
	GhsType m_type;
	int m_index;
	vreal m_angle;
	GhsType GetGType();
	VLine GetCurL();
	int GetIndex();
	VLine m_line;
public:
	vreal GetAngle();
	void ReverseDirection(int dir);

	struct SortVec
	{
		inline bool operator ()  (const GhoshItem &item1, const GhoshItem&item2)
		{
			return item1.m_angle < item2.m_angle;
		}
	};

	static void Sort(std::vector<GhoshItem>& vec);
	void SetLine(const VLine& line);
};

#endif