import os
import zipfile
import urllib.request
import shutil

DATA_URL = "https://github.com/PokeAPI/api-data/archive/refs/tags/2024-05-07.zip"
SPIRTE_URL = "https://github.com/PokeAPI/sprites/archive/refs/tags/2.0.0.zip"
CRIES_URL = "https://github.com/PokeAPI/cries/archive/refs/heads/main.zip"

data_dir = "../data-sources"

class File:
    def __init__(self, url, name):
        self.url = url
        self.name = name

files = [
    File(DATA_URL, "data.zip"),
    File(SPIRTE_URL, "sprites.zip"),
    File(CRIES_URL, "cries.zip")
]

def show_progress(block_num, block_size, total_size):
    downloaded = block_num * block_size
    progress = (downloaded / total_size)
    print(f"Downloaded {downloaded} of {total_size} bytes ({progress:.2%})", end="\r")

def is_needed(file: File):
    return os.path.exists(os.path.join(data_dir, file.name.replace(".zip", "")))

def download_files(file: File):
    if os.path.exists(file.name):
        print(f"{file.name} already exists")
        return

    print(f"Downloading {file.name}")
    urllib.request.urlretrieve(file.url, file.name, show_progress)

def extract_files(file: File):
    print(f"Extracting {file.name}")

    with zipfile.ZipFile(file.name, "r") as zip_ref:
        zip_ref.extractall('.')
        all_paths = zip_ref.namelist()
        top_level_dirs = {os.path.normpath(path).split(os.sep)[0] for path in all_paths if '/' in path or '\\' in path}
        if len(top_level_dirs) != 1:
            print(f"Error: {file.name} does not have a single top level directory")
            return
        else:
            top_level_dir = top_level_dirs.pop()
            os.rename(top_level_dir, file.name.replace(".zip", ""))
            shutil.move(file.name.replace(".zip", ""), data_dir)

def delete_archive(file: File):
    print(f"Deleting {file.name}")
    os.remove(file.name)

if not os.path.exists(data_dir):
    os.makedirs(data_dir)

for file in files:
    if is_needed(file):
        print(f"{file.name.replace('.zip', '')} already exists")
        continue
    download_files(file)
    extract_files(file)
    delete_archive(file)
