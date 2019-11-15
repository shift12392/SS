#pragma once

#include <math.h>
#include <assert.h>



namespace acg
{
	namespace Math
	{
		struct Vector
		{
		public:
			float X;
			float Y;
			float Z;

			Vector() {}

			explicit Vector(float InitValue) :X(InitValue), Y(InitValue), Z(InitValue)
			{
				AssertCheckNan();
			}

			Vector(float InitX, float InitY, float InitZ) :X(InitX), Y(InitY), Z(InitZ)
			{
				AssertCheckNan();
			}
#if defined(DEBUG) || defined(_DEBUG)
			inline Vector& operator = (const Vector &B) const
			{
				X = B.X;
				Y = B.Y;
				Z = B.Z;
				AssertCheckNan();
				return *this;
			}

			Vector(const Vector& B)
			{
				X = B.X;
				Y = B.Y;
				Z = B.Z;
				AssertCheckNan();
			}
#endif


			inline bool operator == (const Vector &B) const
			{
				return X == B.X && Y == B.Y && Z == B.Z;
			}
			inline Vector operator + (const Vector &B) const
			{
				return Vector(X + B.X, Y + B.Y, Z + B.Z);
			}

			inline Vector operator += (const Vector &B)
			{
				X += B.X;
				Y += B.Y;
				Z += B.Z;
				AssertCheckNan();
				return *this;
			}

			inline Vector operator - (const Vector &B) const
			{
				return Vector(X - B.X, Y - B.Y, Z - B.Z);
			}

			inline Vector operator -= (const Vector &B)
			{
				X -= B.X;
				Y -= B.Y;
				Z -= B.Z;
				AssertCheckNan();
				return *this;
			}

			inline Vector operator * (const Vector &B) const
			{
				return Vector(X * B.X, Y * B.Y, Z * B.Z);
			}

			inline Vector operator / (const Vector &B) const
			{
				return Vector(X / B.X, Y / B.Y, Z / B.Z);
			}

			inline Vector operator / (float B) const
			{
				return Vector(X / B, Y / B, Z / B);
			}

			inline Vector operator /= (float B)
			{
				X /= B;
				Y /= B;
				Z /= B;
				AssertCheckNan();
				return *this;
			}

			inline bool Normalize(float Tolerance = SMALL_NUMBER)
			{
				const float LenghtSquared = X * X + Y * Y + Z * Z;
				if(LenghtSquared > Tolerance)
				{
					const float InvLenght = 1.0f / sqrtf(LenghtSquared);
					//const double InvLenght2 = 1.0f / sqrt(LenghtSquared);
					//const float InvLenght2 = powf(LenghtSquared, -0.5);
					//const float Scale = FMath::InvSqrt(LenghtSquared);
					X *= InvLenght;
					Y *= InvLenght;
					Z *= InvLenght;
					return true;
				}

				return false;
			}

			inline void NormallizeNoSafe()
			{
				float InvLenght = 1.0f / Lenght();
				X *= InvLenght;
				Y *= InvLenght;
				Z *= InvLenght;
			}

			inline float DotProduct(Vector B) const
			{
				return X * B.X + Y * B.Y + Z * B.Z;
			}



			inline float LenghtSquared() const
			{
				return X * X + Y * Y + Z * Z;
			}


			inline float Lenght() const
			{
				return sqrtf(LenghtSquared());
			}

			inline bool CheckNan() const
			{
				return isnan(X) || isnan(Z) || isnan(Z);
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


		struct Vector4
		{
		public:
			float X;
			float Y;
			float Z;
			float W;

			Vector4() {}

			explicit Vector4(float InitValue) :X(InitValue), Y(InitValue), Z(InitValue),W(InitValue)
			{
				AssertCheckNan();
			}

			Vector4(float InitX, float InitY, float InitZ,float InitW) :X(InitX), Y(InitY), Z(InitZ),W(InitW)
			{
				AssertCheckNan();
			}
#if defined(DEBUG) || defined(_DEBUG)
			inline Vector4& operator = (const Vector4 &B) const
			{
				X = B.X;
				Y = B.Y;
				Z = B.Z;
				W = B.W;
				AssertCheckNan();
				return *this;
			}

			Vector4(const Vector4& B)
			{
				X = B.X;
				Y = B.Y;
				Z = B.Z;
				W = B.W;
				AssertCheckNan();
			}
#endif


			inline bool operator == (const Vector4 &B) const
			{
				return X == B.X && Y == B.Y && Z == B.Z && W == B.W;
			}
			inline Vector4 operator + (const Vector4 &B) const
			{
				return Vector4(X + B.X, Y + B.Y, Z + B.Z,W + B.W);
			}

			inline Vector4 operator += (const Vector4 &B)
			{
				X += B.X;
				Y += B.Y;
				Z += B.Z;
				W += B.W;
				AssertCheckNan();
				return *this;
			}

			inline Vector4 operator - (const Vector4 &B) const
			{
				return Vector4(X - B.X, Y - B.Y, Z - B.Z,W - B.W);
			}

			inline Vector4 operator -= (const Vector4 &B)
			{
				X -= B.X;
				Y -= B.Y;
				Z -= B.Z;
				W -= B.W;
				AssertCheckNan();
				return *this;
			}

			inline Vector4 operator * (const Vector4 &B) const
			{
				return Vector4(X * B.X, Y * B.Y, Z * B.Z,W*B.W);
			}

			inline Vector4 operator *= (const Vector4 &B)
			{
				X *= B.X;
				Y *= B.Y;
				Z *= B.Z;
				W *= B.W;
				AssertCheckNan();
				return *this;
			}

			inline Vector4 operator / (const Vector4 &B) const
			{
				return Vector4(X / B.X, Y / B.Y, Z / B.Z,W/B.W);
			}

			inline float DotProduct(Vector4 B) const
			{
				return X * B.X + Y * B.Y + Z * B.Z + W + B.W;
			}



			inline float LenghtSquared() const
			{
				return X * X + Y * Y + Z * Z + W*W;
			}


			inline float Lenght() const
			{
				return sqrt(LenghtSquared());
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

