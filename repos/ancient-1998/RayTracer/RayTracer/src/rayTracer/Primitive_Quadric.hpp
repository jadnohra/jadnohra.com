#ifndef _RayTracer_Primitive_Quadric_hpp
#define _RayTracer_Primitive_Quadric_hpp

#include "Primitive.hpp"

namespace rayTracer {

	class Primitive_Quadric : public Primitive {
	public:

		Primitive_Quadric() {}

		void createAsEllipsoid(const Vector3& position, const Vector3& radius) {

			float rx2 = radius.x * radius.x;
			float ry2 = radius.y * radius.y;
			float rz2 = radius.z * radius.z;

			A = 1.0f / (rx2);
			B = 1.0f / (ry2);
			C = 1.0f / (rz2);
			D = 0.0f;
			E = 0.0f;
			F = 0.0f;
			G = 0.0f;
			H = 0.0f;
			I = 0.0f;
			J = -1;

			setPosition(position);
		}

		virtual bool intersects(const RayShot& rayShot, RayIntersectionT& intersection) {

			//http://www.bmsc.washington.edu/people/merritt/graphics/quadrics.html
			//http://www.cc.gatech.edu/classes/AY2001/cs4451_summer/QuadricSurfaces.ppt

			const float& xd = rayShot.ray->direction.x;
			const float& yd = rayShot.ray->direction.y;
			const float& zd = rayShot.ray->direction.z;

			const float& xo = rayShot.ray->origin.x;
			const float& yo = rayShot.ray->origin.y;
			const float& zo = rayShot.ray->origin.z;

			float Aq =	A * (xd * xd) 
						+ B * (yd * yd) 
						+ C * (zd * zd) 
						+ D * (xd * yd) 
						+ E * (yd * zd) 
						+ F * (xd * zd); 

			float Bq =	2.0f * (
							A * xo * xd
							+ B * yo * yd
							+ C * zo * zd
						)
						+ D * (xo * yd + xd * yo) 
						+ E * (yo * zd + yd * zo)
						+ F * (xo * zd + xd * zo)
						+ G * xd
						+ H * yd
						+ I * zd;
					

			float Cq =	A * xo * xo
						+ B * yo * yo
						+ C * zo * zo
						+ D * xo * yo
						+ E * yo * zo
						+ F * xo * zo
						+ G * xo
						+ H * yo
						+ I * zo
						+ J;
			
			/*
			if (Aq == 0.0f) {

				intersection.t = -Cq / Bq;
				return true;
			}
			*/
			
			float det = (Bq * Bq) - (4.0f * Aq * Cq);

			if (det > 0.0f) {

				det = sqrtf(det);

				return PrimitiveUtil::twoPointIntersectHelper(-Bq, det, (2.0f * Aq), rayShot.pOriginPrimitive == this, intersection);
			}

			return false;
		}

		virtual void resolveIntersectionInfo(const RayShot& rayShot, RayShotHit& hit) {

			//xn = 2*A*xi + D*yi + E*zi + G
			//yn = 2*B*yi + D*xi + F*zi + H
			//z n = 2*C*zi + E*xi + F*yi + I

			hit.intersInfo.normal.x = 2.0f * A * hit.intersInfo.point.x
									+ D * hit.intersInfo.point.y
									+ E * hit.intersInfo.point.z
									+ G;

			hit.intersInfo.normal.y = 2.0f * B * hit.intersInfo.point.y
									+ D * hit.intersInfo.point.x
									+ F * hit.intersInfo.point.z
									+ H;

			hit.intersInfo.normal.z = 2.0f * C * hit.intersInfo.point.z
									+ E * hit.intersInfo.point.y
									+ F * hit.intersInfo.point.x
									+ I;


			hit.intersInfo.normal.normalize();

			if (hit.intersInfo.normal.dot(rayShot.ray->direction) > 0.0f)
				hit.intersInfo.normal.negate();
		}

		virtual const Vector3& getPosition() {

			return mPosition;
		}

	protected:

		void setPosition(const Vector3& pos) {

			mPosition = pos;

			const float& xo = pos.x;
			const float& yo = pos.y;
			const float& zo = pos.z;

			float newA = A;
			float newB = B;
			float newC = C;
			float newD = D;
			float newE = E;
			float newF = F;
			float newG = G - D*yo - F*zo - 2.0f * A*xo;
			float newH = H - D*xo - E*zo - 2.0f * B*yo;
			float newI = I - E*yo - F*xo - 2.0f * C*zo;
			float newJ = J 
						+ A * xo*xo + B * yo*yo + C * zo*zo
						+ D * xo*yo + E * yo*zo + F * xo*zo
						- (G*xo + H*yo + I*zo);

			A = newA;
			B = newB;
			C = newC;
			D = newD;
			E = newE;
			F = newF;
			G = newG;
			H = newH;
			I = newI;
			J = newJ;
							
		}

	protected:

		Vector3 mPosition;

		//Ax2 + By2 + Cz2 + Dxy + Eyz + Fxz + Gx + Hy + Iz + J = 0.
		float A;
		float B;
		float C;
		float D;
		float E;
		float F;
		float G;
		float H;
		float I;
		float J;
	};

	class Primitive_Ellipsoid : public Primitive_Quadric {
	public:

		Primitive_Ellipsoid(const Vector3& pos, const Vector3& radius) {

			createAsEllipsoid(pos, radius);
		}

		void getRadius(Vector3& radius) const {

			radius.x = sqrtf(1.0f / A);
			radius.y = sqrtf(1.0f / B);
			radius.z = sqrtf(1.0f / C);
		}

		virtual void appendToVolume(AAB& volume) {

			Vector3 radius;

			getRadius(radius);

			float maxRad = radius.el[0];
			maxRad = std::max(maxRad, radius.el[1]);
			maxRad = std::max(maxRad, radius.el[2]);

			volume.add(Sphere(mPosition, maxRad));
		}

		virtual PlanePartitionResult classify(AAPlane& plane, float planeThickness) {

			Vector3 radius;

			getRadius(radius);

			float maxRad = radius.el[0];
			maxRad = std::max(maxRad, radius.el[1]);
			maxRad = std::max(maxRad, radius.el[2]);

			return plane.classify(Sphere(mPosition, maxRad));
		}
	};
}

#endif