#!/usr/bin/python
import pyopenvdb as vdb
import pymeshpotato.mpmesh as mepo

# Temporary solution.  Will replace by having plugins loaded dynamically
import pymeshpotato.pyOBJInputPlugin as obj

print "Reading ajax model"
imesh = obj.OBJInputMesh()
imesh.loadMesh("/home/kcoley/projects/MeshPotato/examples/testOBJ/ajax.obj")
print "Done reading mesh"

print "Printing number of vertices"
print imesh.getNumberVertices()
print "Done!"


#grid = vdb.read('/home/kcoley/projects/MeshPotato/examples/testOBJ/ajax.vdb', '')
#grid = mepo.processGrid(grid)
#print "Writing new mesh as mepoGrid.vdb"
#vdb.write('newmepoGrid.vdb', [grid])
