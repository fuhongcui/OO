#include <cmath>
struct PointCoord
{
	float x;
	float y;
};
struct LineCoord
{
	PointCoord stPointA;
	PointCoord stPointB;
};
static void DoGetProjectPoint(const LineCoord& stLine, const PointCoord& stPointC, PointCoord& stPointProject, bool& bInSection);