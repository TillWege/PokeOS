import os
import zipfile
import requests
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

def show_progress(downloaded, total_size):
    if total_size == -1:
        print(f"Downloading {downloaded} of unknown size", end="\r")
    else:
        progress = (downloaded / total_size) * 100
        print(f"Downloaded {downloaded} of {total_size} bytes ({progress:.2f}%)", end="\r")

def is_needed(file: File):
    return os.path.exists(os.path.join(data_dir, file.name.replace(".zip", "")))

def download_files(file: File):
    if os.path.exists(file.name):
        print(f"{file.name} already exists")
        return

    print(f"Downloading {file.name}")
    response = requests.get(file.url, stream=True)
    
    total_size = int(response.headers.get('content-length', -1))
    downloaded = 0

    with open(file.name, 'wb') as f:
        for data in response.iter_content(chunk_size=1024):
            f.write(data)
            downloaded += len(data)
            show_progress(downloaded, total_size)

    print()  # Move to the next line after download is complete


def extract_files(file: File):
    print(f"Extracting {file.name}")

    with zipfile.ZipFile(file.name, "r") as zip_ref:
        zip_ref.extractall('.')
        all_paths = zip_ref.namelist()
    
    top_level_dirs = {os.path.normpath(path).split(os.sep)[0] for path in all_paths if '/' in path or '\\' in path}
    if len(top_level_dirs) != 1:
        print(f"Error: {file.name} does not have a single top level directory")
        return
    
    top_level_dir = top_level_dirs.pop()
    new_dir_name = file.name.replace(".zip", "")
    
    try:
        os.rename(top_level_dir, new_dir_name)
    except PermissionError:
        print(f"PermissionError: Could not rename {top_level_dir} to {new_dir_name}. Check if the directory is not in use.")
        return
    except Exception as e:
        print(f"Error: {e}")
        return
    
    try:
        shutil.move(new_dir_name, data_dir)
    except PermissionError:
        print(f"PermissionError: Could not move {new_dir_name} to {data_dir}. Check if the directory is not in use.")
        return
    except Exception as e:
        print(f"Error: {e}")
        return

    print(f"Successfully extracted and moved {file.name}")


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
