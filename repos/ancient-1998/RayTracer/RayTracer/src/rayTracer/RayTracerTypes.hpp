#ifndef _rayTracer_RayTracerTypes_hpp
#define _rayTracer_RayTracerTypes_hpp

#include "../SuperHeader.hpp"
#include "RayTracerConfig.hpp"

#include "../WE3/math/WEVector.h"
#include "../WE3/math/WEVector2.h"
#include "../WE3/math/WEMatrix.h"
#include "../WE3/math/WEPlane.h"

#include "../WE3/coordSys/WECoordSys.h"
#include "../WE3/scene/WESceneTransform.h"
#include "../WE3/scene/WESceneDirection.h"
#include "../WE3/WEString.h"

#include "Clock.hpp"

#include <limits>

#ifdef RayTracerConfig_EnablePackets
	#include "xmmintrin.h"
	#include "dvec.h"
#endif

namespace rayTracer {

	enum RenderDirectionEnum {

		Render_Right = 0, Render_Up
	};

	enum DiffuseMode {

		DiffuseMode_None = -1,
		DiffuseMode_Flat,
		DiffuseMode_Lambert,
	};

	enum SpecularMode {

		SpecularMode_None = -1,
		SpecularMode_Phong,
		SpecularMode_BlinnPhong,
	};

	enum RefractionMode {

		RefractionMode_None = -1,
		RefractionMode_Normal,
	};

	enum ShadowMode {

		ShadowMode_None = -1,
		ShadowMode_Hard
	};

	enum ExposureMode {

		ExposureMode_None = -1,
		ExposureMode_Cap,
		ExposureMode_Exp, 
		ExposureMode_TestGlow, 
	};

	struct RenderParams {

		unsigned int superSampleFactor;
		DiffuseMode diffuseMode;
		ShadowMode shadowMode;
		SpecularMode specularMode;
		RefractionMode refractionMode;
		ExposureMode exposureMode;

		Vector3 backColor;

		float exposure;
		float voidRefractionIndex;

		int maxRayDepth;
		float minColorContribution;
		Vector3 ambient;
		int pixelSpacing;

		bool useSelfTolerance;

		RenderParams() {

			superSampleFactor = 1;
			diffuseMode = DiffuseMode_Lambert;
			shadowMode = ShadowMode_Hard;
			specularMode = SpecularMode_BlinnPhong;
			exposureMode = ExposureMode_Cap;
			refractionMode = RefractionMode_Normal;

			exposure = -1.0f;
			voidRefractionIndex = 1.0f;

			maxRayDepth = 4;
			minColorContribution = 0.05f;
			ambient.zero();
			pixelSpacing = 1;

			backColor.zero();

			useSelfTolerance = false;
		}
	};

	struct RayTolerance {

		float t;

		RayTolerance() {

			t = 0.0f;
		}
	};

	struct Ray {

		Vector3 origin;
		Vector3 direction;
		Vector3 inv_direction;

		int sign[3];

		void setupByTarget(const Vector3& origin, const Vector3& target) {

			this->origin = origin;
			target.subtract(origin, direction);
			direction.normalize();

			prepare();
		}

		void prepare() {

			sign[0] = direction.el[0] < 0.0f;
			sign[1] = direction.el[1] < 0.0f;
			sign[2] = direction.el[2] < 0.0f;

			inv_direction.el[0] = 1.0f / direction.el[0];
			inv_direction.el[1] = 1.0f / direction.el[1];
			inv_direction.el[2] = 1.0f / direction.el[2];
		}

		void getParametricPoint(const float& t, Vector3& pt) const {

			origin.addMultiplication(direction, t, pt);
		}
	};

	struct RayIntersectionT {

		float t;

		inline void reset() {

			t = std::numeric_limits<float>::infinity();
		}
	};

	struct RayIntersection {

		Vector3 point;
		Vector3 normal;

		void reflectRay(const Vector3& incomingDir, Ray& reflected) const {

			reflected.origin = point;

			reflectDir(incomingDir, reflected.direction);
			reflected.prepare();
		}

