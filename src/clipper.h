#ifndef _CLIPPER_
#define _CLIPPER_

#include <math.h>

#include "camera.h"

class Clipper
{
public:
	float frustum[6][4];
	void ExtractFrustum(Camera* cam);
	bool PointInFrustum( float x, float y, float z );
	bool SphereInFrustum( float x, float y, float z, float radius );
};

#endif