#include "Grid.h"

Grid::Grid(std::vector<Object*> objs, std::vector<LightSources*> lights) :
	AccelerationStructure(objs) {
	// determine the bounds of the grid by iterating
	// thru all scene objects and expanding its bounds accordingly
	// USE extendBy for each object iterated thru
	for (int i = 0; i < objects.size(); ++i) {
		// keeps enlarging the grid until it perfectly 
		// encapsulates every geometry
		/*if (objects[i]->bbox.min.x == -FLT_MAX || objects[i]->bbox.max.x == FLT_MAX) continue;
		if (objects[i]->bbox.min.z == FLT_MAX || objects[i]->bbox.max.z == -FLT_MAX) continue;*/
		if (typeid(*objects[i]) == typeid(Plane)) continue;
		gridBbox.extendBy(objects[i]->bbox.minBounds);
		gridBbox.extendBy(objects[i]->bbox.maxBounds);
		
		// print out the values
		std::cout << "current Object Bbox: " << std::endl;
		std::cout << objects[i]->bbox.minBounds.x << " " << objects[i]->bbox.minBounds.y << " " << objects[i]->bbox.minBounds.z << " " << std::endl;
		std::cout << objects[i]->bbox.maxBounds.x << " " << objects[i]->bbox.maxBounds.y << " " << objects[i]->bbox.maxBounds.z << " " << std::endl;
		std::cout << "grid Bbox Object: " << std::endl;
		std::cout << gridBbox.minBounds.x << " " << gridBbox.minBounds.y << " " << gridBbox.minBounds.z << " " << std::endl;
		std::cout << gridBbox.maxBounds.x << " " << gridBbox.maxBounds.y << " " << gridBbox.maxBounds.z << " " << std::endl;
	}

	// 1. Determining grid size
	// 2. Determine the sub-division
	// dimensions by finding cellDimensions, resolution, size, 
	//glm::vec3 diff(gridBbox.maxBounds.x - gridBbox.minBounds.x, gridBbox.minBounds.y - gridBbox.maxBounds.y, gridBbox.minBounds.z - gridBbox.maxBounds.z);
	glm::vec3 diff = gridBbox.maxBounds - gridBbox.minBounds;
	glm::vec3 size;
	int resolution[3];
	int lambda = 5;
	for (int j = 0; j < 3; ++j) {
		size[j] = fabsf(diff[j]); 
		resolution[j] = 5;
	}
	glm::vec3 cellSize(size.x / resolution[0], size.y / resolution[1], size.z / resolution[2]);
	// We won't use the formula for resolution now since we haven't included polygonal meshes yet
	// since we need N, the # of tris in each cell
	//resolution[0] =  floor(size.x * pow(lambda * objects.size() * (1.0f / (size.x * size.y * size.z)), 1.0f / 3.0f));
	//resolution[1] = floor(size.y * pow(lambda * objects.size() * (1.0f / (size.x * size.y * size.z)), 1.0f / 3.0f));
	//resolution[2] = floor(size.z * pow(lambda * objects.size() * (1.0f / (size.x * size.y * size.z)), 1.0f / 3.0f));

	// create a 1-D cell array with resx * resy * resz elements
	numCells = resolution[0] * resolution[1] * resolution[2];
	cells = new Grid::Cell* [numCells];
	
	// memset them all to nullptr for now
	memset(cells, 0x0, sizeof(Grid::Cell*) * numCells);

	// populate the cells with triangles/objects 
	// the ones with no objects in theirs cells remain nullptr
	
	// find the cell coords for xmin and xmax (y & z also)
	// loop thru all the objects and insert each one into the grid
	// based on their max and min bounding points
	int totalItems = 0;
	for (int k = 0; k < objects.size(); ++k) {
		//totalItems++;
		// find the x/y/z indices 
		// subtract minimum of object Bbox with min of grid's Bbox
		// and divide by cellSize x/y/z to get the "normalized" value,
		// and floor to convert to the corresp cell index where the min/max
		// point resides in
		glm::vec3 minDiff = (objects[k]->bbox.minBounds - gridBbox.minBounds);
		glm::vec3 maxDiff = (objects[k]->bbox.maxBounds - gridBbox.minBounds);
		
		// since z is always negative and we want a +ve 
		// index, we negate for z. 
		// Obtain cell coordinates for each object
		int zmin = clamp(floor(minDiff.z / cellSize.z), 0, resolution[2] - 1);
		int zmax = clamp(floor(maxDiff.z / cellSize.z), 0, resolution[2] - 1);
		int ymin = clamp(floor(minDiff.y / cellSize.y), 0, resolution[1] - 1);
		int ymax = clamp(floor(maxDiff.y / cellSize.y), 0, resolution[1] - 1);
		int xmin = clamp(floor(minDiff.x / cellSize.x), 0, resolution[0] - 1);
		int xmax = clamp(floor(maxDiff.x / cellSize.x), 0, resolution[0] - 1);
		// loop thru cells to insert object inside them
		for (int z = zmin; z <= zmax; ++z) {
			for (int y = ymin; y <= ymax; ++y) {
				for (int x = xmin; x <= xmax; ++x) {
					// get the current grid idx -- its 1-dimensional
					// each z index has traversed x*y elems, same idea for y etc...
					int curIdx = z * resolution[0] * resolution[1] + y * resolution[0] + x;
					// cells with objects are not null
					cells[curIdx] = new Cell;
					// insert object into Cell object array
					// cells[curIdx]
					cells[curIdx]->insertObject(objects[k]);
					std::cout << curIdx << std::endl;

				}
			}
		}

	}
	std::vector<Object*> objPtrs;
	std::cout << numCells << std::endl;
	//int totalItems = 0;
	for (int m = 0; m < numCells; ++m) {
		if (cells[m] != nullptr) {
			for (int a = 0; a < cells[m]->cellObjects.size(); ++a) {
				//std::cout << cells[m]->cellObjects[a] << std::endl;
				std::cout << cells[m]->cellObjects.size() << std::endl;
				totalItems += cells[m]->cellObjects.size();
				//bool equal = false;
				//if (objPtrs.size() == 0) {
				//	objPtrs.push_back(cells[m]->cellObjects[a]);
				//	totalItems++;
				//}
				//else {
				//	for (int h = 0; h < objPtrs.size(); ++h) {
				//		if (objPtrs[h] == cells[m]->cellObjects[a]) equal = true;
				//	}
				//	if (!equal) {
				//		objPtrs.push_back(cells[m]->cellObjects[a]);

				//		//totalItems++;
				//	}
				//}
			}
		}
	}
	std::cout << "total object count is " << totalItems << std::endl;
}

