import os
import shutil

data_dir = "../data-sources/cries"
assets_dir = "../assets/cries"

if not os.path.exists(assets_dir):
    os.makedirs(assets_dir)
    print(f"Created directory: {assets_dir}")

cires_path = os.path.join(data_dir, "cries/pokemon/latest")

if os.path.exists(cires_path):
    shutil.move(cires_path, assets_dir)
    print(f"Successfully moved from {cires_path} to {assets_dir}")
else:
    print(f"Source directory does not exist: {cires_path}")
