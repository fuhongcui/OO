#include "Caculate.h"
void DoGetProjectPoint(const LineCoord& stLine, const PointCoord& stPointC, PointCoord& stPointProject, bool& bInSection)
{
	PointCoord stPointA = stLine.stPointA;
	PointCoord stPointB = stLine.stPointB;
	bInSection = false;
	if(fabs(stPointB.x - stPointA.x) < 0.0000001)
	{
		stPointProject.x = stPointA.x;
		stPointProject.y = stPointC.y;
		if((stPointC.y >= stPointA.y && stPointC.y <= stPointB.y) || (stPointC.y >= stPointB.y && stPointC.y <= stPointA.y))
		{
			bInSection = true;
		}
		return;
	}
	else if(fabs(stPointB.y - stPointA.y) < 0.0000001)
	{
		stPointProject.x = stPointC.x;
		stPointProject.y = stPointA.y;
		if((stPointC.x >= stPointA.x && stPointC.x <= stPointB.x) || (stPointC.x >= stPointB.x && stPointC.x <= stPointA.x))
		{
			bInSection = true;
		}
		return;
	}
	else
	{
		float k1 = (stPointB.y - stPointA.y) / (stPointB.x - stPointA.x);
		float b1 = stPointA.y - ((stPointB.y - stPointA.y) / (stPointB.x - stPointA.x) * stPointA.x);
		
		float k2 = -1.0f / k1;
		float b2 = stPointC.y - k2 * stPointC.x;
		
		if(fabs(k1 - k2) < 0.0000001)
		{
			stPointProject.x = stPointC.x;
			stPointProject.y = stPointC.y;
			if((stPointC.x >= std::min(stPointA.x, stPointB.x) && stPointC.x <= std::max(stPointA.x, stPointB.x)) && (stPointC.y >= std::min(stPointA.y, stPointB.y) && stPointC.y <= std::max(stPointA.y, stPointB.y)))
			{
				bInSection = true;
			}
			return;
		}
		stPointProject.x = (b2 - b1) / (k1 -k2);
		stPointProject.y = ((b2 - b1) / (k1 -k2)) * k1 + b1;
		if((stPointProject.x >= std::min(stPointA.x, stPointB.x) && stPointProject.x <= std::max(stPointA.x, stPointB.x)) && (stPointProject.y >= std::min(stPointA.y, stPointB.y) && stPointProject.y <= std::max(stPointA.y, stPointB.y)))
		{
			bInSection = true;
		}
	}
	
}