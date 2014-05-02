Surface-Netgen-Fork
===================

Gustave Granroth 05/02/2014

Short
-----

A triangle-only surface mesh-only fork of Netgen which is optimized for use in real-time 3D graphics applications. With a small CSG mesh, defining the CSG, meshing, and drawing can perform at > 60 fps.

Rationale for the Fork
----------------------

I originally wanted to use the functionality provided from Nglib as part of [Netgen](http://sourceforge.net/apps/mediawiki/netgen-mesher/index.php?title=Main_Page) to do realtime CSG operations and produce a triangle surface mesh for rendering with OpenGL. However, this library does not:

    * Include access to the CSG functionality.
    * Provide for surface mesh generation from a CSG operation.
and does

    * Include many, many extra features, such as tetrahedral volumetric mesh generation.

Because my intended application of Netgen -- realtime 3D graphics/games -- is not the same as most users of Netgen -- Finite Element Analysis or Computational Fluid Dynamics -- I forked the library to remove features I would not be using, attempt to improve performance, and gain access to the functionality buried inside Netgen.

Changes from Netgen
-------------------

    * Only triangle surface meshing from a series of C++ commands or from a .geo file are available.
    * Directory structure has been flattened. 
    * Some early optimization attempts have been made.
    * Access to the geometry specification / mesh generation has been rewritten for ease of access.
    
Example Usage
-------------

**File loading and mesh generation with default meshing parameters**

*From a .geo file*
```C++
netgen::Mesh *pMesh = NULL; // Resulting triangle surface mesh

std::string filename ("scripts/ngdemo.geo");
netgen::CSGeometryRegister regr;
netgen::CSGeometry *pGeom = regr.Load(filename);

netgen::MeshingParameters mParams;
pGeom->GenerateMesh(pMesh, mParams);

delete pGeom; // Done with the geometry
```

*In the C++ code*
```C++
netgen::CSGeometry *pGeom = new netgen::CSGeometry;

// Define a cube    
netgen::Point<3> pa = netgen::Point<3>(0, 0, 0);
netgen::Point<3> pb = netgen::Point<3>(1, 1, 1);
netgen::Primitive *ncube = new netgen::OrthoBrick(pa, pb);
pGeom->AddSurfaces(ncube);
netgen::Solid *cube = new netgen::Solid(ncube);

// Define a cylinder
netgen::Point<3> pac = netgen::Point<3>(0.5, sidePos, -1.1);
netgen::Point<3> pbc = netgen::Point<3>(0.5, sidePos, 1.1);
netgen::OneSurfacePrimitive *surf = new netgen::Cylinder(pac, pbc, 0.3);
pGeom->AddSurfaces(surf);
netgen::Solid *cyl = new netgen::Solid(surf);

// Yet another cylinder
netgen::Point<3> pad = netgen::Point<3>(0.5, -1.1, sidePos);
netgen::Point<3> pbd = netgen::Point<3>(0.5, 1.1, sidePos);
netgen::OneSurfacePrimitive *surfr = new netgen::Cylinder(pad, pbd, 0.2);
pGeom->AddSurfaces(surfr);
netgen::Solid *cylr = new netgen::Solid(surfr);

// Subtact the cylinders from the cube
netgen::Solid *cylinder = new netgen::Solid(netgen::Solid::SUB, cyl);
netgen::Solid  *solid = new netgen::Solid(netgen::Solid::SECTION, new  netgen::Solid(netgen::Solid::SECTION, cube, cylinder), new netgen::Solid(netgen::Solid::SUB, cylr));

// Setup the top level object for meshing
netgen::Flags flags;
pGeom->SetSolid("Root", new netgen::Solid(netgen::Solid::ROOT, solid));
pGeom->SetTopLevelObject(solid);
pGeom->SetFlags("Root", flags);
pGeom->FindIdenticSurfaces(1e-8*pGeom->MaxSize()); // Initializes surfaces

// Generate the mesh
netgen::Mesh *pMesh = NULL;
netgen::MeshingParameters mParams;
pGeom->GenerateMesh(pMesh, mParams);

delete pGeom; // Done with the geometry.
```

For a full example, see this library as used in the Volumetric-CSG-Texture-Mapping application, available on [GitHub](https://github.com/GuMiner/Volumetric-CSG-Texture-Mapping).

TODO List
---------

1. There's a lot of optimization work that could be done.
2. Making this library compilable into a DLL would be nice (even though this is LGPL licensed, just like Netgen).
3. Making it easier to create basic shapes.
4. Adding more basic shapes.

Contact information
-------------------

Gustave Granroth gus.gran@gmail.com