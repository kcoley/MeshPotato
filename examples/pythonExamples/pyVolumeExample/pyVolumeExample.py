import pyopenvdb as vdb
import pymeshpotato.mpmesh as mepo
import pymeshpotato.mpvolume as mpvol

cube = vdb.FloatGrid()
cube.fill(min=(-100,-100,-100), max=(199,199,199), value=-10.0)

sphere1 = vdb.createLevelSetSphere(radius=20, center=(1.5, 14, 5))
sphere2 = vdb.createLevelSetSphere(radius=20, center=(1.5, 4, 3))
volume1 = mpvol.VDBVolumeGrid.ptr(sphere1)
volume2 = mpvol.VDBVolumeGrid.ptr(sphere2)
volume3 = mpvol.VDBVolumeGrid.ptr(cube)
clamp_volume1 = mpvol.Clamp.ptr(volume1, 0, 1)
clamp_volume2 = mpvol.Clamp.ptr(volume2, 0, 1)
addedVolume = mpvol.AddVolumeFloat.ptr(clamp_volume1, clamp_volume2)
#print addedVolume.eval([3,4,5])
#coordbbox = mpvol.CoordBBox(
minBB = mpvol.getVDBCoord([-40, -40, -40])
maxBB = mpvol.getVDBCoord([40, 40, 40])
vdbvolume = mpvol.makeVDBGrid(addedVolume, mpvol.getVDBCoordBBox(minBB, maxBB) , 1.0)
vdb.write("testVolume.vdb", vdbvolume)