// frees the array of pointers to Cell objects, then finally frees
// pointer to the array of Cell pointers
Grid::~Grid() {
	for (int i = 0; i < resolution[0] * resolution[1] * resolution[2]; ++i) {
		delete cells[i];
	}
	delete cells;
}


int Grid::clamp(const int& lo, const int& hi, const int& v) {
	return std::max(lo, std::min(hi, v));
}

bool Grid::intersect(glm::vec3 orig, glm::vec3 dir, std::vector<Object*>& objects, float& tNear, int& objIndex, glm::vec2& uv, Object* hitObj)
{
	// prep the deltaT, nextCrossingT, initial x/y/z, 
	// find the starting cell coordinates using rayOrigGrid 
	// rayOriginGrid = rayOrig - gridMin; 
	// You might need to change your min max point bounds, or alter your code 
	
	// rayOriginGrid 
	//glm::vec3 rayOriginGrid = 
	// set up the neccessary index for bounds, exit, and cell, 
	// and set up parametric distance variables
	glm::vec3 step, exit, rayOriginGrid;
	glm::vec3 nextCrossingT, deltaT;
	
	// Check if ray intersects Grid's Bounding Box
	

	// loop thru all 3 axes to initialize each respective component
	//for (int i = 0; i < NUM_AXES; ++i) {
	//	rayOriginGrid[i] = 

	//
	//}


	return false;
}



