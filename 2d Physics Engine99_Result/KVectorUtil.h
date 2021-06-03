#pragma once
#include <windows.h>
#include "KVector2.h"
#include "KBasis2.h"
#include "KScreenCoordinate.h"
#include <vector>

namespace KVectorUtil
{
    extern KScreenCoordinate    g_screenCoordinate;
    extern KBasis2  g_basis2;

    void SetScreenCoordinate(const KScreenCoordinate& screenCoord);
    void SetBasis2(const KBasis2& basis2);
    KVector2 ScreenToWorld(const KVector2& v0_);
	KVector2 WorldToScreen(const KVector2& v0);
	void DrawLine(HDC hdc, const KVector2& v0, const KVector2& v1
        , int lineWidth = 1, int penStyle = PS_SOLID, COLORREF color = RGB(0,0,0) );
	void DrawLine(HDC hdc, const std::vector<KVector2>& points
		, int lineWidth, int penStyle, COLORREF color, int strip0loop1);
	void DrawArrow(HDC hdc, const KVector2& v0, const KVector2& v1, float tipLength = 0.1f
		, int lineWidth = 1, int penStyle = PS_SOLID, COLORREF color = RGB(0, 0, 0));
    void DrawAxis(HDC hdc, int numHorizontalGrid, int numVerticalGrid
		, COLORREF color1 = RGB(64, 64, 64), COLORREF color2 = RGB(64, 64, 64));
    void DrawGrid(HDC hdc, int numHorizontalGrid, int numVerticalGrid, COLORREF color = RGB(64, 64, 64));
    void DrawCircle(HDC hdc, const KVector2& center, float radius, int numSegment
        , int lineWidth = 1, int penStyle = PS_SOLID, COLORREF color = RGB(0, 0, 0));
    float LengthSquared(const KVector2& a, const KVector2& b);
    float Length(const KVector2& a, const KVector2& b);
	float PointLinesegmentDistance(KVector2 p, KVector2 v, KVector2 w);
	float PointLineDistance(KVector2 p, KVector2 v0, KVector2 v1);
	/// check whether point p is in the convex polygon of 'points'
	bool IsPointInPolygon(const KVector2& p, const std::vector<KVector2>& points);
	bool IsPointInPolygon(const KVector2& p, const KVector2* points, int numPoints);
	bool LineSegementsIntersect(KVector2 p0, KVector2 p1, KVector2 q0, KVector2 q1
		, KVector2& out, bool considerCollinearOverlapAsIntersect = false);
	int LineSegmentPolygonIntersection(const KVector2& p0, const KVector2& p1, const std::vector<KVector2>& points);
    KVector2 GetGeoCenter(const KVector2* points, int vertexCount);
    KVector2 GetGeoCenter(const std::vector<KVector2>& points);
	void Clip(const std::vector<KVector2>& points, const KVector2 p0, const KVector2 p1
		, std::vector<KVector2>& new_points);
	void DrawPolygon(HDC hdc, std::vector<KVector2>& points, COLORREF color);
}
