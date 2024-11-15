import os
import shutil

data_sources_path = "../data-sources"

if os.path.exists(data_sources_path):
    shutil.rmtree(data_sources_path)
    print(f"Successfully deleted the directory: {data_sources_path}")
else:
    print(f"The directory does not exist: {data_sources_path}")
