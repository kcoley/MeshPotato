import pyopenvdb as vdb
import pymeshpotato.mpmesh as mepo
import pymeshpotato.mpvolume as mpvol
import pymeshpotato.mputils as mputils
cube = vdb.FloatGrid()
cube.fill(min=(-19,-19,-19), max=(19,19,19), value=10.0)

sphere1 = vdb.createLevelSetSphere(radius=20, center=(0.0, 0.0, 0.0))
sphere2 = vdb.createLevelSetSphere(radius=20, center=(10.5, 4, 3))
sphere3 = mpvol.ImplicitSphere(1.0, mputils.getMPVec3([0,0,0]))
volume1 = mpvol.VDBVolumeGrid.ptr(sphere1)
volume2 = mpvol.VDBVolumeGrid.ptr(sphere2)
volume3 = mpvol.VDBVolumeGrid.ptr(cube)
clamp_volume1 = mpvol.Clamp.ptr(volume1, 0, -1)
clamp_volume2 = mpvol.Clamp.ptr(volume2, 0, -1)
addedVolume = mpvol.AddVolumeFloat.ptr(clamp_volume1, clamp_volume2)
#print addedVolume.eval([3,4,5])
#coordbbox = mpvol.CoordBBox(
minBB = mpvol.getVDBCoord([-40, -40, -40])
maxBB = mpvol.getVDBCoord([40, 40, 40])
vdbvolume = mpvol.makeVDBGrid(addedVolume, mpvol.getVDBCoordBBox(minBB, maxBB) ,0.5)
vdbvolume2 = mpvol.makeVDBGrid(sphere3, mpvol.getVDBCoordBBox(minBB, maxBB) , 0.05)
vdb.write("testVolume.vdb", grids=[vdbvolume, vdbvolume2, cube])
