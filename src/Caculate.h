#ifndef _CACULATE_H_
#define _CACULATE_H_
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
void GetProjectPoint(const LineCoord& stLine, const PointCoord& stPointC, PointCoord& stPointProject, bool& bInSection);

#endif