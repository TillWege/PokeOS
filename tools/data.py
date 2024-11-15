import os
import shutil

data_dir = "../data-sources/data/data/api/v2"
assets_dir = "../assets/data"

def extract_pokemon_data():
    source_dir = os.path.join(data_dir, "pokemon")
    target_dir = os.path.join(assets_dir, "pokemon")

    os.makedirs(target_dir, exist_ok=True)

    if os.path.isdir(source_dir):
        for folder_name in os.listdir(source_dir):
            folder_path = os.path.join(source_dir, folder_name)

            if os.path.isdir(folder_path):
                index_file_path = os.path.join(folder_path, 'index.json')

                if os.path.isfile(index_file_path):
                    target_file_path = os.path.join(target_dir, f"{folder_name}.json")
                    shutil.copy(index_file_path, target_file_path)


extract_pokemon_data()