		void reflectDir(const Vector3& incomingDir, Vector3& reflected) const {

			float c1 = -normal.dot(incomingDir);
			normal.mul(2.0f * c1, reflected);
			reflected.add(incomingDir);
		}
	};

	struct RayShot;
	struct RayShotHit;

	class Material {
	public:

		Material() 
			: mDiffuse(1.0f), mReflection(0.0f), 
			mRefraction(0.0f), mRefractionIndex(1.0f), 
			mSpecular(0.0f), mSpecularPower(40.0f),
			mIsShadowCaster(true) {

			mColor.set(1.0f);
		}

		inline float& specularPower() { return mSpecularPower; }
		inline float& specular() { return mSpecular; }
		inline float& diffuse() { return mDiffuse; }
		inline float& reflection() { return mReflection; }
		inline float& refraction() { return mRefraction; }
		inline float& refractionIndex() { return mRefractionIndex; }
		
		inline bool& isShadowCaster() { return mIsShadowCaster; }
		inline bool lightProducesDiffuse() { return diffuse() > 0.0f; }
		inline bool lightProducesSpecular() { return specular() > 0.0f; }

		inline Vector3& color() { return mColor; }

	protected:

		bool mIsShadowCaster;

		float mDiffuse;
		float mReflection;
		float mSpecular;
		float mSpecularPower;
		float mRefraction;
		float mRefractionIndex;
		
		Vector3 mColor;
	};

	typedef WETL::CountedPtrArrayEx<Material, size_t> Materials;

	struct Vertex : Vector3 {
	};

	struct Triangle {

		unsigned int vertices[3];
		Vector3 normal;

		void autoGenNormal(const Vertex* vertices, bool rotLeftHanded);
	};

	class Primitive {
	public:

		#ifdef _DEBUG
			static unsigned int kCounter;
			unsigned int ID;
		#endif

	public:

		Primitive() {
		
			#ifdef _DEBUG
				ID = kCounter++;
			#endif
		}

		inline void setMaterial(Material* pMaterial) { mMaterial = pMaterial; }
		inline Material& material() { return mMaterial; }
		
		virtual bool intersects(const RayShot& rayShot, RayIntersectionT& intersection) = 0;
		virtual void resolveIntersectionInfo(const RayShot& rayShot, RayShotHit& hit)  = 0;

		virtual const Vector3& getPosition()  = 0;

		virtual void appendToVolume(AAB& volume) = 0;
		virtual PlanePartitionResult classify(AAPlane& plane, float planeThickness) = 0;
		//choose best of left or right even if the plane is crossed
		virtual PlanePartitionResult chooseClassify(AAPlane& plane, float planeThickness) {

			assert(false);
			return PP_None;
		}

	protected:

		SoftPtr<Material> mMaterial;
	};


	struct RayShot {

		const RayTolerance* rayTolerance;
		const Ray* ray;
		Primitive* pOriginPrimitive;

		inline RayShot() {}

		inline RayShot(const Ray& ray, Primitive* pOriginPrimitive, const RayTolerance& rayTolerance) {
		
			this->ray = &ray;
			this->rayTolerance = &rayTolerance;
			this->pOriginPrimitive = pOriginPrimitive;

		}
	};

	struct RayShotHit {

		unsigned int primID;
		RayIntersectionT inters;
		RayIntersection intersInfo;

		inline void reset() {

			primID = -1;
			inters.reset();
		}

		inline void resolvePoint(const Ray& ray) {
			
			ray.getParametricPoint(inters.t, intersInfo.point);
		}

		inline bool hasPrimitive() { return primID != -1; }
	};

	#ifdef RayTracerConfig_EnablePackets

		struct Packet4 {

			union { float el[4]; __m128 el4; int eli[4]; __m128i eli4; };

			__forceinline Packet4() {}
			__forceinline Packet4(const __m128& val) { el4 = val; }
			__forceinline Packet4(const __m128i& val) { eli4 = val; }
			__forceinline Packet4(const float val) { el4 = _mm_set1_ps(val); }
			__forceinline Packet4(const int val) { eli4 = _mm_set1_epi32(val); }

