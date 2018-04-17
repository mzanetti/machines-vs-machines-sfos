#!/usr/bin/python

import os
import json
import sys
import subprocess
import shutil
import fileinput
from bzrlib.branch import Branch
from bzrlib.workingtree import WorkingTree
from bzrlib.errors import PointlessMerge
from bzrlib.errors import UncommittedChanges
from bzrlib.commit import Commit

# bzr plugins
from bzrlib.plugin import load_plugins
load_plugins()

def exportImage(svgname, pngname, imageWidth):
  devnull = open('/dev/null', 'w')
  if not os.path.isfile(pngname):
    try:
      subprocess.call(["inkscape", "-f", str(svgname), "-e", str(pngname), "-C", "-w", str(imageWidth)], stdout=devnull, stderr=devnull)
    except:
      print("\nError running inkscape. Please make sure you have inkscape installed.")
      sys.exit(1)


baseSize = 128
if len(sys.argv) > 1:
  baseSize = int(sys.argv[1])

if not os.path.exists("levelpacks"):
  print("Checking out level raw data")
  remoteBranch = Branch.open("lp:machines-vs-machines/levelpacks")
  remoteBranch.create_checkout("levelpacks/", lightweight=False)
else:
  print("Using already checked out levelpacks dir.")
  remoteBranch = Branch.open("lp:machines-vs-machines/levelpacks")
  localBranch = WorkingTree.open("levelpacks")
#  localBranch.revert()
  try:
    localBranch.merge_from_branch(remoteBranch)
    comm = Commit()
    changes = localBranch.changes_from(localBranch.basis_tree())
    print("%s files modified, %s added, %s removed" % (len(changes.modified), len(changes.added), len(changes.removed)))
    localBranch.update()
    comm.commit(working_tree=localBranch, message="Merge from trunk", local=True)
  except UncommittedChanges as e:
    print("WARNING: Cannot pull from server. You have changes in your local levelpacks dir. Please revert or commig them.")
    print("Do you want to continue generating from the local copy? (Y/n)")
    line = sys.stdin.readline()
    if line == "Y\n" or line == "y\n" or line == "\n":
      pass
    else:
      sys.exit(0)
  except PointlessMerge as e:
    print("Merge failed: %s" % e)


print("Generating level packs for a field size of %s" % baseSize)
  
levelpacks = os.walk("levelpacks/").next()[1]
levelpacks = [ elem for elem in levelpacks if not elem.startswith(".") ]

