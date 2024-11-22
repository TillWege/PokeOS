import os
import shutil

data_dir = "../data-sources/cries"
assets_dir = "../assets/cries"

if not os.path.exists(assets_dir):
    os.makedirs(assets_dir)
    print(f"Created directory: {assets_dir}")

cries_path = os.path.join(data_dir, "cries/pokemon/latest")

if os.path.exists(cries_path):
    for item in os.listdir(cries_path):
        item_path = os.path.join(cries_path, item)
        destination_path = os.path.join(assets_dir, item)
        if os.path.isfile(item_path):
            shutil.move(item_path, destination_path)
    
    print(f"Successfully moved from {cries_path} to {assets_dir}")
else:
    print(f"Source directory does not exist: {cries_path}")
