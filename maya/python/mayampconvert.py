import maya.OpenMaya as OpenMaya
import pyopenvdb as vdb
import pymeshpotato.mpmesh as mepo
import pymeshpotato.pyVDBOutputPlugin as vdbout
import maya.cmds as cmds
import os


"""
def printMessage(*args):
	print "Hello World"
try:
	if cmds.window(qtWin, exists=True):
		cmds.deleteUI(qtWin)
except NameError:
	pass


meshpotato_dir = os.environ['MESHPOTATO_DIR']
## Define path to ui file
pathToFile = meshpotato_dir + '/maya/python/mpconvertForMaya/ui/mpconvertForMaya.ui'

## Load our window and put it into a variable.
qtWin = cmds.loadUI(uiFile=pathToFile)

#cmds.deleteUI("qtWin", window=True)
## Open our window
def main():
	cmds.showWindow(qtWin)

# Resize the main window
#
def closeWindow():
	cmds.deleteUI(qtWin)
def convert():
	voxelSize = cmds.textField("voxelSizeField", query = True, text = True)
	filePath = cmds.textField("filePathField", query = True, text = True)
	cmds.mp_writeMesh(voxelSize=float(voxelSize), fileName=str(filePath))
#	runExample(float(voxelSize), str(filePath))
	if cmds.checkBox("vdbViewCheckBox", query = True, value = True):
		os.system("vdb_view " + str(filePath) + " & ")
	if cmds.checkBox("meshViewerCheckBox", query = True, value = True):
		os.system("meshviewer " + str(filePath) + " & ")

def browse():
	newfilepath = cmds.fileDialog2(fm=0, okc='Save file as')
	cmds.textField("filePathField", e=1, text=str(newfilepath[0]))
"""
class MPConvert(object):
    use = None
    @classmethod
    def showUI(cls, uiFile):
        win = cls(uiFile)
        win.create()
        return win
    def __init__(self):
        MPConvert.use = self
        #self.filePath = filePath
        self.uiFile = os.environ['MESHPOTATO_DIR']+'/maya/python/mpconvertForMaya/ui/mpconvertForMaya.ui'
        self.qtWin = 'mpConvertPtrWindow'
    def create(self, verbose=False):
        if cmds.window(self.qtWin, exists = True):
            cmds.deleteUI(self.qtWin)
        self.qtWin = cmds.loadUI(uiFile=self.uiFile, verbose=verbose)
        cmds.showWindow(self.qtWin)
    def closeWindow(self):
        cmds.deleteUI(self.qtWin)
    def convert(self):
        voxelSize = self.getVoxelSize()
        filePath = self.getFilePath()
        cmds.mp_writeMesh(voxelSize=voxelSize, fileName=filePath)
        #runExample(voxelSize, filePath)
        print "Done conversion"
        if self.useMeshViewer():
            print "Using meshviewer"
            os.system("meshviewer " + filePath + " & ")
        if self.useVDBView():
            print "Using vdb_view"
            os.system("vdb_view " + filePath + " & ")
    def browse(self):
        newfilepath = cmds.fileDialog2(fm=0, okc='Save file as')
        cmds.textField("filePathField", e=1, text=str(newfilepath[0]))
    def getVoxelSize(self):
        return float(cmds.textField("voxelSizeField", query = True, text = True))
    def getFilePath(self):
        return cmds.textField("filePathField", query = True, text = True)
    def useVDBView(self):
        return cmds.checkBox("vdbViewCheckBox", query = True, value = True)
    def useMeshViewer(self):
        return cmds.checkBox("meshViewerCheckBox", query = True, value = True)
