"""
-------------------------------------------------------------------------------
Creation Info:
Author: Kacey Coley http://www.kaceycoley.com
    
Date: 2014.03.18
Version: 1.0
Requires: Maya 2012 or newer

Release Notes:
1.0 - 2014.03.18 - Initial release

Description:
This command converts a selected mesh into a new mesh based on its extension

Supported Modes:
-N/A

Usage:
Add this file to your plug-in path and load it in Maya from the Plug-in
Manager (Window -> Settings/Preferences -> Plug-in Manager). You can then
execute the command mp_writeMesh.
-------------------------------------------------------------------------------
"""

import maya.OpenMaya as om
import maya.OpenMayaMPx as ommpx
import pyopenvdb as vdb
import pymeshpotato.mpmesh as mepo
import pymeshpotato.pyVDBOutputPlugin as vdbout

class MP_WriteMeshCmd(ommpx.MPxCommand):
    """
    A command that prints a string to the console.
    """
    
    ## the name of the command
    kPluginCmdName = 'mp_writeMesh'
    ## specifies that world space should be used; otherwise local space is used
    kVoxelSizeFlag = '-vs'
    kVoxelSizeLongFlag = '-voxelSize'

    kFileNameFlag = '-fn'
    kFileNameLongFlag = '-fileName'
    def __init__(self):
        """
        Initialize the instance.
        """
        ommpx.MPxCommand.__init__(self)
        self.__voxelSize = 1.0
        self.__fileName = '/tmp/test.vdb'
    
    def doIt(self, args):
        """
        Print string to the console.
        """
        try: argData = om.MArgDatabase(self.syntax(), args) # if this fails, it will raise its own exception...
        except:
            print "failed..." 
            pass # ...so we can just pass here
        else:
            print "converting..."
            # get the active selection
            selection = om.MSelectionList()
            om.MGlobal.getActiveSelectionList( selection )
            iterSel = om.MItSelectionList(selection, om.MFn.kMesh)
            if argData.isFlagSet(MP_WriteMeshCmd.kVoxelSizeFlag):
                self.__voxelSize = argData.flagArgumentDouble(MP_WriteMeshCmd.kVoxelSizeFlag, 0)
            if argData.isFlagSet(MP_WriteMeshCmd.kFileNameFlag):
                self.__fileName = argData.flagArgumentString(MP_WriteMeshCmd.kFileNameFlag, 0)
            # go through selection
            while not iterSel.isDone():
     
                # get dagPath
                dagPath = om.MDagPath()
                iterSel.getDagPath( dagPath )
     
                # create empty point array
                inMeshMPointArray = om.MPointArray()
                # create empy normal array
                inMeshMNormalArray = om.MFloatVectorArray() 
                # create function set and get points and normals in world space
                currentInMeshMFnMesh = om.MFnMesh(dagPath)
                currentInMeshMFnMesh.getPoints(inMeshMPointArray, om.MSpace.kWorld)
                currentInMeshMFnMesh.getNormals(inMeshMNormalArray, om.MSpace.kWorld)
                #get the number of faces for the selection
                numFaces = currentInMeshMFnMesh.numPolygons() 
                normalList = []
                faceList = []
                #get normals
                for i in range(0,numFaces):
                    normal = om.MVector()
                    currentInMeshMFnMesh.getPolygonNormal(i, normal, om.MFn.kMesh)
                    normal = normal.normal()
                    normalList.append([str(normal[0]), str(normal[1]), str(normal[2])])

                #get faces
                for i in range(0, numFaces):
                    vertexL = om.MIntArray()
                    currentInMeshMFnMesh.getPolygonVertices(i, vertexL)
                    vertices = []
                    for n in range(vertexL.length()):
                        vertices.append(str(vertexL[n] + 1))
                        faceList.append(vertices)
                        # put each point to a list
                pointList = []
                for i in range( inMeshMPointArray.length() ) :
                    pointList.append( [str(inMeshMPointArray[i][0]), str(inMeshMPointArray[i][1]), str(inMeshMPointArray[i][2])] )
                # return the vertices, normals and faces
                omesh_cube = vdbout.VDBOutputMesh()
                meshspec = mepo.MeshSpec()
                meshspec.voxelSize = self.__voxelSize
                omesh_cube.loadMesh(mepo.getPythonList(pointList), mepo.getPythonList(normalList), mepo.getPythonList(faceList), meshspec)
                omesh_cube.writeMesh(str(self.__fileName))
                print("Wrote mesh %s" % self.__fileName)

                return [pointList, normalList, faceList]
            
    @classmethod
    def cmdCreator(cls):
        """
        Return pointer to proxy object.
        """
        return ommpx.asMPxPtr(cls())
    @classmethod
    def syntaxCreator(cls):
        """
        Specify custom syntax
        """
        syntax = om.MSyntax()
        syntax.addFlag(cls.kVoxelSizeFlag, cls.kVoxelSizeLongFlag, om.MSyntax.kDouble)
        syntax.addFlag(cls.kFileNameFlag, cls.kFileNameLongFlag, om.MSyntax.kString)
        syntax.useSelectionAsDefault(True) # if a selection list argument is not specified when the command is entered, then use the current selection
        syntax.setObjectType(om.MSyntax.kSelectionList, 1, 1) # this specifies we want the objects as an MSelectionList that may contain only 1 item
        return syntax
def initializePlugin(obj):
    """
    Initialize the plug-in.
    """
    plugin = ommpx.MFnPlugin(
        obj,
        'Kacey Coley',
        '1.0',
        'Any'
    )
    try:
        plugin.registerCommand(
            MP_WriteMeshCmd.kPluginCmdName,
            MP_WriteMeshCmd.cmdCreator,
            MP_WriteMeshCmd.syntaxCreator
        )
    except:
        raise Exception(
            'Failed to register command: %s'%
            MP_WriteMeshCmd.kPluginCmdName
        )

def uninitializePlugin(obj):
    """
    Uninitialize the plug-in.
    """
    plugin = ommpx.MFnPlugin(obj)
    try:
        plugin.deregisterCommand(MP_WriteMeshCmd.kPluginCmdName)        
    except:
        raise Exception(
            'Failed to unregister command: %s'%
            MP_WriteMeshCmd.kPluginCmdName
        )