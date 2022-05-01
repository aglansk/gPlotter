#pragma once

#include <cmath>
#include <cstring>
#include <vector>
#include <cwchar>

#define toVector(s) (new std::vector<wchar_t> ((s), (s) + wcslen(s)))

struct RGBABitmapImageReference;

struct Rectangle;

struct ScatterPlotSeries;

struct ScatterPlotSettings;

struct BarPlotSeries;

struct BarPlotSettings;

struct RGBA;

struct RGBABitmap;

struct RGBABitmapImage;

struct BooleanArrayReference;

struct BooleanReference;

struct CharacterReference;

struct NumberArrayReference;

struct NumberReference;

struct StringArrayReference;

struct StringReference;

struct Chunk;

struct IHDR;

struct PHYS;

struct PNGImage;

struct ZLIBStruct;

struct LinkedListNodeStrings;

struct LinkedListStrings;

struct LinkedListNodeNumbers;

struct LinkedListNumbers;

struct LinkedListCharacters;

struct LinkedListNodeCharacters;

struct DynamicArrayNumbers;

struct RGBABitmapImageReference {
	RGBABitmapImage* image;
};

struct Rectangle {
	double x1;
	double x2;
	double y1;
	double y2;
};

struct ScatterPlotSeries {
	bool linearInterpolation;
	std::vector<wchar_t>* pointType;
	std::vector<wchar_t>* lineType;
	double lineThickness;
	std::vector<double>* xs;
	std::vector<double>* ys;
	RGBA* color;
	//Glansk
	std::vector<wchar_t>* legend;
};

struct ScatterPlotSettings {
	std::vector<ScatterPlotSeries*>* scatterPlotSeries;
	bool autoBoundaries;
	double xMax;
	double xMin;
	double yMax;
	double yMin;
	bool autoPadding;
	double xPadding;
	double yPadding;
	std::vector<wchar_t>* xLabel;
	std::vector<wchar_t>* yLabel;
	std::vector<wchar_t>* title;
	bool showGrid;
	RGBA* gridColor;
	bool xAxisAuto;
	bool xAxisTop;
	bool xAxisBottom;
	bool yAxisAuto;
	bool yAxisLeft;
	bool yAxisRight;
	double width;
	double height;
};

struct BarPlotSeries {
	std::vector<double>* ys;
	RGBA* color;
};

struct BarPlotSettings {
	double width;
	double height;
	bool autoBoundaries;
	double yMax;
	double yMin;
	bool autoPadding;
	double xPadding;
	double yPadding;
	std::vector<wchar_t>* title;
	bool showGrid;
	RGBA* gridColor;
	std::vector<BarPlotSeries*>* barPlotSeries;
	std::vector<wchar_t>* yLabel;
	bool autoColor;
	bool grayscaleAutoColor;
	bool autoSpacing;
	double groupSeparation;
	double barSeparation;
	bool autoLabels;
	std::vector<StringReference*>* xLabels;
	bool barBorder;
};

struct RGBA {
	double r;
	double g;
	double b;
	double a;
};

struct RGBABitmap {
	std::vector<RGBA*>* y;
};

struct RGBABitmapImage {
	std::vector<RGBABitmap*>* x;
};

struct BooleanArrayReference {
	std::vector<bool>* booleanArray;
};

struct BooleanReference {
	bool booleanValue;
};

struct CharacterReference {
	wchar_t characterValue;
};

struct NumberArrayReference {
	std::vector<double>* numberArray;
};

struct NumberReference {
	double numberValue;
};

struct StringArrayReference {
	std::vector<StringReference*>* stringArray;
};

struct StringReference {
	std::vector<wchar_t>* string;
};

struct Chunk {
	double length;
	std::vector<wchar_t>* type;
	std::vector<double>* data;
	double crc;
};

struct IHDR {
	double Width;
	double Height;
	double BitDepth;
	double ColourType;
	double CompressionMethod;
	double FilterMethod;
	double InterlaceMethod;
};

struct PHYS {
	double pixelsPerMeter;
};

struct PNGImage {
	std::vector<double>* signature;
	IHDR* ihdr;
	ZLIBStruct* zlibStruct;
	bool physPresent;
	PHYS* phys;
};

struct ZLIBStruct {
	double CMF;
	double CM;
	double CINFO;
	double FLG;
	double FCHECK;
	double FDICT;
	double FLEVEL;
	std::vector<double>* CompressedDataBlocks;
	double Adler32CheckValue;
};

struct LinkedListNodeStrings {
	bool end;
	std::vector<wchar_t>* value;
	LinkedListNodeStrings* next;
};

struct LinkedListStrings {
	LinkedListNodeStrings* first;
	LinkedListNodeStrings* last;
};

struct LinkedListNodeNumbers {
	LinkedListNodeNumbers* next;
	bool end;
	double value;
};

struct LinkedListNumbers {
	LinkedListNodeNumbers* first;
	LinkedListNodeNumbers* last;
};

struct LinkedListCharacters {
	LinkedListNodeCharacters* first;
	LinkedListNodeCharacters* last;
};

struct LinkedListNodeCharacters {
	bool end;
	wchar_t value;
	LinkedListNodeCharacters* next;
};

struct DynamicArrayNumbers {
	std::vector<double>* array;
	double length;
};