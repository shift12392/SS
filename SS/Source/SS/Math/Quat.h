#pragma once

#include "ACGMath.h"
#include "Vector.h"


namespace acg
{
	namespace Math
	{
		struct Quat
		{
			float X;
			float Y;
			float Z;
			float W;

			Quat() {};

			Quat(float InitX, float InitY, float InitZ, float InitW)
				:X(InitX), Y(InitY), Z(InitZ), W(InitW)
			{
				AssertCheckNan();
			}

			// 绕任意旋转轴V，旋转AngleRadians弧度。
			//V：            必须规范化
			//AngleRadians： 旋转的角度，弧度表示
			Quat(const Vector& V, float AngleRadians)
			{
				float half = 0.5f * AngleRadians;
				float sinHalf = sinf(half);
				W = cosf(half);
				X = sinHalf * V.X;
				Y = sinHalf * V.Y;
				Z = sinHalf * V.Z;
			}

#if defined(DEBUG) || defined(_DEBUG)
			Quat(const Quat& B)
				:X(B.X), Y(B.Y), Z(B.Z), W(B.W)
			{
				AssertCheckNan();
			}
			Quat& operator =(const Quat& B)
			{
				X = B.X;
				Y = B.Y;
				Z = B.Z;
				W = B.W;
				AssertCheckNan();
				return *this;
			}
#endif



			Quat operator + (const Quat& B) const
			{
				return Quat(X + B.X, Y + B.Y, Z + B.Z, W + B.W);
			}

			Quat operator += (const Quat& B)
			{
				X += B.X;
				Y += B.Y;
				Z += B.Z;
				W += B.W;
				AssertCheckNan();
				return *this;
			}

			Quat operator - (const Quat& B) const
			{
				return (Quat(X - B.X, Y - B.Y, Z - B.Z, W - B.W));
			}

			Quat operator -= (const Quat& B)
			{
				X -= B.X;
				Y -= B.Y;
				Z -= B.Z;
				W -= B.W;
				AssertCheckNan();
				return *this;
			}

			Quat operator* (const Quat& B) const
			{
				Quat Ret;
				Multiply(Ret, *this, B);
				return Ret;
			}

			static void Multiply(Quat &Out,const Quat& A, const Quat& B)
			{
				Out.X =  A.W * B.X - A.Z*B.Y + A.Y*B.Z + A.X*B.W;
				Out.Y =  A.Z * B.X + A.W*B.Y - A.X*B.Z + A.Y*B.W;
				Out.Z = -A.Y * B.X + A.X*B.Y + A.W*B.Z + A.Z*B.W;
				Out.W = -A.X * B.X - A.Y*B.Y - A.Z*B.Z + A.W*B.W;
			}

			inline Vector RotateVector(const Vector& V) const
			{

			}


			inline bool CheckNan() const
			{
				return isnan(X) || isnan(Y) || isnan(Z) || isnan(W);
			}

#if defined(DEBUG) || defined(_DEBUG)
			inline void AssertCheckNan() const
			{
				assert(!CheckNan());
			}
#else

			inline void AssertCheckNan() const {}

#endif // defined(DEBUG) || defined(_DEBUG)
		};
	}
}
