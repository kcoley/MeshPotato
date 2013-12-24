#!/usr/bin/python
import pyopenvdb as vdb
import pymeshpotato.mpmesh as mepo

# Temporary solution.  Will replace by having plugins loaded dynamically
import pymeshpotato.pyOBJInputPlugin as objin
import pymeshpotato.pyOBJOutputPlugin as objout
import pymeshpotato.pyVDBInputPlugin as vdbin
import pymeshpotato.pyVDBOutputPlugin as vdbout

import os

def LoadMesh(path):
	fileExtension = os.path.splitext(path)[1][1:]
	print fileExtension
	if fileExtension == "vdb":
		imesh = vdbin.VDBInputMesh()
		imesh.loadMesh(path)
		return imesh
	elif fileExtension == "obj":
		imesh = objin.OBJInputMesh()
		imesh.loadMesh(path)
		return imesh

def WriteMesh(imesh, path):
	fileExtension = os.path.splitext(path)[1][1:]
	print fileExtension
	if fileExtension == "vdb":
		omesh = vdbout.VDBOutputMesh()
		omesh.loadMesh(imesh.getVertices(), imesh.getNormals(), imesh.getFaces(), mepo.MeshSpec())
		omesh.writeMesh(path)
		return omesh
	elif fileExtension == "obj":
		omesh = objout.OBJOutputMesh()
		omesh.loadMesh(imesh.getVertices(), imesh.getNormals(), imesh.getFaces(), mepo.MeshSpec())
		omesh.writeMesh(path)
		return omesh
		

print "Example 1: ajax obj to ajax vdb"
#imesh = LoadMesh("/home/kcoley/projects/MeshPotato/examples/testOBJ/ajax.obj")
#omesh = WriteMesh(imesh, "vdbtest.vdb")
print "Done Conversion!"
imesh = LoadMesh("smoothAgripa.obj")
omesh = WriteMesh(imesh, "newagripaSmooth.vdb")
#imesh = LoadMesh("/home/kcoley/projects/MeshPotato/examples/testOBJ/ajax.vdb")
#imesh.loadMesh("/home/kcoley/projects/MeshPotato/examples/testOBJ/ajax.vdb")

print "Printing number of vertices"
print imesh.getNumberVertices()
omesh = vdbout.VDBOutputMesh()

vertices = [["0","0","0"],
	["0","0","1"],
	["0","1","0"],
	["0","1","1"],
	["1","0","0"],
	["1","0","1"],
	["1","1","0"],
	["1","1","1"]
]
normals = [["0","0","1"],
	["0","0","-1"],
	["0","1","0"],
	["0","-1","0"],
	["1","0","0"],
	["-1","0","0"]
]
faces = [["1","7","5"],
	["1","3","7"],
	["1","4","3"],
	["1","2","4"],
	["3","8","7"],
	["3","4","8"],
	["5","7","8"],
	["5","8","6"],
	["1","5","6"],
	["1","6","2"],
	["2","6","8"],
	["2","8","4"]
]
print "Done Conversion!"

print "Example 2: Manual conversion of cube"
omesh_cube = vdbout.VDBOutputMesh()
omesh_cube.loadMesh(mepo.getPythonList(vertices), mepo.getPythonList(normals), mepo.getPythonList(faces), mepo.MeshSpec())
omesh_cube.writeMesh("cube.vdb")

print "Done Manual Conversion for cube!"