			__forceinline operator __m128&() { return el4; } 
			__forceinline operator __m128i&() { return eli4; } 

			__forceinline operator const __m128&() const { return el4; } 
			__forceinline operator const __m128i&() const { return eli4; } 
		};

	#endif

	struct Constants {
	
		const float infinity;

		#ifdef RayTracerConfig_EnablePackets
			const Packet4 zero4;
			const Packet4 one4;
			const Packet4 infinity4;
			const Packet4 fullMask4;
			const Packet4 onei4;

			struct MaskInfo {
				short first;
				short count;
			};

			static const MaskInfo MaskBitInfoTable[16];

		#endif

		Constants() 
			: infinity(std::numeric_limits<float>::infinity())
			#ifdef RayTracerConfig_EnablePackets
			,
			zero4(0.0f), 
			fullMask4(_mm_cmpeq_ps(zero4.el4, zero4.el4)), 
			one4(1.0f),
			onei4(1),
			infinity4(std::numeric_limits<float>::infinity()) 
			#endif
		{
		}

		static const Constants kConstants;
	};

	#ifdef RayTracerConfig_EnablePackets

		static __forceinline __m128 _mm_safercp( const __m128 v )
		{
			const __m128 nr = _mm_rcp_ps( v );
			const __m128 muls = _mm_mul_ps( _mm_mul_ps( nr, nr ), v );
			return _mm_sub_ps( _mm_add_ps( nr, nr ), _mm_andnot_ps( _mm_cmpeq_ps( v, _mm_set1_ps( 0 ) ), muls ) ); 
		}

		struct Packet4_3 {

			union {

				struct {
					union { float x[4]; __m128 x4; int xi[4]; __m128i xi4; };
					union { float y[4]; __m128 y4; int yi[4]; __m128i yi4; };
					union { float z[4]; __m128 z4; int zi[4]; __m128i zi4; };
				};

				struct {

					__m128 el4[3];
				};

				struct {

					__m128i eli4[3];
				};

				struct {

					float el[3][4];
				};

				struct {

					int eli[3][4];
				};
			};

			__forceinline const __m128& operator[](const int dim) const {

				return el4[dim];
				//return ((const __m128*) &x4)[dim];
				//return (dim == 0 ? x4 : (dim == 1 ? y4 : z4));
			}

			__forceinline __m128& operator[](const int dim) {

				return el4[dim];
				//return ((const __m128*) &x4)[dim];
				//return (dim == 0 ? x4 : (dim == 1 ? y4 : z4));
			}

			__forceinline const float& del(const int dim, const int idx) const {

				return el[dim][idx];
				//return (x + dim)[idx];
			}

			__forceinline const int& deli(const int dim, const int idx) const {

				return eli[dim][idx];
				//return (xi + dim)[idx];
			}
		};

		struct PacketRayShot {

			Packet4_3 ray4_origin;
			Packet4_3 ray4_direction;
			Packet4_3 ray4_rdirection;
			//Packet4_3 ray4_sign;

			const RayTolerance* rayTolerance;
			Primitive* pOriginPrimitive;

			void setRayDir(const int ri, const Vector3& dir) {

				ray4_direction.x[ri] = dir.x;
				ray4_direction.y[ri] = dir.y;
				ray4_direction.z[ri] = dir.z;
			}

			void setRayOrigin4(const Vector3& origin) {

				ray4_origin.x4 = _mm_set1_ps(origin.x);
				ray4_origin.y4 = _mm_set1_ps(origin.y);
				ray4_origin.z4 = _mm_set1_ps(origin.z);
			}

			inline void scaleDirection4(__m128 scale) {

				ray4_direction.x4 = _mm_mul_ps(ray4_direction.x4, scale);
				ray4_direction.y4 = _mm_mul_ps(ray4_direction.y4, scale);
				ray4_direction.z4 = _mm_mul_ps(ray4_direction.z4, scale);
			}

