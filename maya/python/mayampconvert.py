import maya.OpenMaya as OpenMaya
import pyopenvdb as vdb
import pymeshpotato.mpmesh as mepo
import pymeshpotato.pyVDBOutputPlugin as vdbout
import maya.cmds as cmds
import os
 
def particleFillSelection(  ):
	print "running particleFillSelection" 
	# get the active selection
	selection = OpenMaya.MSelectionList()
	OpenMaya.MGlobal.getActiveSelectionList( selection )
	iterSel = OpenMaya.MItSelectionList(selection, OpenMaya.MFn.kMesh)
 
	# go through selection
	while not iterSel.isDone():
 
		# get dagPath
		dagPath = OpenMaya.MDagPath()
		iterSel.getDagPath( dagPath )
 
		# create empty point array
		inMeshMPointArray = OpenMaya.MPointArray()
		inMeshMNormalArray = OpenMaya.MFloatVectorArray() 
		# create function set and get points in world space
		currentInMeshMFnMesh = OpenMaya.MFnMesh(dagPath)
		currentInMeshMFnMesh.getPoints(inMeshMPointArray, OpenMaya.MSpace.kWorld)
		currentInMeshMFnMesh.getNormals(inMeshMNormalArray, OpenMaya.MSpace.kWorld)

		numFaces = currentInMeshMFnMesh.numPolygons() 
		normalList = []
		faceList = []
		#get normals
		for i in range(0,numFaces):
			normal = OpenMaya.MVector()
			currentInMeshMFnMesh.getPolygonNormal(i, normal, OpenMaya.MFn.kMesh)
			normal = normal.normal()
			normalList.append([str(normal[0]), str(normal[1]), str(normal[2])])

	
		#get faces
		for i in range(0, numFaces):
			vertexL = OpenMaya.MIntArray()
			currentInMeshMFnMesh.getPolygonVertices(i, vertexL)
			vertices = []
			for n in range(vertexL.length()):
				vertices.append(str(vertexL[n] + 1))
			faceList.append(vertices)
		# put each point to a list
		pointList = []
		for i in range( inMeshMPointArray.length() ) :
			pointList.append( [str(inMeshMPointArray[i][0]), str(inMeshMPointArray[i][1]), str(inMeshMPointArray[i][2])] )

		return [pointList, normalList, faceList]
def runExample(voxelSize, filePath):
	print "running example"
	listing = particleFillSelection()
	omesh_cube = vdbout.VDBOutputMesh()
	meshspec = mepo.MeshSpec()
	meshspec.voxelSize = voxelSize
	omesh_cube.loadMesh(mepo.getPythonList(listing[0]), mepo.getPythonList(listing[1]), mepo.getPythonList(listing[2]), meshspec)
	omesh_cube.writeMesh(filePath)




def printMessage(*args):
	print "Hello World"


if cmds.window(qtWin, exists=True):
	cmds.deleteUI(qtWin)

meshpotato_dir = os.environ['MESHPOTATO_DIR']
## Define path to ui file
pathToFile = meshpotato_dir + '/maya/python/mpconvertForMaya/ui/mpconvertForMaya.ui'

## Load our window and put it into a variable.
qtWin = cmds.loadUI(uiFile=pathToFile)
 
#cmds.deleteUI("qtWin", window=True)
## Open our window
cmds.showWindow(qtWin)

# Resize the main window
#
def convert():
	voxelSize = cmds.textField("voxelSizeField", query = True, text = True)
	filePath = cmds.textField("filePathField", query = True, text = True)
	runExample(float(voxelSize), str(filePath))