for levelpack in levelpacks:
  print("\n\n*** Found level pack: %s"% levelpack)
  try:
    os.makedirs("lpbuild/" + levelpack + "/towers/")
    os.makedirs("lpbuild/" + levelpack + "/enemies/")
    os.makedirs("lpbuild/" + levelpack + "/sounds/")
  except:
    pass

  levelpackFile = open("levelpacks/" + levelpack + "/levelpack.json", "r")
  levelpackMap = json.loads(levelpackFile.read())
  titleImage = levelpackMap['titleImage']
  imageWidth = baseSize * 10
  svgname = os.path.join("levelpacks", levelpack, ("%s.svg" % titleImage))
  pngname = os.path.join("lpbuild", levelpack, ("%s.png" % titleImage))

  if os.path.isfile(svgname):
    exportImage(svgname, pngname, imageWidth / 2)
  else:
    print("WARNING: Main SVG image not found: %s" % svgname)

  levelpackMap['titleImage'] = titleImage + ".png"

  # end images
  svgname = os.path.join("levelpacks", levelpack, "finish-easy.svg")
  pngname = os.path.join("lpbuild", levelpack, "finish-easy.png")
  if os.path.isfile(svgname):
    exportImage(svgname, pngname, imageWidth)
  else:
    print("WARNING: finish-easy.svg image not found: %s" % svgname)

  svgname = os.path.join("levelpacks", levelpack, "finish-medium.svg")
  pngname = os.path.join("lpbuild", levelpack, "finish-medium.png")
  if os.path.isfile(svgname):
    exportImage(svgname, pngname, imageWidth)
  else:
    print("WARNING: finish.medium.svg image not found: %s" % svgname)

  svgname = os.path.join("levelpacks", levelpack, "finish-hard.svg")
  pngname = os.path.join("lpbuild", levelpack, "finish-hard.png")
  if os.path.isfile(svgname):
    exportImage(svgname, pngname, imageWidth)
  else:
    print("WARNING: finish-hard.svg image not found: %s" % svgname)



  outfile = open("lpbuild/" + levelpack + "/levelpack.json", "w")
  outfile.write(json.dumps(levelpackMap, indent=2))


  towersfile = open("levelpacks/" + levelpack + "/towers/towers.json", "r")
  towersmap = json.loads(towersfile.read())

  print("\n\nPreparing towers:\n")

  for towermap in towersmap:
    print("Preparing tower: %s" % towermap['name'])
    for towerconfig in towermap['configs']:
      spriteCount = 1;
      if "spriteCount" in towerconfig:
        spriteCount = towerconfig["spriteCount"]
      imagename = towerconfig['image']
      imageWidth = spriteCount * baseSize
      shotImageName = towerconfig['shotImage']
      shotSound = towerconfig['shotSound']
      svgname = os.path.join("levelpacks", levelpack, "towers", ("%s.svg" % imagename))
      pngname = os.path.join("lpbuild", levelpack, "towers", ("%s.png" % imagename))
      print("Tower config: image: %s, sprite count: %i, shot image: %s" % (imagename, spriteCount, shotImageName))

      # Write the correct output format to towers file
      towerconfig['image'] = imagename + ".png"
      towerconfig['imageBaseSize'] = baseSize
      towerconfig['shotImage'] = shotImageName + ".png"
      towerconfig['shotCenter']['x'] = towerconfig['shotCenter']['x'] * baseSize / 255
      towerconfig['shotCenter']['y'] = towerconfig['shotCenter']['y'] * baseSize / 255
      towerconfig['shotStartDistance'] = towerconfig['shotStartDistance'] * baseSize / 255

      if os.path.isfile(svgname):
        exportImage(svgname, pngname, imageWidth)
      else:
        print("WARNING: SVG image not found %s" % svgname)

      svgname = os.path.join("levelpacks", levelpack, "towers", ("%s.svg" % shotImageName))
      pngname = os.path.join("lpbuild", levelpack, "towers", ("%s.png" % shotImageName))

      if os.path.isfile(svgname):
        exportImage(svgname, pngname, imageWidth / 2)
      else:
        print("WARNING: Shot SVG image not found: %s" % svgname)
      
      oggname = os.path.join("levelpacks", levelpack, "towers", ("%s.ogg" % shotSound))
      wavname = os.path.join("lpbuild", levelpack, "towers", ("%s.wav" % shotSound))
      devnull = open('/dev/null', 'w')
      try:
        subprocess.call(["oggdec", str(oggname), "-o", str(wavname)], stdout=devnull, stderr=devnull)
      except:
        print("\nError running oggdec. Please make sure you have vorbis-tools installed")
        sys.exit(1)

  
  outfile = open("lpbuild/" + levelpack + "/towers/towers.json", "w")
  outfile.write(json.dumps(towersmap, indent=2))

  print("\n\nPreparing enemies:\n")

  enemiesfile = open("levelpacks/" + levelpack + "/enemies/enemies.json", "rw")
  enemiesmap = json.loads(enemiesfile.read())

  for enemymap in enemiesmap:
    spriteCount = 1
    if "spriteCount" in enemymap:
      spriteCount = enemymap['spriteCount']
    imagename = enemymap['image']
    svgname = os.path.join("levelpacks", levelpack, "enemies", ("%s.svg" % imagename))
    pngname = os.path.join("lpbuild", levelpack, "enemies", ("%s.png" % imagename))
    print("Preparing enemy: image: %s, sprite count: %i" % (imagename, spriteCount))
    imageWidth = spriteCount * baseSize

    # Write the correct output format to enemies file
    enemymap['image'] = imagename + ".png"
    enemymap['imageBaseSize'] = baseSize

    if not os.path.isfile(svgname):
      print("WARNING: SVG image not found %s" % svgname)
      continue;

    exportImage(svgname, pngname, imageWidth)

  outfile = open("lpbuild/" + levelpack + "/enemies/enemies.json", "w")
  outfile.write(json.dumps(enemiesmap, indent=2))

  oggname = os.path.join("levelpacks", levelpack, "enemies", "Explosion.ogg")
  wavname = os.path.join("lpbuild", levelpack, "enemies", "Explosion.wav")
  devnull = open('/dev/null', 'w')
  try:
    print("Decoding Explosion sound")
    subprocess.call(["oggdec", str(oggname), "-o", str(wavname)], stdout=devnull)
  except:
    print("\nError running oggdec. Please make sure you have vorbis-tools installed")
    sys.exit(1)

  for i in range(0, 5):
    svgname = os.path.join("levelpacks", levelpack, "enemies", ("scrap%s.svg" % i))
    pngname = os.path.join("lpbuild", levelpack, "enemies", ("scrap%s.png" % i))
    exportImage(svgname, pngname, imageWidth)


  print("\n\nPreparing levels:\n")
  
  levels = os.walk("levelpacks/" + levelpack).next()[1]
  levels = [ elem for elem in levels if not elem.startswith(".") and not elem == "towers" and not elem == "enemies" and not elem == "sounds" ]

  for level in levels:
    levelfile = open("levelpacks/" + levelpack + "/" + level + "/level.json", "rw")
    try:
      levelmap = json.loads(levelfile.read())
    except ValueError as e:
      print("ERROR loading level file: %s" % e)
      continue

    try:
      os.makedirs("lpbuild/" + levelpack + "/" + level)
    except:
      pass

    svgname = os.path.join("levelpacks", levelpack, level, "board.svg")
    pngname = os.path.join("lpbuild", levelpack, level, "board.png")
    
    print("Preparing Level: %s, rows %s, cols %s, image: %s" % (level, levelmap['board']['rows'], levelmap['board']['columns'], svgname))
    
    imageWidth = levelmap['board']['columns'] * baseSize/2 + 8 * baseSize/2

    if not os.path.isfile(svgname):
      print("WARNING: SVG image not found %s" % svgname)
      continue;

    exportImage(svgname, pngname, imageWidth)

    imageWidth = baseSize * 2
    pngname = os.path.join("lpbuild", levelpack, level, "board-preview.png")
    exportImage(svgname, pngname, imageWidth)

    outfile = open("lpbuild/" + levelpack + "/" + level + "/level.json", "w")
    outfile.write(json.dumps(levelmap, indent=2))


  print("\n\nPreparing sounds:\n\n")

  path = "levelpacks/" + levelpack + "/sounds/"
  sounds = [ f for f in os.listdir(path) ]
  sounds = [ elem for elem in sounds if not elem.startswith(".") and elem.endswith(".ogg") ]

#  for sound in sounds:
#    oggname = os.path.join("levelpacks", levelpack, "sounds", sound)
#    wavname = os.path.join("lpbuild", levelpack, "sounds", ("%s.wav" % sound.split(".")[0]))
#    devnull = open('/dev/null', 'w')
#    try:
#      print("Converting sound: %s -> %s" % (oggname, wavname))
#      subprocess.call(["oggdec", str(oggname), "-o", str(wavname)], stdout=devnull, stderr=devnull)
#    except:
#      print("\nError running oggdec. Please make sure you have vorbis-tools installed")
#      sys.exit(1)

  for sound in sounds:
    oggname = os.path.join("levelpacks", levelpack, "sounds", sound)
    outname = os.path.join("lpbuild", levelpack, "sounds", ("%s.ogg" % sound.split(".")[0]))
    subprocess.call(["cp", str(oggname), str(outname)], stdout=devnull, stderr=devnull)