			void prepareRays(const Constants& cts) {

				 __m128 recpmag4 = _mm_rsqrt_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(ray4_direction.x4, ray4_direction.x4), 
								_mm_mul_ps(ray4_direction.y4, ray4_direction.y4)), 
								_mm_mul_ps(ray4_direction.z4, ray4_direction.z4)));


				 scaleDirection4(recpmag4);

				 {
					ray4_rdirection.x4 = _mm_safercp(ray4_direction.x4);
					ray4_rdirection.y4 = _mm_safercp(ray4_direction.y4);
					ray4_rdirection.z4 = _mm_safercp(ray4_direction.z4);
				 }

				 /*
				 {
					//use _mm_movemask_ps ?
					{
						ray4_sign.x4 = _mm_and_ps(_mm_cmplt_ps(ray4_rdirection.x4, cts.zero4), cts.onei4.el4);
						ray4_sign.y4 = _mm_and_ps(_mm_cmplt_ps(ray4_rdirection.y4, cts.zero4), cts.onei4.el4);
						ray4_sign.z4 = _mm_and_ps(_mm_cmplt_ps(ray4_rdirection.z4, cts.zero4), cts.onei4.el4);
					 }
				 }
				 */
			}

			inline PacketRayShot() {}

			inline PacketRayShot(Primitive* pOriginPrimitive, const RayTolerance& rayTolerance) {
			
				this->rayTolerance = &rayTolerance;
				this->pOriginPrimitive = pOriginPrimitive;

			}
		};


		struct PacketRayHit {

			Packet4 t;
			Packet4 pid;
			int hitBitMask;
			
			inline void reset(const Constants& cts) {
		
				pid.eli4 = _mm_set1_epi32((int) -1);
				t.el4 = cts.infinity4;
			}

			inline int getHitMask(const Constants& cts) {

				return _mm_movemask_ps(Packet4(_mm_cmpeq_epi32(pid.eli4, cts.zero4.eli4)).el4);
			}

			inline int getHitCount(const Constants& cts) {

				return Constants::MaskBitInfoTable[getHitMask(cts)].count;
			}
		};

		struct PacketRayMask {

			Packet4 active;
			//Constants::MaskInfo info;

			PacketRayMask() {}
			inline PacketRayMask(const Constants& cts) { reset(cts); }

			inline void reset(const Constants& cts) {
		
				active.el4 = cts.fullMask4;
				//info.first = 0;
				//info.count = 4;
			}

			inline void setActiveMask(const __m128& newMask) {

				active.el4 = newMask;
				//int bits = _mm_movemask_ps(active.el4);
				//info = Constants::MaskBitInfoTable[bits];
				/*
				activeCount = 0;
				firstActive = -1;

				//TODO use table
				if (bits & 8) { ++activeCount; firstActive = 3; }
				if (bits & 4) { ++activeCount; firstActive = 2; }
				if (bits & 2) { ++activeCount; firstActive = 1; }
				if (bits & 1) { ++activeCount; firstActive = 0; }
				*/
			}

			inline void setActiveMask(const int& newMask) {

				const int fullMask = (int) ((unsigned int) -1);

				//pretty slow?!?
				//active.eli4 = _mm_set_epi32(newMask & 8 ? fullMask : 0, newMask & 4 ? fullMask : 0, newMask & 2 ? fullMask : 0, newMask & 1 ? fullMask : 0) ;
				active.eli4 = _mm_set_epi32((newMask & 8) * fullMask, (newMask & 4) * fullMask, (newMask & 2) * fullMask, (newMask & 1) * fullMask) ;
			}

			inline int extractMask() const {

				return _mm_movemask_ps(active.el4);
			}

			inline short getActiveCount() const {

				return Constants::MaskBitInfoTable[_mm_movemask_ps(active.el4)].count;
				//return info.count;
			}

			inline short getFirst() const {

				return Constants::MaskBitInfoTable[_mm_movemask_ps(active.el4)].first;
				//return info.first;
			}

			static const PacketRayMask kFullMask;
		};


	#endif
}

#endif