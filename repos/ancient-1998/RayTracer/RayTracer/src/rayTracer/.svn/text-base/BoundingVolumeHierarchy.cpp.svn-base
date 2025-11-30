#include "BoundingVolumeHierarchy.hpp"

namespace rayTracer {

BoundingVolumeHierarchy::Node::Node() {

	volume.empty();
}

void BoundingVolumeHierarchy::Node::recurseDestroy() {

	if (leftChild.isValid())
		leftChild->recurseDestroy();

	if (rightChild.isValid())
		rightChild->recurseDestroy();
	
	deleteSelf();
}

void BoundingVolumeHierarchy::Node::deleteSelf() {

	delete this;
}

void BoundingVolumeHierarchy::destroy() {

	if (mRootNode.isValid()) {

		mRootNode->recurseDestroy();
		mRootNode.destroy();
	}
}

void BoundingVolumeHierarchy::Node::add(Primitive* pPrim) {

	leaf->primitives.addOne() = pPrim;
	pPrim->appendToVolume(volume);
}

bool BoundingVolumeHierarchy::Node::findBestSplitPlane(AAPlane& splitPlane, int& splitLeftCount, int& splitRightCount, int& hintDim, float planeThickness) {
	
	int chosenSplitDim = -1;
	float chosenSplitRatio = 2.0f;

	bool hasSolution = false;
	int chosenSplitLeftCount = 0;
	int chosenSplitRightCount = 0;
	int chosenSplitCommonCount = leaf->primitives.count + 1;
	AAPlane chosenSplitPlane;
	
	for (int splitDim = 0; splitDim < 3; ++splitDim)  {

		Vector3 center;
		Vector3 normal;

		volume.center(center);
		normal.zero();
		normal.el[splitDim] = 1.0f;

		splitPlane.init(center, normal, true);

		int leftCount = 0;
		int rightCount = 0;
		int commonCount = 0;

		for (int pi = 0; pi < leaf->primitives.count; ++pi) {

			SoftPtr<Primitive> prim = leaf->primitives[pi];

			switch (prim->classify(splitPlane, planeThickness)) {

				case PP_Back: ++leftCount; break;
				case PP_Front: ++rightCount; break;
				case PP_BackAndFront: ++commonCount; ++leftCount; ++rightCount; break;
				default: break;
			}
		}

		/*
		float splitRatio = 0.0f;

		if (rightCount)
			splitRatio = (float) leftCount / ((float) (leftCount + rightCount));
		else
			splitRatio = (float) rightCount / ((float) (leftCount + rightCount));

		if (splitRatio > 1.0f)
			splitRatio = 1.0f / splitRatio;

		if (1.0f - splitRatio > splitRatio)
			splitRatio = 1.0f - splitRatio;
			*/

		if (leftCount && rightCount && commonCount < chosenSplitCommonCount) {

			hasSolution = true;
			chosenSplitDim = splitDim;
			//chosenSplitRatio = splitRatio;

			chosenSplitPlane = splitPlane;

			chosenSplitCommonCount = commonCount;
			chosenSplitLeftCount = leftCount;
			chosenSplitRightCount = rightCount;
		}
	}

	hintDim = (chosenSplitDim + 1) % 3;
	splitPlane = chosenSplitPlane;
	
	splitLeftCount = chosenSplitLeftCount;
	splitRightCount = chosenSplitRightCount;

	return hasSolution;
}

void BoundingVolumeHierarchy::Node::split(int maxLeafPrimitiveCount, int& hintDim, float planeThickness) {

	if (leaf.isNull() || leaf->primitives.count <= maxLeafPrimitiveCount)
		return;

	AAPlane splitPlane;
	int splitLeftCount;
	int splitRightCount;

	if (findBestSplitPlane(splitPlane, splitLeftCount, splitRightCount, hintDim, planeThickness)
		&& 
		splitLeftCount != leaf->primitives.count && splitRightCount != leaf->primitives.count) {
	
		splitLeftCount = 0;
		splitRightCount = 0;

		for (int pi = 0; pi < leaf->primitives.count; ++pi) {

			SoftPtr<Primitive> prim = leaf->primitives[pi];

			switch (prim->chooseClassify(splitPlane, planeThickness)) {

				case PP_Back: ++splitLeftCount; break;
				case PP_Front: ++splitRightCount; break;
				//case PP_BackAndFront: leftChild->add(prim); rightChild->add(prim); break;
				default: break;
			}
		}


		//create nodes close in memmory
		//they might still be far apart from next nodes though because of
		//leafs coming in between! improve cache friendliness for traversal in general
		if (splitLeftCount) {

			leftChild = new Node();
		}

		if (splitRightCount) {

			rightChild = new Node();
		}

		if (splitLeftCount) {

			//leftChild = new Node();
			leftChild->leaf = new Leaf();
			leftChild->leaf->primitives.reserve(splitLeftCount);
		}

		if (splitRightCount) {

			//rightChild = new Node();
			rightChild->leaf = new Leaf();
			rightChild->leaf->primitives.reserve(splitRightCount);
		}

		for (int pi = 0; pi < leaf->primitives.count; ++pi) {

			SoftPtr<Primitive> prim = leaf->primitives[pi];

			/*
			switch (prim->classify(splitPlane, planeThickness)) {

				case PP_Back: leftChild->add(prim); break;
				case PP_Front: rightChild->add(prim); break;
				case PP_BackAndFront: leftChild->add(prim); rightChild->add(prim); break;
				default: break;
			}
			*/

			switch (prim->chooseClassify(splitPlane, planeThickness)) {

				case PP_Back: leftChild->add(prim); break;
				case PP_Front: rightChild->add(prim); break;
				//case PP_BackAndFront: leftChild->add(prim); rightChild->add(prim); break;
				default: break;
			}
		}

		leaf.destroy();

		if (leftChild.isValid()) {

			leftChild->volume.growBy(planeThickness);
			leftChild->split(maxLeafPrimitiveCount, hintDim, planeThickness);
		}

		if (rightChild.isValid()) {

			rightChild->volume.growBy(planeThickness);
			rightChild->split(maxLeafPrimitiveCount, hintDim, planeThickness);
		}

	} else {

		//assert(false);
	}
}

void BoundingVolumeHierarchy::Node::recurseCount(unsigned int& maxDepth, unsigned int& count) {

	++maxDepth;

	if (leaf.isValid()) {

		++count;
	}

	unsigned int maxLeftDepth = maxDepth;
	unsigned int maxRightDepth = maxDepth;

	if (leftChild.isValid())
		leftChild->recurseCount(maxLeftDepth, count);

	if (rightChild.isValid())
		rightChild->recurseCount(maxRightDepth, count);

	maxDepth = std::max(maxLeftDepth, maxRightDepth);
}

void BoundingVolumeHierarchy::build(Scene& scene, int maxLeafPrimitiveCount, float planeThickness) {

	destroy();
	mScene = &scene;

	mRootNode = new Node();

	{
		mRootNode->leaf = new Leaf();
		mRootNode->leaf->primitives.reserve(scene.primitives().count);

		for (int pi = 0; pi < scene.primitives().count; ++pi) {

			SoftPtr<Primitive> prim = scene.primitives()[pi];

			mRootNode->add(prim);
		}

		int hintDim = 0;

		mRootNode->split(maxLeafPrimitiveCount, hintDim, planeThickness);
	}

	mLeafCount = 0;
	mMaxDepth = 0;
	mRootNode->recurseCount(mMaxDepth, mLeafCount);

	#ifdef RayTracerConfig_SpatialAccel_Iter
		
		#ifdef RayTracerConfig_EnablePackets
			mPacketTraversalStack.create(mMaxDepth * 2);
		#endif
		
		mTraversalStack.create(mMaxDepth * 2);
		
	#endif
}

/*
 * http://www.cs.utah.edu/~awilliam/box/
 * Ray-box intersection using IEEE numerical properties to ensure that the
 * test is both robust and efficient, as described in:
 *
 *      Amy Williams, Steve Barrus, R. Keith Morley, and Peter Shirley
 *      "An Efficient and Robust Ray-Box Intersection Algorithm"
 *      Journal of graphics tools, 10(1):49-54, 2005
 *
 */
bool BoundingVolumeHierarchy::Node::intersects(const Ray& ray, float& t0, float& t1) {

	//const AAB& volume = mVolume;

	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	tmin = (volume.parameters[ray.sign[0]].x - ray.origin.x) * ray.inv_direction.x;
	tmax = (volume.parameters[1-ray.sign[0]].x - ray.origin.x) * ray.inv_direction.x;
	tymin = (volume.parameters[ray.sign[1]].y - ray.origin.y) * ray.inv_direction.y;
	tymax = (volume.parameters[1-ray.sign[1]].y - ray.origin.y) * ray.inv_direction.y;
	if ( (tmin > tymax) || (tymin > tmax) ) 
		return false;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;
	tzmin = (volume.parameters[ray.sign[2]].z - ray.origin.z) * ray.inv_direction.z;
	tzmax = (volume.parameters[1-ray.sign[2]].z - ray.origin.z) * ray.inv_direction.z;
	if ( (tmin > tzmax) || (tzmin > tmax) ) 
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	if ( (tmin < t1) && (tmax > t0) ) {

		t0 = tmin;
		t1 = tmax;

		return true;
	}

	return false;
}

bool BoundingVolumeHierarchy::Node::intersects2(const Ray& ray, float& outTMin) {

	//const AAB& volume = mVolume;

	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	tmin = (volume.parameters[ray.sign[0]].x - ray.origin.x) * ray.inv_direction.x;
	tmax = (volume.parameters[1-ray.sign[0]].x - ray.origin.x) * ray.inv_direction.x;
	tymin = (volume.parameters[ray.sign[1]].y - ray.origin.y) * ray.inv_direction.y;
	tymax = (volume.parameters[1-ray.sign[1]].y - ray.origin.y) * ray.inv_direction.y;
	if ( (tmin > tymax) || (tymin > tmax) ) 
		return false;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;
	tzmin = (volume.parameters[ray.sign[2]].z - ray.origin.z) * ray.inv_direction.z;
	tzmax = (volume.parameters[1-ray.sign[2]].z - ray.origin.z) * ray.inv_direction.z;
	if ( (tmin > tzmax) || (tzmin > tmax) ) 
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	if ( (tmin >= 0.0f) ) {

		outTMin = tmin;

		return true;
	}

	return false;
}

bool BoundingVolumeHierarchy::Node::shootRay(const RayShot& rayShot, RayShotHit& result, float minT, float maxT) {

	if (leaf.isValid()) {

		bool intersects = false;
		LeafPrimitives& primitives = leaf->primitives;

		for (int pi = 0; pi < primitives.count; ++pi) {

			SoftPtr<Primitive> prim = primitives[pi];

			{
				//TODO! pass inters directly that way no need to set to infinity and early out works as well
				//RayIntersectionT intersection;
				//intersection.t = std::numeric_limits<float>::infinity();
			
				if (prim->intersects(rayShot, result.inters)) {

					//if (intersection.t < result.inters.t) 
					{
						//result.inters.t = intersection.t;
						intersects = true;
						result.primID = pi;

						#ifdef RayTracerConfig_Trace_SpatialAccel_Traversal
							printf("0x%x,", (unsigned int) prim.ptr());
						#endif
					} 
				}
			}
		}

		return intersects;

	} else {

		/*

		//slower because of cache organization?

		float leftMinT = minT, leftMaxT = maxT;
		float& rightMinT = minT, &rightMaxT = maxT;
		
		bool leftIntersects = (leftChild.isValid() && leftChild->intersects(dref(rayShot.ray), leftMinT, leftMaxT));
		bool rightIntersects =  (rightChild.isValid() && rightChild->intersects(dref(rayShot.ray), rightMinT, rightMaxT));

		bool leftHit = false;

		if (leftIntersects) {

			#ifdef RayTracerConfig_Trace_SpatialAccel_Traversal
				printf("L,");
			#endif

			leftHit = leftChild->shootRay(rayShot, result, leftMinT, leftMaxT);
		}

		bool rightHit = false;

		if (rightIntersects
			&& (!leftHit || result.inters.t > rightMinT)
			) {

			#ifdef RayTracerConfig_Trace_SpatialAccel_Traversal
				printf("R,");
			#endif

			rightHit = rightChild->shootRay(rayShot, result, rightMinT, rightMaxT);
		}

		return leftHit || rightHit; 
		*/

		float leftMinT = minT, leftMaxT = maxT;
		bool leftHit = false;

		if (leftChild.isValid() && leftChild->intersects(dref(rayShot.ray), leftMinT, leftMaxT)) {

			#ifdef RayTracerConfig_Trace_SpatialAccel_Traversal
				printf("L,");
			#endif

			leftHit = leftChild->shootRay(rayShot, result, leftMinT, leftMaxT);
		}

		//bool rightHit = false;

		if (rightChild.isValid() 
			&& rightChild->intersects(dref(rayShot.ray), minT, maxT)
			&& (!leftHit || result.inters.t > minT)
			) {

			#ifdef RayTracerConfig_Trace_SpatialAccel_Traversal
				printf("R,");
			#endif

			//rightHit = 
			rightChild->shootRay(rayShot, result, minT, maxT);
		}

		return result.hasPrimitive();
		//return leftHit || rightHit;
	}
}

void BoundingVolumeHierarchy::Node::shootRay2(const RayShot& rayShot, RayShotHit& result, float minT, float maxT) {

	if (this->intersects(dref(rayShot.ray), minT, maxT)
		&& minT < result.inters.t
		) {

		if (leaf.isValid()) {

			LeafPrimitives& primitives = leaf->primitives;

			for (int pi = 0; pi < primitives.count; ++pi) {

				SoftPtr<Primitive> prim = primitives[pi];

				{
					if (prim->intersects(rayShot, result.inters)) {

						result.primID = pi;

						#ifdef RayTracerConfig_Trace_SpatialAccel_Traversal
							printf("0x%x,", (unsigned int) prim.ptr());
						#endif
					}
				}
			}

		} else {

			if (leftChild.isValid())
				leftChild->shootRay2(rayShot, result, minT, maxT);

			if (rightChild.isValid())
				rightChild->shootRay2(rayShot, result, minT, maxT);
		}
	}
}

void BoundingVolumeHierarchy::Node::shootRay3(Stack1& stack, const RayShot& rayShot, RayShotHit& result, float minT) {

	SoftPtr<Node> nodePtr = this;

	while (nodePtr.isValid()) {

		Node& node = nodePtr;
		float nodeMinT = minT;

		if (node.intersects2(dref(rayShot.ray), nodeMinT)
			&& nodeMinT < result.inters.t
			) {
		
			if (node.leaf.isValid()) {

				LeafPrimitives& primitives = node.leaf->primitives;

				for (int pi = 0; pi < primitives.count; ++pi) {

					SoftPtr<Primitive> prim = primitives[pi];

					{
						if (prim->intersects(rayShot, result.inters)) {

							result.primID = pi;

							#ifdef RayTracerConfig_Trace_SpatialAccel_Traversal
								printf("0x%x,", (unsigned int) prim.ptr());
							#endif
						}
					}
				}

				nodePtr = stack.popNode();

			} else {

				if (node.rightChild.isValid())
					stack.pushNode(node.rightChild);
				
				if (node.leftChild.isValid())
					nodePtr = node.leftChild;
				else
					nodePtr = stack.popNode();
			}

		} else {

			nodePtr = stack.popNode();
		}
	}
}

bool BoundingVolumeHierarchy::shootRay(const RayShot& rayShot, RayShotHit& result) {


	if (mRootNode.isValid()) {

		#ifdef RayTracerConfig_SpatialAccel_Iter
			mRootNode->shootRay3(mTraversalStack, rayShot, result, 0.0f);
		#else
			//mRootNode->shootRay(rayShot, result, 0.0f, std::numeric_limits<float>::infinity());
			mRootNode->shootRay2(rayShot, result, 0.0f, std::numeric_limits<float>::infinity());
		#endif

		#ifdef RayTracerConfig_Trace_SpatialAccel_Traversal
			printf("\n");
		#endif

		return result.hasPrimitive();
	}

	return false;
}

#ifdef RayTracerConfig_EnablePackets

bool BoundingVolumeHierarchy::Node::intersects(const PacketRayShot& rayPacket, PacketRayMask& rayPacketState, Packet4& tmin, Packet4& tmax, bool& tPacketOutIsValid, const bool forceTestAllRays) {

	tPacketOutIsValid = false;

	if (!forceTestAllRays) {

		//check 1st ray
		{
			const int rayIndex = rayPacketState.getFirst();
			
			if (rayIndex < 0)
				return false;
			
			float inter_tmin = tmin.el[rayIndex];
			float inter_tmax = tmax.el[rayIndex];

			for (int dim = 0; dim < 3; ++dim) {

				//float dtmin = (volume.parameters[rayPacket.ray4_sign.deli(dim,0)].el[dim] - rayPacket.ray4_origin.del(dim,0)) * rayPacket.ray4_rdirection.del(dim,0);
				//float dtmax = (volume.parameters[1-rayPacket.ray4_sign.deli(dim,0)].el[dim] - rayPacket.ray4_origin.del(dim,0)) * rayPacket.ray4_rdirection.del(dim,0);

				const float dt0 = (volume.min.el[dim] - rayPacket.ray4_origin.del(dim,0)) * rayPacket.ray4_rdirection.del(dim,0);
				const float dt1 = (volume.max.el[dim] - rayPacket.ray4_origin.del(dim,0)) * rayPacket.ray4_rdirection.del(dim,0);
				const float dtmin = dt0 < dt1 ? dt0 : dt1;
				const float dtmax = dt0 < dt1 ? dt1 : dt0;

				inter_tmin = (inter_tmin < dtmin) ? dtmin : inter_tmin;
				inter_tmax = (dtmax <  inter_tmax) ? dtmax : inter_tmax;
			}

			if (inter_tmin <= inter_tmax) 
				return true;

			if (rayPacketState.getActiveCount() == 1)
				return false;
		}

		//check frustum
		{
		}
	}

	//check all rays after 1st ray
	{
		tPacketOutIsValid = true;

		__m128 inter_tmin = tmin.el4;
		__m128 inter_tmax = tmax.el4;

		for (int dim = 0; dim < 3; ++dim) {

			const __m128 vol_min = _mm_set1_ps(volume.min.el[dim]);
			const __m128 vol_max = _mm_set1_ps(volume.max.el[dim]);

			const __m128 dt0 = _mm_mul_ps(_mm_sub_ps(vol_min, rayPacket.ray4_origin[dim]), rayPacket.ray4_rdirection[dim]);
			const __m128 dt1 = _mm_mul_ps(_mm_sub_ps(vol_max, rayPacket.ray4_origin[dim]), rayPacket.ray4_rdirection[dim]);

			const __m128 dtmin = _mm_min_ps(dt0, dt1);
			const __m128 dtmax = _mm_max_ps(dt0, dt1);

			inter_tmin = _mm_max_ps(inter_tmin, dtmin);
			inter_tmax = _mm_min_ps(dtmax, inter_tmax);
		}

		const __m128 inters = _mm_cmple_ps(inter_tmin, inter_tmax);
		const __m128 active_inters = _mm_and_ps(inters, rayPacketState.active.el4);

		rayPacketState.setActiveMask(active_inters);
		
		tmin.el4 = inter_tmin; 
		tmax.el4 = inter_tmax;

		return (rayPacketState.getActiveCount() > 0);
	}

	return false;
}

bool BoundingVolumeHierarchy::Node::intersects(const Constants& cts, const PacketRayShot& rayPacket, PacketRayMask& rayPacketState, Packet4& tmin, const bool forceFullTest) {
	
	if (!forceFullTest) {

		//check 1st ray
		{
			const int rayIndex = rayPacketState.getFirst();
			
			if (rayIndex < 0)
				return false;
			
			float inter_tmin = tmin.el[rayIndex];
			float inter_tmax = std::numeric_limits<float>::infinity();

			for (int dim = 0; dim < 3; ++dim) {

				//float dtmin = (volume.parameters[rayPacket.ray4_sign.deli(dim,0)].el[dim] - rayPacket.ray4_origin.del(dim,0)) * rayPacket.ray4_rdirection.del(dim,0);
				//float dtmax = (volume.parameters[1-rayPacket.ray4_sign.deli(dim,0)].el[dim] - rayPacket.ray4_origin.del(dim,0)) * rayPacket.ray4_rdirection.del(dim,0);

				const float dt0 = (volume.min.el[dim] - rayPacket.ray4_origin.del(dim,0)) * rayPacket.ray4_rdirection.del(dim,0);
				const float dt1 = (volume.max.el[dim] - rayPacket.ray4_origin.del(dim,0)) * rayPacket.ray4_rdirection.del(dim,0);
				const float dtmin = dt0 < dt1 ? dt0 : dt1;
				const float dtmax = dt0 < dt1 ? dt1 : dt0;

				inter_tmin = (inter_tmin < dtmin) ? dtmin : inter_tmin;
				inter_tmax = (dtmax <  inter_tmax) ? dtmax : inter_tmax;
			}

			if (inter_tmin <= inter_tmax) 
				return true;

			if (rayPacketState.getActiveCount() == 1)
				return false;
		}

		//check frustum
		{
		}
	}
	
	//check all rays after 1st ray
	{
		__m128 inter_tmin = tmin.el4;
		__m128 inter_tmax = cts.infinity4;

		for (int dim = 0; dim < 3; ++dim) {

			const __m128 vol_min = _mm_set1_ps(volume.min.el[dim]);
			const __m128 vol_max = _mm_set1_ps(volume.max.el[dim]);

			const __m128 dt0 = _mm_mul_ps(_mm_sub_ps(vol_min, rayPacket.ray4_origin[dim]), rayPacket.ray4_rdirection[dim]);
			const __m128 dt1 = _mm_mul_ps(_mm_sub_ps(vol_max, rayPacket.ray4_origin[dim]), rayPacket.ray4_rdirection[dim]);

			const __m128 dtmin = _mm_min_ps(dt0, dt1);
			const __m128 dtmax = _mm_max_ps(dt0, dt1);

			inter_tmin = _mm_max_ps(inter_tmin, dtmin);
			inter_tmax = _mm_min_ps(dtmax, inter_tmax);
		}

		const __m128 inters = _mm_cmple_ps(inter_tmin, inter_tmax);
		const __m128 active_inters = _mm_and_ps(inters, rayPacketState.active.el4);

		rayPacketState.setActiveMask(active_inters);
		
		tmin.el4 = inter_tmin; 

		return (rayPacketState.getActiveCount() > 0);
	}

	return false;
}

bool BoundingVolumeHierarchy::Node::shootRayPacket(const Constants& cts, const PacketRayShot& rayPacket, PacketRayHit& t, PacketRayMask& rayPacketState, const Packet4& inTmin, const Packet4& inTmax) {

	if (leaf.isValid()) {

		LeafPrimitives& primitives = leaf->primitives;
		bool hasIntersections = false;

		for (int pi = 0; pi < primitives.count; ++pi) {

			Primitive_Triangle& tri = dref((Primitive_Triangle*) primitives[pi]);

			{
				hasIntersections = tri.intersectRayTri_impl_project(cts, rayPacket, t, rayPacketState, pi) || hasIntersections;
			}
		}

		return hasIntersections;

	} else {

		PacketRayMask leftRayPacketState(rayPacketState);
		Packet4 leftTMin = inTmin;
		Packet4 leftTMax = inTmax;
		bool hasLeftHit = false;
		bool tPacketOutIsValid;

		if (leftChild.isValid() && leftChild->intersects(rayPacket, leftRayPacketState, leftTMin, leftTMax, tPacketOutIsValid, false)) {

			#ifdef RayTracerConfig_Trace_SpatialAccel_Traversal
				printf("L,");
			#endif
			
			hasLeftHit = leftChild->shootRayPacket(cts, rayPacket, t, leftRayPacketState, leftTMin, leftTMax);
		}

		PacketRayMask rightRayPacketState(rayPacketState);
		Packet4 rightTMin = inTmin;
		Packet4 rightTMax = inTmax;
		bool hasRightHit = false;
		

		if (rightChild.isValid() 
			#ifdef RayTracerConfig_EnablePacketTraversalOcclusion
				&& rightChild->intersects(rayPacket, rightRayPacketState, rightTMin, rightTMax, tPacketOutIsValid, hasLeftHit)
				&& (!hasLeftHit || (_mm_movemask_ps(_mm_cmpgt_ps(t.t.el4, rightTMin.el4)) != 0))
			#else
				&& rightChild->intersects(rayPacket, rightRayPacketState, rightTMin, rightTMax, tPacketOutIsValid, false)
			#endif
			) {

		
			#ifdef RayTracerConfig_Trace_SpatialAccel_Traversal
				printf("R,");
			#endif

			hasRightHit = rightChild->shootRayPacket(cts, rayPacket, t, rightRayPacketState, rightTMin, rightTMax);
		}

		return hasLeftHit || hasRightHit;
	}
}

void BoundingVolumeHierarchy::Node::shootRayPacketIterative(PacketStack& stack, const Constants& cts, const PacketRayShot& rayPacket, PacketRayHit& t, PacketRayMask& rayPacketState, const Packet4& minT) {

	SoftPtr<Node> nodePtr = this;

	PacketRayMask rayPacketMask;
	rayPacketMask.reset(cts);

	bool hasIntersections = false;
	//printf("0x%x\n", &rayPacketMask.active.el4);

	while (nodePtr.isValid()) {

		Node& node = nodePtr;
		Packet4 nodeMinT = minT;
		
		if (
			#ifdef RayTracerConfig_EnablePacketTraversalOcclusion
				node.intersects(cts, rayPacket, rayPacketMask, nodeMinT, hasIntersections)
				&& (_mm_movemask_ps(_mm_cmplt_ps(nodeMinT, t.t)) != 0)
			#else
				node.intersects(cts, rayPacket, rayPacketMask, nodeMinT, false)
			#endif
			) {
		
			if (node.leaf.isValid()) {

				LeafPrimitives& primitives = node.leaf->primitives;

				for (int pi = 0; pi < primitives.count; ++pi) {

					Primitive_Triangle& tri = dref((Primitive_Triangle*) primitives[pi]);
					tri.intersectRayTri_impl_project(cts, rayPacket, t, rayPacketState, pi);
				}
				
				#ifdef RayTracerConfig_EnablePacketTraversalOcclusion
					hasIntersections = hasIntersections || (t.getHitCount(cts) > 0);
				#endif
				nodePtr = stack.popNode(rayPacketMask);

			} else {

				if (node.rightChild.isValid())
					stack.pushNode(node.rightChild, rayPacketMask);
				
				if (node.leftChild.isValid())
					nodePtr = node.leftChild;
				else
					nodePtr = stack.popNode(rayPacketMask);
			}

		} else {

			nodePtr = stack.popNode(rayPacketMask);
		}
	}
}

bool BoundingVolumeHierarchy::shootRayPacket(const Constants& cts, const PacketRayShot& rayPacket, PacketRayHit& t) {

	if (mRootNode.isValid()) {

		PacketRayMask rayPacketState;
		rayPacketState.reset(cts);

		#ifdef RayTracerConfig_SpatialAccel_Iter
			mRootNode->shootRayPacketIterative(mPacketTraversalStack, cts, rayPacket, t, rayPacketState, cts.zero4);
		#else
			mRootNode->shootRayPacket(cts, rayPacket, t, rayPacketState, cts.zero4, cts.infinity4);
		#endif
		
		#ifdef RayTracerConfig_Trace_SpatialAccel_Traversal
			printf("\n");
		#endif
	}

	return false;
}
#endif

}