#include "TIStaticGeometryRealTimeStructs.h"

DWORD		TIStaticGeometryRealTimeNode::RenderID = 0;
D3DMATRIX	TIStaticGeometryRealTimeNode::ViewProjectionMatrix;
LONG		TIStaticGeometryRealTimeNode::RenderedNodeCount = 0;
D3DVECTOR	TIStaticGeometryRealTimeNode::FrustumPoints[8];
TIFrustum	TIStaticGeometryRealTimeNode::Frustum;

LONG		TIStaticGeometryRealTimeMaterialGroup::RenderedPolyCount = 0;
LONG		TIStaticGeometryRealTimeMaterialGroup::RenderedVertexCount = 0;