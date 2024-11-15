import sqlite3
import dataclasses
import os
import json

data_dir = "../assets/data"
db_file = "../assets/pokemon.db"

db = sqlite3.connect(db_file)

cursor = db.cursor()

def create_pokemon_table():
    cursor.execute("""
        CREATE TABLE IF NOT EXISTS pokemon (
            id INTEGER PRIMARY KEY,
            name TEXT NOT NULL
        )
    """)

@dataclasses.dataclass
class Pokemon:
    number: int
    name: str

def insert_pokemon(pkm: Pokemon):
    cursor.execute("INSERT INTO pokemon (id, name) VALUES (?, ?)", (pkm.number, pkm.name))
    db.commit()

if __name__ == "__main__":
    create_pokemon_table()
    pokemon_dir = os.path.join(data_dir, "pokemon")

    for file_name in os.listdir(pokemon_dir):
        file_path = os.path.join(pokemon_dir, file_name)
        with open(file_path, 'r') as file:
            data = json.load(file)
            pokemon = Pokemon(number=data['id'], name=data['name'])
            insert_pokemon(pokemon)