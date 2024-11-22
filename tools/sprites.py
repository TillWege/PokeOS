import os
import shutil

data_dir = "../data-sources/sprites"
assets_dir = "../assets/sprites"

if not os.path.exists(assets_dir):
    os.makedirs(assets_dir)

sprites_path = os.path.join(data_dir, "sprites/pokemon")

if os.path.exists(sprites_path):
    for item in os.listdir(sprites_path):
        item_path = os.path.join(sprites_path, item)
        destination_path = os.path.join(assets_dir, item)
        if os.path.isfile(item_path):
            shutil.move(item_path, destination_path)
else:
    print(f"Source directory {sprites_path} does not exist.")