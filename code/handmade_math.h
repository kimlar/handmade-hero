#ifndef HANDMADE_MATH_H

union v2 {
  struct {
    real32 X, Y;
  };
  real32 E[2];
};

inline v2
operator+(v2 A, v2 B) {
  v2 Result;
  Result.X = A.X + B.X;
  Result.Y = A.Y + B.Y;
  return Result;
}

inline v2
operator+=(v2 &A, v2 B) {
  A = A + B;
  return A;
}

inline v2
operator-(v2 A) {
  v2 Result;
  Result.X = -A.X;
  Result.Y = -A.Y;
  return Result;
}

inline v2
operator-(v2 A, v2 B) {
  v2 Result;
  Result.X = A.X - B.X;
  Result.Y = A.Y - B.Y;
  return Result;
}

inline v2
operator-=(v2 &A, v2 B) {
  A = A - B;
  return A;
}

inline v2
operator*(real32 A, v2 B) {
  v2 Result = {A * B.X, A * B.Y};
  return Result;
}

inline v2
operator*(v2 B, real32 A) {
  v2 Result = A * B;
  return Result;
}

inline v2
operator*=(v2 &A, real32 B) {
  A = B * A;
  return A;
}

inline real32
Inner(v2 A, v2 B) {
  real32 Result = A.X*B.X + A.Y*B.Y;
  return Result;
}

inline real32
LengthSq(v2 A) {
  real32 Result = Inner(A, A);
  return Result;
}

inline real32
Length(v2 A) {
  real32 Result = SquareRoot(LengthSq(A));
  return Result;
}

inline bool32
IsZero(v2 A) {
  bool32 Result = A.X == 0 && A.Y == 0;
  return Result;
}

struct rectangle2 {
  v2 Min;
  v2 Max;
};

inline rectangle2
RectMinMax(v2 Min, v2 Max) {
  rectangle2 Result = {};
  Result.Min = Min;
  Result.Max = Max;
}

inline rectangle2
RectCenterHalfDim(v2 Center, v2 HalfDim) {
  rectangle2 Result = {};
  Result.Min = Center - HalfDim;
  Result.Max = Center + HalfDim;
  return Result;
}

inline rectangle2
RectCenterDim(v2 Center, v2 Dim) {
  rectangle2 Result = RectCenterHalfDim(Center, 0.5f*Dim);
  return Result;
}

inline bool32
IsInRectangle(rectangle2 Rect, v2 Test) {
  bool32 Result = (Test.X >= Rect.Min.X) &&
    (Test.X <= Rect.Max.X) &&
    (Test.Y >= Rect.Min.Y) &&
    (Test.Y <= Rect.Max.Y);
  return Result;
}

#define HANDMADE_MATH_H
#endif
