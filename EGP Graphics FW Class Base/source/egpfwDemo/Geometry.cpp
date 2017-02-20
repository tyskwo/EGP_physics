//  David Hartman 0923439
//  Ty Wood       0901793
//
//  EGP 425.01
//  Simple Particle Engine
//  17.02.05
//
//  We certify that this work is entirely our own.
//  The assessor of this project may reproduce this project and provide copies to other academic staff,
//  and/or communicate a copy of this project to a plagiarism-checking service,
//  which may retain a copy of the project on its database.





#include "Geometry.h"





// functions to return data of octahedron
// written by: Ty
const float        *Octahedron::getPositions()   { return Octahedron::Positions;  }
const float        *Octahedron::getColors()      { return Octahedron::Colors;     }
const unsigned int *Octahedron::getIndeces()     { return Octahedron::IndexOrder; }
      unsigned int  Octahedron::getVertexCount() { return Octahedron::Vertices;   }
      unsigned int  Octahedron::getIndexCount()  { return Octahedron::Indices;    }





// functions to return data of isocahedron
// written by: Ty
const float        *Isocahedron::getPositions()   { return Isocahedron::Positions;  }
const float        *Isocahedron::getColors()      { return Isocahedron::Colors;     }
const float        *Isocahedron::getNormals()     { return Isocahedron::Normals;    }
const unsigned int *Isocahedron::getIndeces()     { return Isocahedron::IndexOrder; }
      unsigned int  Isocahedron::getVertexCount() { return Isocahedron::Vertices;   }
      unsigned int  Isocahedron::getIndexCount()  { return Isocahedron::Indices;    }
