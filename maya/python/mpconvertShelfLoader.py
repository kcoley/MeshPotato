#Use to load to shelf in maya manually
import os, sys
path = os.environ['MESHPOTATO_DIR']+'/maya/python'

if path not in sys.path:
    print('meshpotato >> append the current path\n')
    sys.path.append(path)

import mayampconvert as main
reload(main)

main.main()
def closeWindow():
	main.closeWindow()
def convert():
	main.convert()
def browse():
	main.browse()
