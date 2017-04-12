/*
Nexus

Copyright(C) 2012 - Federico Ponchio
ISTI - Italian National Research Council - Visual Computing Lab

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License (http://www.gnu.org/licenses/gpl.txt)
for more details.
*/
#ifndef NXZ_ENCODER_H
#define NXZ_ENCODER_H

#include <vector>
#include <map>

#include <limits.h>
#include <float.h>

#include "cstream.h"
#include "zpoint.h"
#include "index_attribute.h"
#include "vertex_attribute.h"
#include "color_attribute.h"
#include "normal_attribute.h"

namespace crt {

class Encoder {
public:

	uint32_t nvert, nface;
	std::map<std::string, std::string> exif; //mtllib ...,

	IndexAttribute index;
	std::map<std::string, VertexAttribute *> data;
	int header_size;

	Stream stream;

	Encoder(uint32_t _nvert, uint32_t _nface = 0, Stream::Entropy entropy = Stream::TUNSTALL);



	bool addPositions(float *buffer, float q = 0.0f, Point3f o = Point3f(0.0f));
	bool addPositions(float *buffer, uint32_t *index, float q = 0.0f, Point3f o = Point3f(0.0f));
	bool addPositions(float *buffer, uint16_t *index, float q = 0.0f, Point3f o = Point3f(0.0f));

	bool addPositionsBits(float *buffer, int bits);
	bool addPositionsBits(float *buffer, uint32_t *index, int bits);
	bool addPositionsBits(float *buffer, uint16_t *index, int bits);

	bool addNormals(float *buffer, int bits, NormalAttr::Prediction no = NormalAttr::ESTIMATED);
	bool addNormals(int16_t *buffer, int bits, NormalAttr::Prediction no = NormalAttr::ESTIMATED);

	bool addColors(unsigned char *buffer, int rbits = 6, int gbits = 7, int bbits = 6, int abits = 5);

	bool addUvs(float *buffer, float q = 0);

	bool addAttribute(const char *name, char *buffer, VertexAttribute::Format format, int components, float q, uint32_t strategy = 0);
	//its your job to fill attr variables appropriately (type and number of components in particular.
	bool addAttribute(const char *name, char *buffer, VertexAttribute *attr);

	void addGroup(int end_triangle) { index.groups.push_back(Group(end_triangle)); }
	void addGroup(int end_triangle, std::map<std::string, std::string> &props) {
		Group g(end_triangle);
		g.properties = props;
		index.groups.push_back(g);
	}

	void encode();

private:
	uint32_t current_vertex;
	uint32_t last_index; //moved here so that it works across groups


	std::vector<bool> boundary;
	std::vector<int> encoded;    //encoded vertex number
	std::vector<Quad> prediction;

	void encodePointCloud();

	void encodeMesh();
	void encodeFaces(int start, int end);
};

} //namespace

#endif // NXZ_ENCODER_H